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
        int _popMaxRecord;
        int _totalMembersOverTime;

    public:

        Species(int i);

        int getId();
        void addAgent(Agent *agent);
        void removeAgent(Agent *agent);
        Agent *first();
        int getNumberOAgents();
        int empty();
        int getPopMaxRecord();
        int getTotalMembersOverTime();
    };
}


#endif //SCRIPTBOTS_SPECIES_H
