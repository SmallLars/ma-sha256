#include "producer.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/syscall.h>

Producer::Producer(unsigned taskLengthMax) {
    if (taskLengthMax > MAX_VARS) {
        printf("MAX_VARS in producer.h zu klein ...\n");
        exit(EXIT_FAILURE);
    }

    semid = semget(1337, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid < 0) {
        printf("Fehler beim Anlegen des Semaphors ...\n");
        exit(EXIT_FAILURE);
    } else {
        if (semctl(semid, 0, SETVAL, (int) 1) == -1) {
            printf("Fehler bei Initialisierung des Semaphors ...\n");
            exit(EXIT_FAILURE);
        }
    }

    taskLength = 1;
    this->taskLengthMax = taskLengthMax;
    outStart = 0;
    reset();
    progress = 0;
    work_amount = 0;
}

Producer::~Producer() {
    semctl(semid, 0, IPC_RMID, 0);
}

void Producer::addVar(unsigned var) {
    vars.push_back(var);

    unsigned divisor = 1;
    work_amount = vars.size();                                     // Für TaskLength == 1
    for (unsigned t = 1; t < taskLengthMax; t++) {                 // Für jede weitere TaskLength
        divisor *= t;
        for (unsigned o = 1; o <= vars.size() - outStart; o++) {   // Für jeden Output
            unsigned dividend = vars.size() - o;
            for (unsigned mul = 1; mul < t; mul++) {
                dividend *= (vars.size() - o - mul);
            }
            work_amount += (dividend / divisor);
        }
    }
}

void Producer::setOutStart() {
    outStart = vars.size();
    reset();
}

int Producer::getWork(std::vector<unsigned>& task) {
   sembuf semaphore;
   semaphore.sem_num = 0;

   semaphore.sem_op = -1;
   semaphore.sem_flg = SEM_UNDO;
   if (semop(semid, &semaphore, 1) == -1) {
      perror(" semop-enter ");
      return 0;
   }

   //printf ("PID %d ENTER\n", syscall(SYS_gettid));

   if (counter[0] == vars.size() - (taskLength - 1)) {
       if (taskLength < taskLengthMax) {
           taskLength++;
           printf("\n");
           reset();
       } else {
           semaphore.sem_op = 1;
           semaphore.sem_flg = SEM_UNDO;
           semop(semid, &semaphore, 1);
           return 0;
       }
   }

   task.clear();
//   printf(" -> ");
   for (unsigned i = 0; i < taskLength; i++) {
       task.push_back(vars[counter[i]]);
//       printf("%u ", vars[counter[i]]);
   }
//   printf("\n");

   progress++;
   printf("\r %u / %u: %9u / %9u", taskLength, taskLengthMax, progress, work_amount);
   fflush(stdout);

   counter[taskLength - 1]++;
   for (unsigned i = taskLength - 1; i > 0; i--) {
       if (counter[i] == vars.size() - (taskLength - 1 - i)) {
           counter[i - 1]++;
           for (unsigned j = i; j < taskLength; j++) counter[j] = counter[j - 1] + 1;
       }
   }
   if (taskLength > 1 && counter[taskLength - 1] < outStart) counter[taskLength - 1] = outStart;

   semaphore.sem_op = 1;
   semaphore.sem_flg = SEM_UNDO;
   if (semop(semid, &semaphore, 1) == -1) {
      perror(" semop-leave ");
      return 0;
   }

   return 1;
}

void Producer::reset() {
    for (unsigned i = 0; i < MAX_VARS; i++) counter[i] = i;
    if (taskLength > 1 && counter[taskLength - 1] < outStart) counter[taskLength - 1] = outStart;
}
