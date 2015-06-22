#include "producer.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/syscall.h>

Producer::Producer(unsigned taskLengthMax) {
    semid = semget (1337, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid < 0) {
        printf("Fehler beim Anlegen des Semaphors ...\n");
    } else {
        if (semctl(semid, 0, SETVAL, (int) 1) == -1) {
            printf("Fehler bei Initialisierung des Semaphors ...\n");
        }
    }

    taskLength = 3;
    this->taskLengthMax = taskLengthMax;
    reset();
    progress = 0;
    work_amount = 0;
}

Producer::~Producer() {
    semctl(semid, 0, IPC_RMID, 0);
}

void Producer::addVar(unsigned var) {
    vars.push_back(var);

    work_amount = 0;
    for (unsigned i = 1; i <= taskLengthMax; i++) {
        unsigned tmp = vars.size();
        for (unsigned mul = 1; mul < i; mul++) tmp *= (vars.size() - mul);
        work_amount += (tmp / i);
    }
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
           reset();
       } else {
           semaphore.sem_op = 1;
           semaphore.sem_flg = SEM_UNDO;
           semop(semid, &semaphore, 1);
           return 0;
       }
   }

   task.clear();
   for (unsigned i = 0; i < taskLength; i++) {
       task.push_back(vars[counter[i]]);
//       printf("%u ", vars[counter[i]]);
   }
//   printf("\n");

   progress++;
   printf("\r%u / %u", progress, work_amount);

   counter[taskLength - 1]++;
   for (unsigned i = taskLength - 1; i > 0; i--) {
       if (counter[i] == vars.size() - (taskLength - 1 - i)) {
           counter[i - 1]++;
           for (unsigned j = i; j <= taskLength; j++) counter[j] = counter[j - 1] + 1;
       }
   }

   semaphore.sem_op = 1;
   semaphore.sem_flg = SEM_UNDO;
   if (semop(semid, &semaphore, 1) == -1) {
      perror(" semop-leave ");
      return 0;
   }

   return 1;
}

void Producer::reset() {
    counter[0] = 0;
    counter[1] = 1;
    counter[2] = 2;
    counter[3] = 3;
    counter[4] = 4;
}
