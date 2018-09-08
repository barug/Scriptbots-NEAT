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
    _totalMembersOverTime(0)
{

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
        //cout << "species : " << _id << " size pre erase : " << _agents.size() << std::endl;
        auto it = std::find(_agents.begin(), _agents.end(), agent);
        _agents.erase(it);
        //cout << "size post erase : " << _agents.size() << endl;
    } catch (std::exception &e) {}
}

Agent *Species::first()
{
    return *(_agents.begin());
}

int Species::getNumberOAgents()
{
    _agents.size();
}

int Species::empty()
{
    return _agents.empty();
}

int Species::getPopMaxRecord()
{
    return _popMaxRecord;
}

int Species::getTotalMembersOverTime()
{
    return _totalMembersOverTime;
}