#include "modulgraph.h"

#include <fstream>
#include <iomanip>
#include <string.h>
#include <vector>
#include <algorithm>
#include <utility>

#include "../module/modul.h"

using std::list;
using std::ofstream;
using std::setw;
using std::vector;
using std::find;
using std::pair;

ModulGraph::ModulGraph() {
}

ModulGraph::~ModulGraph() {
/*
    for (unsigned m = 0; m < module.size(); m++) {
        module[m].print(std::cout);
    }
*/
}

void ModulGraph::newModul(unsigned level, const char* name, Modul* modul) {
    if (level != 10) return;

    module.push_back(Node());
    Node& node = module.back();
    node.name = name;
    for (unsigned i = 0; i < modul->getInputs().size(); i++) {
        for (list<Node>::iterator it = module.begin() ; it != module.end(); ++it) {
            if (it->output == modul->getInputs()[i]) {
                it->usage.push_back(&node);
                node.inputs.push_back(&*it);
                break;
            }
        }
    }
    if (node.inputs.size() != modul->getInputs().size()) {
        std::cout << "ALERT: Wasn't able to find all inputs.\n";
    }
    node.intern = modul->getStart();
    node.internCount = modul->getAdditionalVarCount() - modul->getOutputNum();
    node.output = modul->getOutput();
}

void ModulGraph::calcDistances() {
    // Dijkstra for every node
    for (list<Node>::iterator start = module.begin(); start != module.end(); ++start) {
        // Initialisation
        list<Node*> todo;
        for (list<Node>::iterator it = module.begin(); it != module.end(); ++it) {
            it->dist = 0xFFFFFFFF;
            todo.push_back(&*it);
        }
        start->dist = 0;
        // For every node
        while (!todo.empty()) {
            // Find node with smallest distance
            Node* min_dist = *(todo.begin());
            for (list<Node*>::iterator it = todo.begin(); it != todo.end(); ++it) if ((*it)->dist < min_dist->dist) min_dist = *it;
            // Collect neighbours of that node
            vector<Node*> neighbours(min_dist->inputs);
            neighbours.insert(neighbours.end(), min_dist->usage.begin(), min_dist->usage.end());
            // Check for all neighbours in working list if there is a smaller distance
            for (vector<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
                if (find(todo.begin(), todo.end(), *it) != todo.end()) {
                    unsigned new_dist = min_dist->dist + 1;
                    if (new_dist < (*it)->dist) (*it)->dist = new_dist;
                }
            }
            // Remove node from working list
            todo.remove(min_dist);
        }
        // Save result from Dijkstra in start node
        for (list<Node>::iterator it = module.begin(); it != module.end(); ++it) {
            start->distance[it->output] = it->dist;
        }
    }
    // After all clear dist in module
    for (list<Node>::iterator it = module.begin(); it != module.end(); ++it) {
        it->dist = 0xFFFFFFFF;
    }
}

void ModulGraph::printGraph(const char* filename) {
    ofstream outputFile(filename);

//                                      rankdir=LR
    outputFile << "digraph G {\n  graph[ranksep=2]\n\n";

    for (list<Node>::iterator it = module.begin(); it != module.end(); ++it) {
//                                                            ellipse             yellow    blue
        outputFile << setw(7) << it->output << " [shape=square style=filled fillcolor=green label=\"" << it->name << "\"]\n";
    }
    outputFile << "\n";

    outputFile << "  {rank=min 0}\n";
    outputFile << "  {rank=same";
    for (unsigned i = 16; i < 24; i++) outputFile << " " << i * 32;

    for (list<Node>::iterator it = module.begin(); it != module.end();) {
        if (strcmp(it->name, "ConstAdd_32")) {
            ++it;
            continue;
        }

        outputFile << " " << it->output;
        outputFile << "}\n";
        it++;

        vector<unsigned> ranks[4];

        ranks[0].push_back((it++)->output);
        ranks[0].push_back((it++)->output);
        ranks[1].push_back((it++)->output);
        ranks[0].push_back((it++)->output);
        ranks[0].push_back((it++)->output);
        ranks[1].push_back((it++)->output);
        ranks[0].push_back((it++)->output);
        ranks[2].push_back((it++)->output);
        ranks[3].push_back((it++)->output);
        ranks[3].push_back((it++)->output);
     
        if (it != module.end()) {
           if (strcmp(it->name, "Ssig0_32") == 0) {
               ranks[0].push_back((it++)->output);
               ranks[0].push_back((it++)->output);
               ranks[1].push_back((it++)->output);
               ranks[1].push_back((it++)->output);
               ranks[2].push_back((it++)->output);
           }
           ranks[2].push_back(it->inputs[0]->output);
        }

        for (unsigned r = 0; r < 3; r++) {
            outputFile << "  {rank=same";
            for (unsigned i = 0; i < ranks[r].size(); i++) {
                outputFile << " " << ranks[r][i];
            }
            outputFile << "}\n";
        }

        outputFile << "  {rank=same";
        outputFile << " " << ranks[3][0];
        outputFile << " " << ranks[3][1];
    }
    outputFile << "}\n\n";

    for (list<Node>::iterator it = module.begin(); it != module.end(); ++it) {
        for (unsigned e = 0; e < it->usage.size(); e++) {
            outputFile << setw(7) << it->output << " -> " << setw(5) << it->usage[e]->output << " [arrowhead=none]\n";
        }
    }

    outputFile << "}";

    outputFile.close();
}
