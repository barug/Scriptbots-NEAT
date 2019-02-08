//
// Created by barth on 9/13/18.
//

#include "settings.h"
#include <iostream>
#include <fstream>
#include <sstream>

int conf::WIDTH;  //width and height of simulation
int conf::HEIGHT;
int conf::WWIDTH;  //window width and height
int conf::WHEIGHT;

int conf::CZ; //cell size in pixels, for food squares. Should divide well into Width Height

int conf::NUMBOTS; //initially, and minimally
float conf::BOTRADIUS; //for drawing
float conf::BOTSPEED;
float conf::SPIKESPEED; //how quickly can attack spike go up?
float conf::SPIKEMULT; //essentially the strength of every spike impact

float conf::DAMAGE_LIMIT; //if different from 0 only agent with herbivor attr under < DAMAGE_LIMIT will do damages;
float conf::DAMAGE_GRADIENT; // if different from 0 damage made by agents is on a gradient in relationship to herbivor attributes, with full carn doing full damage

int conf::BABIES; //number of babies per agent when they reproduce
float conf::BOOSTSIZEMULT; //how much boost do agents get? when boost neuron is on
float conf::REPRATEH; //reproduction rate for herbivors
float conf::REPRATEC; //reproduction rate for carnivors
float conf::MATING_RADIUS;
double conf::MATING_COMPATIBILITY_TRESHOLD;
double conf::BODY_COMPAT_MULT; // this controls how much agent's body attributes influence compatibility;
double conf::BRAIN_COMPAT_MULT; // this controls how much agent's brain genome compatibility influence overall compatibility
int conf::MATING_BABIES;

float conf::DIST;		//how far can the eyes see on each bot?
float conf::MUTRATE1_INIT_LOW;
float conf::MUTRATE1_INIT_HIGH;
float conf::MUTRATE2_INIT_LOW;
float conf::MUTRATE2_INIT_HIGH;

float conf::METAMUTRATE1; //what is the change in MUTRATE1 and 2 on reproduction? lol
float conf::METAMUTRATE2;

float conf::FOODINTAKE; //how much does every agent consume?
float conf::FOODWASTE; //how much food disapears if agent eats?
float conf::FOODMAX; //how much food per cell can there be at max?
int conf::FOODADDFREQ; //how often does random square get to full food?

float conf::FOODTRANSFER; //how much is transfered between two agents trading food? per iteration
float conf::FOOD_SHARING_DISTANCE; //how far away is food shared between bots?

float conf::TEMPERATURE_DISCOMFORT; //how quickly does health drain in nonpreferred temperatures (0= disabled. 0.005 is decent value)

float conf::FOOD_DISTRIBUTION_RADIUS; //when bot is killed, how far is its body distributed?

float conf::REPMULT; //when a body of dead animal is distributed, how much of it goes toward increasing birth counter for surrounding bots?

