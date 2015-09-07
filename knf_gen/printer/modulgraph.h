#ifndef __MODULGRAPH_H__
#define __MODULGRAPH_H__

#include "printer.h"

#include <list>
#include <set>
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
        void printGraph(const char* filename, const std::vector<CMSat::Lit>& highlight = std::vector<CMSat::Lit>());

        void calcDistances();
        unsigned getModulCount(const std::vector<CMSat::Lit>& clause);
        unsigned getDistance(const std::vector<CMSat::Lit>& clause);
        unsigned getDistance(const std::vector<unsigned>& literale);
    private:
        std::list<Node> module;
        std::map<unsigned, Node*> varToNode;

        void getModule(std::set<Node*>& module, const std::vector<CMSat::Lit>& clause);
        void getModule(std::set<Node*>& module, const std::vector<unsigned>& literale);
};

#endif //__MODULGRAPH_H__
