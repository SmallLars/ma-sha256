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

        void calcDistances();
        void printGraph(const char* filename);
    private:
        std::list<Node> module;
};

#endif //__MODULGRAPH_H__
