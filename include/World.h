#ifndef WORLD_H
#define WORLD_H

#include "NEAT/innovation.h"
#include "NEAT/Species.h"
#include "View.h"
#include "Agent.h"
#include "settings.h"
#include "VTKView.h"
#include "VTKPlotView.h"
#include "VTKSpeciesView.h"

#include <vector>

class World {
public:
    World();
    World(std::string path);
    ~World();
    
    void update();
    void reset();
    
    void draw(View* view, bool drawfood);
    
    bool isClosed() const;
    void setClosed(bool close);
    
    /**
     * Returns the number of herbivores and 
     * carnivores in the world.
     * first : num herbs
     * second : num carns
     */
    std::pair<int,int> numHerbCarnivores() const;
    
    int numAgents() const;
    int epoch() const;
    
    //mouse interaction
    void processMouse(int button, int state, int x, int y);

    //void addNewByCrossover();
    void addRandomBots(int num);
    void addCarnivore();
    void addHerbivore();
    
    void positionOfInterest(int type, float &xi, float &yi);

    void printToFile();
    void setSaveFilePath(std::string path);
    
    std::vector<int> numCarnivore;
    std::vector<int> numHerbivore; 
    int ptr;

private:
    void setInputs();
    void processOutputs();
    void brainsTick();  //takes in[] to out[] for every agent
    
    void writeReport();
    
    void reproduce(int ai, float MR, float MR2);
    void mate(Agent *a1, Agent *a2);
    
    void speciateAgent(Agent *agent);

    void removeShortLivedSpecies();


    std::string saveFilePath;

    int modcounter;
    int current_epoch;
    int idcounter;
    
    std::vector<Agent*> agents;
    std::vector<NEAT::Species*> all_species;
    int last_species;
    // ******* Member variables used during reproduction *******
    std::vector<NEAT::Innovation*> innovations;  // For holding the genetic innovations of the newest generation
    int cur_node_id;  //Current label number available
    double cur_innov_num;
    
    // food
    int FW;
    int FH;
    int fx;
    int fy;
    float food[conf::WIDTH/conf::CZ][conf::HEIGHT/conf::CZ];
    bool CLOSED; //if environment is closed, then no random bots are added per time interval
};

#endif // WORLD_H