void conf::initialize() {
    WIDTH = 8000;  //width and height of simulation
    HEIGHT = 4000;
    WWIDTH = 1600;  //window width and height
    WHEIGHT = 900;

    CZ = 50; //cell size in pixels, for food squares. Should divide well into Width Height

    NUMBOTS=70; //initially, and minimally
    BOTRADIUS=10; //for drawing
    BOTSPEED= 0.3;
    SPIKESPEED= 0.005; //how quickly can attack spike go up?
    SPIKEMULT= 1; //essentially the strength of every spike impact

    DAMAGE_LIMIT = 0;
    DAMAGE_GRADIENT = 0;

    BABIES=2; //number of babies per agent when they reproduce
    BOOSTSIZEMULT=2; //how much boost do agents get? when boost neuron is on
    REPRATEH=7; //reproduction rate for herbivors
    REPRATEC=7; //reproduction rate for carnivors
    MATING_RADIUS=100;
    MATING_COMPATIBILITY_TRESHOLD=4.0;
    BODY_COMPAT_MULT=1;
    BRAIN_COMPAT_MULT=1;
    MATING_BABIES=3;
    DIST= 150;		//how far can the eyes see on each bot?
    MUTRATE1_INIT_LOW = 0.001;
    MUTRATE1_INIT_HIGH = 0.005;
    MUTRATE2_INIT_LOW = 0.01;
    MUTRATE2_INIT_HIGH = 0.07;
    METAMUTRATE1= 0.002; //what is the change in MUTRATE1 and 2 on reproduction? lol
    METAMUTRATE2= 0.05;

    FOODINTAKE= 0.002; //how much does every agent consume?
    FOODWASTE= 0.001; //how much. food disapears if agent eats?
    FOODMAX= 0.5; //how much food per cell can there be at max?
    FOODADDFREQ= 15; //how often does random square get to full food?

    FOODTRANSFER= 0.001; //how much is transfered between two agents trading food? per iteration
    FOOD_SHARING_DISTANCE= 50; //how far away is food shared between bots?

    TEMPERATURE_DISCOMFORT = 0; //how quickly does health drain in nonpreferred temperatures (0= disabled. 0.005 is decent value)

    FOOD_DISTRIBUTION_RADIUS=100; //when bot is killed, how far is its body distributed?

    REPMULT = 5; //when a body of dead animal is distributed, how much of it goes toward increasing birth counter for surrounding bots?
}

void conf::load_conf(std::string path)
{
    char curline[4096];
    std::string wordBuff;
    std::ifstream inFile(path, std::ifstream::in);

    inFile.eof();
    //for (ifstream.getline(curline, 4096 && (inFile.rdstate & std::ifstream::eofbit)))
    while (!inFile.eof()) {
        inFile.getline(curline, 4096);
        std::stringstream lineStream(curline);

        lineStream >> wordBuff;
        if (wordBuff == "WIDTH") {
            lineStream >> conf::WIDTH;
        }
        if (wordBuff == "HEIGHT") {
            lineStream >> conf::HEIGHT;
        }
        if (wordBuff == "WWIDTH") {
            lineStream >> conf::WWIDTH;
        }
        if (wordBuff == "WHEIGHT") {
            lineStream >> conf::WHEIGHT;
        }
        if (wordBuff == "CZ") {
            lineStream >> conf::CZ;
        }
        if (wordBuff == "NUMBOTS") {
            lineStream >> conf::NUMBOTS;
        }
        if (wordBuff == "BOTRADIUS") {
            lineStream >> conf::BOTRADIUS;
        }
        if (wordBuff == "BOTSPEED") {
            lineStream >> conf::BOTSPEED;
        }
        if (wordBuff == "SPIKESPEED") {
            lineStream >> conf::SPIKESPEED;
        }
        if (wordBuff == "SPIKEMULT") {
            lineStream >> conf::SPIKEMULT;
        }
        if (wordBuff == "DAMAGE_LIMIT") {
            lineStream >> conf::DAMAGE_LIMIT;
        }
        if (wordBuff == "DAMAGE_GRADIENT") {
            lineStream >> conf::DAMAGE_GRADIENT;
        }
        if (wordBuff == "BABIES") {
            lineStream >> conf::BABIES;
        }
        if (wordBuff == "BOOSTSIZEMULT") {
            lineStream >> conf::BOOSTSIZEMULT;
        }
        if (wordBuff == "REPRATEH") {
            lineStream >> conf::REPRATEH;
        }
        if (wordBuff == "REPRATEC") {
            lineStream >> conf::REPRATEC;
        }
        if (wordBuff == "MATING_RADIUS") {
            lineStream >> conf::MATING_RADIUS;
        }
        if (wordBuff == "MATING_COMPATIBILITY_TRESHOLD") {
            lineStream >> conf::MATING_COMPATIBILITY_TRESHOLD;
        }
        if (wordBuff == "BODY_COMPAT_MULT") {
            lineStream >> conf::BODY_COMPAT_MULT;
        }
        if (wordBuff == "BRAIN_COMPAT_MULT") {
            lineStream >> conf::BRAIN_COMPAT_MULT;
        }
        if (wordBuff == "MATING_BABIES") {
            lineStream >> conf::MATING_BABIES;
        }
        if (wordBuff == "DIST") {
            lineStream >> conf::DIST;
        }
        if (wordBuff == "MUTRATE1_INIT_LOW") {
            lineStream >> conf::MUTRATE1_INIT_LOW;
        }
        if (wordBuff == "MUTRATE1_INIT_HIGH") {
            lineStream >> conf::MUTRATE1_INIT_HIGH;
        }
        if (wordBuff == "MUTRATE2_INIT_LOW") {
            lineStream >> conf::MUTRATE2_INIT_LOW;
        }
        if (wordBuff == "MUTRATE2_INIT_HIGH") {
            lineStream >> conf::MUTRATE2_INIT_HIGH;
        }
        if (wordBuff == "METAMUTRATE1") {
            lineStream >> conf::METAMUTRATE1;
        }
        if (wordBuff == "METAMUTRATE2") {
            lineStream >> conf::METAMUTRATE2;
        }
        if (wordBuff == "FOODINTAKE") {
            lineStream >> conf::FOODINTAKE;
        }
        if (wordBuff == "FOODWASTE") {
            lineStream >> conf::FOODWASTE;
        }
        if (wordBuff == "FOODMAX") {
            lineStream >> conf::FOODMAX;
        }
        if (wordBuff == "FOODADDFREQ") {
            lineStream >> conf::FOODADDFREQ;
        }
        if (wordBuff == "FOODTRANSFER") {
            lineStream >> conf::FOODTRANSFER;
        }
        if (wordBuff == "FOOD_SHARING_DISTANCE") {
            lineStream >> conf::FOOD_SHARING_DISTANCE;
        }
        if (wordBuff == "TEMPERATURE_DISCOMFORT") {
            lineStream >> conf::TEMPERATURE_DISCOMFORT;
        }
        if (wordBuff == "FOOD_DISTRIBUTION_RADIUS") {
            lineStream >> conf::FOOD_DISTRIBUTION_RADIUS;
        }
        if (wordBuff == "REPMULT") {
            lineStream >> conf::REPMULT;
        }
    }
    inFile.close();
}

