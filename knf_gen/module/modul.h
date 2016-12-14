#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>

#include "../collector/collector.h"

#define STATS_LENGTH 3
#define STATS_MAXVAR 0
#define STATS_VARCOUNT 1
#define STATS_CLAUSECOUNT 2

class Modul {
    public:
        Modul(unsigned bitWidth, unsigned inputCount, unsigned outputCount);
        virtual ~Modul();

        unsigned getMaxVar();
        unsigned getVarCount();
        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void setInputs(const std::vector<unsigned>& inputs);
        void setStart(unsigned start);
        void setOutput(unsigned output);
        void import(Collector* collector, const char* filename);

        unsigned getBitWidth();
        std::vector<unsigned>& getInputs();
        std::vector<unsigned>& getInputWidth();

        unsigned getStart();
        unsigned getOutput();
        unsigned getInputNum();
        unsigned getOutputNum();

        unsigned append(CMSat::SATSolver* solver);
        unsigned writeLog(const char* filename);
        unsigned writeDimacs(const char* filename);
        unsigned writeTT(const char* filename);

        virtual unsigned* getStats() = 0;
        virtual void create(Collector* collector) = 0;
    protected:
        std::vector<unsigned> inputs;
        unsigned start;
        unsigned output;

        void setInputsBitWidth(...);

        void createTrue(Collector* collector, unsigned out);
        void createFalse(Collector* collector, unsigned out);
        void createEQ(Collector* collector, unsigned out, unsigned in);
        void createNEQ(Collector* collector, unsigned out, unsigned in);

        void createAND(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert = false);
        void createOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert = false);

        void createXOR(Collector* collector, unsigned out, unsigned in1, bool invert = false);
        void createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert = false);
        void createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, unsigned in3, bool invert = false);
        void createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, unsigned in3, unsigned in4, bool invert = false);
    private:
        unsigned bitWidth;
        unsigned inputCount;
        std::vector<unsigned> inputWidth;

        unsigned inputNum;
        unsigned outputNum;

        unsigned* count();
};

#endif //__MODUL_H__
