#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>

#include "../printer/printer.h"

class Modul {
    public:
        Modul(unsigned inputCount, unsigned inputBitWidth);
        ~Modul();

        unsigned getMaxVar();
        unsigned getVarCount();
        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void setInputs(const std::vector<unsigned>& inputs);
        void setStart(unsigned start);
        void setOutput(unsigned output);

        unsigned append(CMSat::SATSolver* solver);
        unsigned writeDimacs(const char* filename);
        unsigned writeTT(const char* filename);

        virtual void create(Printer* printer) = 0;
    protected:
        std::vector<unsigned> inputs;
        unsigned start;
        unsigned output;

        void createTrue(Printer* printer, unsigned out);
        void createFalse(Printer* printer, unsigned out);
        void createEQ(Printer* printer, unsigned out, unsigned in);
        void createNEQ(Printer* printer, unsigned out, unsigned in);

        void createAND(Printer* printer, unsigned out, unsigned in1, unsigned in2);
        void createXOR(Printer* printer, unsigned out, unsigned in1, unsigned in2, bool invert = false);
        void createOR(Printer* printer, unsigned out, unsigned in1, unsigned in2);

        void createXOR(Printer* printer, unsigned out, unsigned in1, unsigned in2, unsigned in3, bool invert = false);
    private:
        unsigned inputCount;
        unsigned inputBitWidth;
};

#endif //__MODUL_H__
