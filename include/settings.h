#define INPUTSIZE 25
#define OUTPUTSIZE 9
#define NUMEYES 4
#define BRAINSIZE 200
#define CONNS 4

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class conf {
public:
     static int WIDTH;  //width and height of simulation
     static int HEIGHT;
     static int WWIDTH;  //window width and height
     static int WHEIGHT;

     static int CZ; //cell size in pixels, for food squares. Should divide well into Width Height

     static int NUMBOTS; //initially, and minimally
     static float BOTRADIUS; //for drawing
     static float BOTSPEED;
     static float SPIKESPEED; //how quickly can attack spike go up?
     static float SPIKEMULT; //essentially the strength of every spike impact

     static float DAMAGE_LIMIT; //if different from 0 only agent with herbivor attr under < DAMAGE_LIMIT will do damages;
     static float DAMAGE_GRADIENT; // if different from 0 damage made by agents is on a gradient in relationship to herbivor attributes, with full carn doing full damage

     static int BABIES; //number of babies per agent when they reproduce
     static float BOOSTSIZEMULT; //how much boost do agents get? when boost neuron is on
     static float REPRATEH; //reproduction rate for herbivors
     static float REPRATEC; //reproduction rate for carnivors
     static float MATING_RADIUS;
     static double MATING_COMPATIBILITY_TRESHOLD;
     static double BODY_COMPAT_MULT; // this controls how much agent's body attributes influence compatibility;
     static double BRAIN_COMPAT_MULT; // this controls how much agent's brain genome compatibility influence overall compatibility
     static int MATING_BABIES;

     static float DIST;		//how far can the eyes see on each bot?
     static float MUTRATE1_INIT_LOW;
     static float MUTRATE1_INIT_HIGH;
     static float MUTRATE2_INIT_LOW;
     static float MUTRATE2_INIT_HIGH;
     static float METAMUTRATE1; //what is the change in MUTRATE1 and 2 on reproduction? lol
     static float METAMUTRATE2;

     static float FOODINTAKE; //how much does every agent consume?
     static float FOODWASTE; //how much food disapears if agent eats?
     static float FOODMAX; //how much food per cell can there be at max?
     static int FOODADDFREQ; //how often does random square get to full food?

     static float FOODTRANSFER; //how much is transfered between two agents trading food? per iteration
     static float FOOD_SHARING_DISTANCE; //how far away is food shared between bots?
    
     static float TEMPERATURE_DISCOMFORT; //how quickly does health drain in nonpreferred temperatures (0= disabled. 0.005 is decent value)

     static float FOOD_DISTRIBUTION_RADIUS; //when bot is killed, how far is its body distributed?
    
     static float REPMULT; //when a body of dead animal is distributed, how much of it goes toward increasing birth counter for surrounding bots?


     static void initialize();
     static void load_conf(std::string path);
     static void save_to_file(std::ofstream &outFile);
     static void load_from_save(std::ifstream &inFile);
};

#endif
