#ifndef __MODULGRAPH_H__
#define __MODULGRAPH_H__

#include "printer.h"

#include <list>
#include <map>

struct Node {
    const char* name;
    std::vector<Node*> inputs;
    unsigned intern;
    unsigned internCount;
    unsigned output;
    std::vector<Node*> usage;

    unsigned dist;
    std::map<unsigned, unsigned> distance;
};

class ModulGraph : public Printer {
    public:
        ModulGraph();
        ~ModulGraph();

        void newModul(unsigned level, const char* name, Modul* modul);

        void printGraph(const char* filename);

        void calcDistances();
        unsigned getModulCount(std::vector<CMSat::Lit>& clause);
        unsigned getDistance(std::vector<CMSat::Lit>& clause);
    private:
        std::list<Node> module;
        std::map<unsigned, Node*> varToNode;
};

#endif //__MODULGRAPH_H__
