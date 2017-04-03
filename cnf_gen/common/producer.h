#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include <vector>

#define MAX_VARS 10

class Producer {
    public:
        Producer(unsigned taskLengthMax);
        ~Producer();

        void addVar(unsigned var);
        void setOutStart();
        int getWork(std::vector<unsigned>& task);
    protected:

    private:
        int semid;
        unsigned taskLength;
        unsigned taskLengthMax;
        unsigned counter[MAX_VARS];
        std::vector<unsigned> vars;
        unsigned outStart;

        unsigned progress;
        unsigned work_amount;

        void reset();
};

#endif //__PRODUCER_H__
