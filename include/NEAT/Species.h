//
// Created by barth on 9/6/18.
//

#ifndef SCRIPTBOTS_SPECIES_H
#define SCRIPTBOTS_SPECIES_H

#include <list>
#include <vector>
#include "Agent.h"

class Agent;

namespace NEAT {
    class Species {

        int _id;
        std::list<Agent*> _agents;
        unsigned long _popMaxRecord;
        int _totalMembersOverTime;
        bool _drawn;

    public:

        Species(int i);
        Species(std::ifstream &inFile, std::vector<Agent*> all_agents);

        int getId();
        void addAgent(Agent *agent);
        void removeAgent(Agent *agent);
        Agent *first();
        int getNumberOfAgents();
        int empty();
        unsigned long getPopMaxRecord();
        int getTotalMembersOverTime();
        void setDrawn(bool drawn);
        bool isDrawn();
        void saveToFile(std::ofstream &outFile);
    };
}


#endif //SCRIPTBOTS_SPECIES_H
