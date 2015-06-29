#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include <vector>

class Producer {
    public:
        Producer(unsigned taskLengthMax);
        ~Producer();

        void addVar(unsigned var);
        int getWork(std::vector<unsigned>& task);
    protected:

    private:
        int semid;
        unsigned taskLength;
        unsigned taskLengthMax;
        unsigned counter[5];
        std::vector<unsigned> vars;

        unsigned progress;
        unsigned work_amount;

        void reset();
};

#endif //__PRODUCER_H__
