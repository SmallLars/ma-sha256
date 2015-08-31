#ifndef __MODULGRAPH_H__
#define __MODULGRAPH_H__

#include "printer.h"

#include <list>

struct Node {
    const char* name;
    std::vector<Node*> inputs;
    unsigned intern;
    unsigned internCount;
    unsigned output;
    std::vector<Node*> usage;
};

class ModulGraph : public Printer {
    public:
        ModulGraph();
        ~ModulGraph();

        void newModul(unsigned level, const char* name, Modul* modul);

        void printGraph(const char* filename);
    private:
        std::list<Node> module;
};

#endif //__MODULGRAPH_H__
