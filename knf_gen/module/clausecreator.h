#ifndef __CLAUSECREATOR_H__
#define __CLAUSECREATOR_H__

#include <vector>

#include "../collector/collector.h"

#define CC_DC 2

class ClauseCreator {
    public:
        ClauseCreator(Collector* collector);
        ~ClauseCreator();

        Collector* getCollector();
        void setLiterals(unsigned n_args, ...);
        void addLiterals(unsigned n_args, ...);
        void printClause(unsigned n_args, ...);
    protected:

    private:
        std::vector<unsigned> literals;
        Collector* collector;
};

#endif //__CLAUSECREATOR_H__