void conf::save_to_file(std::ofstream &outFile)
{


    outFile << "confBegin" << std::endl;

    outFile << WIDTH<< " ";  //width and height of simulation
    outFile << HEIGHT<< " ";
    outFile << WWIDTH<< " ";  //window width and height
    outFile << WHEIGHT<< " ";

    outFile << CZ<< " "; //cell size in pixels, for food squares. Should divide well into Width Height

    outFile << NUMBOTS<< " "; //initially, and minimally
    outFile << BOTRADIUS<< " "; //for drawing
    outFile << BOTSPEED<< " ";
    outFile << SPIKESPEED<< " "; //how quickly can attack spike go up?
    outFile << SPIKEMULT<< " "; //essentially the strength of every spike impact
    outFile << DAMAGE_LIMIT << " ";
    outFile << DAMAGE_GRADIENT << " ";
    outFile << BABIES<< " "; //number of babies per agent when they reproduce
    outFile << BOOSTSIZEMULT<< " "; //how much boost do agents get? when boost neuron is on
    outFile << REPRATEH<< " "; //reproduction rate for herbivors
    outFile << REPRATEC<< " "; //reproduction rate for carnivors
    outFile << MATING_RADIUS<< " ";
    outFile << MATING_COMPATIBILITY_TRESHOLD<< " ";
    outFile << BODY_COMPAT_MULT << " ";
    outFile << BRAIN_COMPAT_MULT << " ";
    outFile << MATING_BABIES<< " ";

    outFile << DIST<< " ";		//how far can the eyes see on each bot?
    outFile << METAMUTRATE1<< " "; //what is the change in MUTRATE1 and 2 on reproduction? lol
    outFile << METAMUTRATE2<< " ";
    outFile << MUTRATE1_INIT_LOW<< " ";
    outFile << MUTRATE1_INIT_HIGH<< " ";
    outFile << MUTRATE2_INIT_LOW<< " ";
    outFile << MUTRATE2_INIT_HIGH<< " ";


    outFile << FOODINTAKE<< " "; //how much does every agent consume?
    outFile << FOODWASTE<< " "; //how much food disapears if agent eats?
    outFile << FOODMAX<< " "; //how much food per cell can there be at max?
    outFile << FOODADDFREQ<< " "; //how often does random square get to full food?

    outFile << FOODTRANSFER<< " "; //how much is transfered between two agents trading food? per iteration
    outFile << FOOD_SHARING_DISTANCE<< " "; //how far away is food shared between bots?

    outFile << TEMPERATURE_DISCOMFORT<< " "; //how quickly does health drain in nonpreferred temperatures (0= disabled. 0.005 is decent value)

    outFile << FOOD_DISTRIBUTION_RADIUS<< " "; //when bot is killed, how far is its body distributed?

    outFile << REPMULT<< std::endl; //when a body of dead animal is distributed, how much of it goes toward increasing birth counter for surrounding bots?

    outFile << "confEnd" << std::endl;
}

