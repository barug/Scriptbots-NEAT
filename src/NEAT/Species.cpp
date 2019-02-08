//
// Created by barth on 9/6/18.
//

#include <algorithm>
#include "NEAT/Species.h"

using namespace std;
using namespace NEAT;

Species::Species(int i) :
    _id(i),
    _popMaxRecord(0),
    _totalMembersOverTime(0),
    _drawn(false)
{

}

Species::Species(std::ifstream &inFile, std::vector<Agent*> all_agents)
{
    std::string wordBuff;


    inFile >> wordBuff;
    if (wordBuff != "speciesBegin")
        throw std::runtime_error("bad format : speciesBegin");

    inFile >> _id;
    inFile >> _popMaxRecord;
    inFile >> _totalMembersOverTime;
    inFile >> _drawn;

    int numberOfAgents;
    int agentId;
    inFile >> numberOfAgents;

    for (int i = 0; i < numberOfAgents; ++i) {
        inFile >> agentId;
        std::vector<Agent*>::iterator it =
                std::find_if(all_agents.begin(), all_agents.end(), [agentId](Agent *agent) {
            return agent->id == agentId;
        });
        if (it == all_agents.end()) {
            throw std::runtime_error("load Species : bad agent id " + std::to_string(agentId));
        }
        _agents.push_back(*it);
        (*it)->species = this;
    }

    if (_agents.size() != numberOfAgents) {
        throw std::runtime_error("load Species : wrong number of agents " + std::to_string(_agents.size()));
    }

    inFile >> wordBuff;
    if (wordBuff != "speciesEnd")
        throw std::runtime_error("bad format : speciesEnd");

}

int Species::getId()
{
    return _id;
}

void Species::addAgent(Agent *agent)
{
    _agents.push_back(agent);
    if (_agents.size() > _popMaxRecord) {
        _popMaxRecord = _agents.size();
    }
    ++_totalMembersOverTime;
    agent->species = this;
}

void Species::removeAgent(Agent *agent)
{
    try {
        auto it = std::find(_agents.begin(), _agents.end(), agent);
        _agents.erase(it);
    } catch (std::exception &e) {}
}

Agent *Species::first()
{
    return *(_agents.begin());
}

int Species::getNumberOfAgents()
{
    _agents.size();
}

int Species::empty()
{
    return _agents.empty();
}

unsigned long Species::getPopMaxRecord()
{
    return _popMaxRecord;
}

int Species::getTotalMembersOverTime()
{
    return _totalMembersOverTime;
}

void Species::setDrawn(bool drawn)
{
    _drawn = drawn;
}

bool Species::isDrawn()
{
    return _drawn;
}

void Species::saveToFile(std::ofstream &outFile)
{
    outFile << "speciesBegin" << std::endl;

    outFile << _id << " ";
    outFile << _popMaxRecord << " ";
    outFile << _totalMembersOverTime << " ";
    outFile << _drawn << " ";
    outFile << _agents.size() << std::endl;

    for (auto agent: _agents) {
        outFile << agent->id << " ";
    }

    outFile << std::endl << "speciesEnd" << std::endl;
}