void conf::load_from_save(std::ifstream &inFile)
{
    std::string wordBuff;

    inFile >> wordBuff;

    if (wordBuff != "confBegin")
        throw std::runtime_error("bad format : confBegin");

    inFile >> WIDTH;  //width and height of simulation
    inFile >> HEIGHT;
    inFile >> WWIDTH;  //window width and height
    inFile >> WHEIGHT;

    inFile >> CZ; //cell size in pixels, for food squares. Should divide well into Width Height

    inFile >> NUMBOTS; //initially, and minimally
    inFile >> BOTRADIUS; //for drawing
    inFile >> BOTSPEED;
    inFile >> SPIKESPEED; //how quickly can attack spike go up?
    inFile >> SPIKEMULT; //essentially the strength of every spike impact
    inFile >> DAMAGE_LIMIT;
    inFile >> DAMAGE_GRADIENT;
    inFile >> BABIES; //number of babies per agent when they reproduce
    inFile >> BOOSTSIZEMULT; //how much boost do agents get? when boost neuron is on
    inFile >> REPRATEH; //reproduction rate for herbivors
    inFile >> REPRATEC; //reproduction rate for carnivors
    inFile >> MATING_RADIUS;
    inFile >> MATING_COMPATIBILITY_TRESHOLD;
    inFile >> BODY_COMPAT_MULT;
    inFile >> BRAIN_COMPAT_MULT;
    inFile >> MATING_BABIES;

    inFile >> DIST;		//how far can the eyes see on each bot?
    inFile >> METAMUTRATE1; //what is the change in MUTRATE1 and 2 on reproduction? lol
    inFile >> METAMUTRATE2;
    inFile >> MUTRATE1_INIT_LOW;
    inFile >> MUTRATE1_INIT_HIGH;
    inFile >> MUTRATE2_INIT_LOW;
    inFile >> MUTRATE2_INIT_HIGH;

    inFile >> FOODINTAKE; //how much does every agent consume?
    inFile >> FOODWASTE; //how much food disapears if agent eats?
    inFile >> FOODMAX; //how much food per cell can there be at max?
    inFile >> FOODADDFREQ; //how often does random square get to full food?

    inFile >> FOODTRANSFER; //how much is transfered between two agents trading food? per iteration
    inFile >> FOOD_SHARING_DISTANCE; //how far away is food shared between bots?

    inFile >> TEMPERATURE_DISCOMFORT; //how quickly does health drain in nonpreferred temperatures (0= disabled. 0.005 is decent value)

    inFile >> FOOD_DISTRIBUTION_RADIUS; //when bot is killed, how far is its body distributed?

    inFile >> REPMULT; //when a body of dead animal is distributed, how much of it goes toward increasing birth counter for surrounding bots?

    inFile >> wordBuff;

    if (wordBuff != "confEnd")
        throw std::runtime_error("bad format : confEnd");
}
