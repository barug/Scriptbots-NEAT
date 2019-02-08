#include "Agent.h"

#include "settings.h"
#include "helpers.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "DWRAONBrain.h"
#include "MLPBrain.h"
#include "AssemblyBrain.h"
#include "NEAT/innovation.h"

using namespace std;
using namespace NEAT;

Agent::Agent()
{
    pos= Vector2f(randf(0,conf::WIDTH),randf(0,conf::HEIGHT));
    angle= randf(-M_PI,M_PI);
    health= 1.0+randf(0,0.1);
    age=0;
    spikeLength=0;
    red= 0;
    gre= 0;
    blu= 0;
    w1=0;
    w2=0;
    soundmul=1;
    give=0;
    clockf1= randf(5,100);
    clockf2= randf(5,100);
    boost=false;
    indicator=0;
    gencount=0;
    selectflag=0;
    dfood=0;
    ir=0;
    ig=0;
    ib=0;
    temperature_preference=randf(0,1);
    hybrid= false;
    herbivore= randf(0,1);
    repcounter= herbivore*randf(conf::REPRATEH-0.1,conf::REPRATEH+0.1) + (1-herbivore)*randf(conf::REPRATEC-0.1,conf::REPRATEC+0.1);

    id=0;
    
    smellmod= randf(0.1, 0.5);
    soundmod= randf(0.2, 0.6);
    hearmod= randf(0.7, 1.3);
    eyesensmod= randf(1, 3);
    bloodmod= randf(1, 3);
    
    MUTRATE1= randf(conf::MUTRATE1_INIT_LOW, conf::MUTRATE1_INIT_HIGH);
    MUTRATE2= randf(conf::MUTRATE2_INIT_LOW, conf::MUTRATE2_INIT_HIGH);

    spiked= false;
    
    in.resize(INPUTSIZE, 0);
    out.resize(OUTPUTSIZE, 0);
    
    eyefov.resize(NUMEYES, 0);
    eyedir.resize(NUMEYES, 0);
    for(int i=0;i<NUMEYES;i++) {
        eyefov[i] = randf(0.5, 2);
        eyedir[i] = randf(0, 2*M_PI);
    }
}

Agent::~Agent() {
    if (brain)
        delete brain;
}




void Agent::makeBasicBrain() {
    brain = new NEATBrain();
    brain->initiateBasicBrain();
}

void Agent::printSelf()
{
    printf("Agent age=%i\n", age);
}

void Agent::initEvent(float size, float r, float g, float b)
{
    indicator=size;
    ir=r;
    ig=g;
    ib=b;
}

void Agent::tick()
{
    brain->tick(in, out);
}

double Agent::compatibility(Agent *other) {
    double compatibility = 0;
    double brainCompat = brain->compatibility(other->brain);
    double bodyCompat = 0;

    bodyCompat += abs(this->herbivore - other->herbivore) * 4;
    bodyCompat += abs(this->smellmod - other->smellmod) / 2;
    bodyCompat += abs(this->hearmod- other->hearmod) / 2;
    bodyCompat += abs(this->eyesensmod - other->eyesensmod) / 2;
    bodyCompat += abs(this->bloodmod - other->bloodmod) / 5;

    double eyesCompat = 0;
    for(int i=0;i<NUMEYES;i++) {
        eyesCompat += abs(this->eyefov[i] - other->eyefov[i]) / 2;
        eyesCompat += abs(this->eyedir[i] - other->eyedir[i]) / (2 * M_PI);
    }
    eyesCompat /= 5;
    bodyCompat += eyesCompat;

    compatibility = conf::BRAIN_COMPAT_MULT * brainCompat + conf::BODY_COMPAT_MULT * bodyCompat;
    if (compatibility > conf::MATING_COMPATIBILITY_TRESHOLD) {
    }
    return compatibility;
}

Agent *Agent::reproduce(float MR, float MR2, vector<NEAT::Innovation*> &innovations, double &cur_innov_num)
{
    bool BDEBUG = false;
    if(BDEBUG) printf("New birth---------------\n");
    Agent *a2 = new Agent();

    //spawn the baby somewhere closeby behind agent
    //we want to spawn behind so that agents dont accidentally eat their young right away
    Vector2f fb(conf::BOTRADIUS,0);
    fb.rotate(-a2->angle);
    a2->pos= this->pos + fb + Vector2f(randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2), randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2));
    if (a2->pos.x<0) a2->pos.x= conf::WIDTH+a2->pos.x;
    if (a2->pos.x>=conf::WIDTH) a2->pos.x= a2->pos.x-conf::WIDTH;
    if (a2->pos.y<0) a2->pos.y= conf::HEIGHT+a2->pos.y;
    if (a2->pos.y>=conf::HEIGHT) a2->pos.y= a2->pos.y-conf::HEIGHT;

    a2->gencount= this->gencount+1;
    a2->repcounter= a2->herbivore*randf(conf::REPRATEH-0.1,conf::REPRATEH+0.1) + (1-a2->herbivore)*randf(conf::REPRATEC-0.1,conf::REPRATEC+0.1);

    //noisy attribute passing
    a2->MUTRATE1= this->MUTRATE1;
    a2->MUTRATE2= this->MUTRATE2;
    if (randf(0,1)<0.1) a2->MUTRATE1= randn(this->MUTRATE1, conf::METAMUTRATE1);
    if (randf(0,1)<0.1) a2->MUTRATE2= randn(this->MUTRATE2, conf::METAMUTRATE2);
    if (this->MUTRATE1<0.001) this->MUTRATE1= 0.001;
    if (this->MUTRATE2<0.02) this->MUTRATE2= 0.02;
    a2->herbivore= cap(randn(this->herbivore, 0.03));
    if (randf(0,1)<MR*5) a2->clockf1= randn(a2->clockf1, MR2);
    if (a2->clockf1<2) a2->clockf1= 2;
    if (randf(0,1)<MR*5) a2->clockf2= randn(a2->clockf2, MR2);
    if (a2->clockf2<2) a2->clockf2= 2;
    
    a2->smellmod = this->smellmod;
    a2->soundmod = this->soundmod;
    a2->hearmod = this->hearmod;
    a2->eyesensmod = this->eyesensmod;
    a2->bloodmod = this->bloodmod;
    if(randf(0,1)<MR*5) {float oo = a2->smellmod; a2->smellmod = randn(a2->smellmod, MR2); if(BDEBUG) printf("smell mutated from %f to %f\n", oo, a2->smellmod);}
    if(randf(0,1)<MR*5) {float oo = a2->soundmod; a2->soundmod = randn(a2->soundmod, MR2); if(BDEBUG) printf("sound mutated from %f to %f\n", oo, a2->soundmod);}
    if(randf(0,1)<MR*5) {float oo = a2->hearmod; a2->hearmod = randn(a2->hearmod, MR2); if(BDEBUG) printf("hear mutated from %f to %f\n", oo, a2->hearmod);}
    if(randf(0,1)<MR*5) {float oo = a2->eyesensmod; a2->eyesensmod = randn(a2->eyesensmod, MR2); if(BDEBUG) printf("eyesens mutated from %f to %f\n", oo, a2->eyesensmod);}
    if(randf(0,1)<MR*5) {float oo = a2->bloodmod; a2->bloodmod = randn(a2->bloodmod, MR2); if(BDEBUG) printf("blood mutated from %f to %f\n", oo, a2->bloodmod);}
    
    a2->eyefov = this->eyefov;
    a2->eyedir = this->eyedir;
    for(int i=0;i<NUMEYES;i++){
        if(randf(0,1)<MR*5) a2->eyefov[i] = randn(a2->eyefov[i], MR2);
        if(a2->eyefov[i]<0) a2->eyefov[i] = 0;
        
        if(randf(0,1)<MR*5) a2->eyedir[i] = randn(a2->eyedir[i], MR2);
        if(a2->eyedir[i]<0) a2->eyedir[i] = 0;
        if(a2->eyedir[i]>2*M_PI) a2->eyedir[i] = 2*M_PI;
    }
    
    a2->temperature_preference= cap(randn(this->temperature_preference, 0.005));

    //mutate brain here
    a2->brain = this->brain->duplicate();
    a2->brain->mutate(MR,MR2, innovations, cur_innov_num);
    a2->brain->generateNetwork();

    return a2;
}



Agent *Agent::mate(const Agent* other, vector<NEAT::Innovation*> &innovations, double &cur_innov_num)
{
    bool BDEBUG = false;
    if(BDEBUG) printf("New birth---------------\n");

    //this could be made faster by returning a pointer
    //instead of returning by value
    Agent *anew = new Agent();

    //spawn the baby somewhere closeby behind agent
    //we want to spawn behind so that agents dont accidentally eat their young right away
    Vector2f fb(conf::BOTRADIUS,0);
    fb.rotate(-anew->angle);
    anew->pos= this->pos + fb + Vector2f(randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2), randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2));
    if (anew->pos.x<0) anew->pos.x= conf::WIDTH+anew->pos.x;
    if (anew->pos.x>=conf::WIDTH) anew->pos.x= anew->pos.x-conf::WIDTH;
    if (anew->pos.y<0) anew->pos.y= conf::HEIGHT+anew->pos.y;
    if (anew->pos.y>=conf::HEIGHT) anew->pos.y= anew->pos.y-conf::HEIGHT;

    anew->hybrid=true; //set this non-default flag
    anew->gencount= this->gencount;
    if (other->gencount<anew->gencount) anew->gencount= other->gencount;

    //agent heredity attributes
    anew->clockf1= randf(0,1)<0.5 ? this->clockf1 : other->clockf1;
    anew->clockf2= randf(0,1)<0.5 ? this->clockf2 : other->clockf2;
    anew->herbivore= randf(0,1)<0.5 ? this->herbivore : other->herbivore;
    anew->MUTRATE1= randf(0,1)<0.5 ? this->MUTRATE1 : other->MUTRATE1;
    anew->MUTRATE2= randf(0,1)<0.5 ? this->MUTRATE2 : other->MUTRATE2;

    if (randf(0,1)<0.1) anew->MUTRATE1= randn(anew->MUTRATE1, conf::METAMUTRATE1);
    if (randf(0,1)<0.1) anew->MUTRATE2= randn(anew->MUTRATE2, conf::METAMUTRATE2);
    if (this->MUTRATE1<0.001) anew->MUTRATE1= 0.001;
    if (this->MUTRATE2<0.02) anew->MUTRATE2= 0.02;
    anew->herbivore= cap(randn(anew->herbivore, 0.03));
    if (randf(0,1)<anew->MUTRATE1*5) anew->clockf1= randn(anew->clockf1, anew->MUTRATE1);
    if (anew->clockf1<2) anew->clockf1= 2;
    if (randf(0,1)<anew->MUTRATE1*5) anew->clockf2= randn(anew->clockf2, anew->MUTRATE2);
    if (anew->clockf2<2) anew->clockf2= 2;

    anew->temperature_preference = randf(0,1)<0.5 ? this->temperature_preference : other->temperature_preference;
    anew->temperature_preference= cap(randn(anew->temperature_preference, 0.005));


    anew->smellmod= randf(0,1)<0.5 ? this->smellmod : other->smellmod;
    anew->soundmod= randf(0,1)<0.5 ? this->soundmod : other->soundmod;
    anew->hearmod= randf(0,1)<0.5 ? this->hearmod : other->hearmod;
    anew->eyesensmod= randf(0,1)<0.5 ? this->eyesensmod : other->eyesensmod;
    anew->bloodmod= randf(0,1)<0.5 ? this->bloodmod : other->bloodmod;
    if(randf(0,1)<anew->MUTRATE1*5) {float oo = anew->smellmod; anew->smellmod = randn(anew->smellmod, anew->MUTRATE2); if(BDEBUG) printf("smell mutated from %f to %f\n", oo, anew->smellmod);}
    if(randf(0,1)<anew->MUTRATE1*5) {float oo = anew->soundmod; anew->soundmod = randn(anew->soundmod, anew->MUTRATE2); if(BDEBUG) printf("sound mutated from %f to %f\n", oo, anew->soundmod);}
    if(randf(0,1)<anew->MUTRATE1*5) {float oo = anew->hearmod; anew->hearmod = randn(anew->hearmod, anew->MUTRATE2); if(BDEBUG) printf("hear mutated from %f to %f\n", oo, anew->hearmod);}
    if(randf(0,1)<anew->MUTRATE1*5) {float oo = anew->eyesensmod; anew->eyesensmod = randn(anew->eyesensmod, anew->MUTRATE2); if(BDEBUG) printf("eyesens mutated from %f to %f\n", oo, anew->eyesensmod);}

    if(randf(0,1)<anew->MUTRATE1*5) {float oo = anew->bloodmod; anew->bloodmod = randn(anew->bloodmod, anew->MUTRATE2); if(BDEBUG) printf("blood mutated from %f to %f\n", oo, anew->bloodmod); }
    
    anew->eyefov= randf(0,1)<0.5 ? this->eyefov : other->eyefov;
    anew->eyedir= randf(0,1)<0.5 ? this->eyedir : other->eyedir;

    for(int i=0;i<NUMEYES;i++){
        if(randf(0,1)<anew->MUTRATE1*5) anew->eyefov[i] = randn(anew->eyefov[i], anew->MUTRATE2);
        if(anew->eyefov[i]<0) anew->eyefov[i] = 0;

        if(randf(0,1)<anew->MUTRATE1*5) anew->eyedir[i] = randn(anew->eyedir[i], anew->MUTRATE2);
        if(anew->eyedir[i]<0) anew->eyedir[i] = 0;
        if(anew->eyedir[i]>2*M_PI) anew->eyedir[i] = 2*M_PI;
    }

    anew->brain = this->brain->crossover(other->brain);
    anew->brain->mutate(anew->MUTRATE1, anew->MUTRATE2, innovations, cur_innov_num);
    anew->brain->generateNetwork();

    return anew;
}

void Agent::removeFromSpecies()
{
    species->removeAgent(this);
}

void Agent::printToFile(std::ofstream &outFile)
{
    outFile << "AgentBegin" << std::endl;
    outFile << pos.x << " ";
    outFile << pos.y << " ";
    outFile << health << " ";
    outFile << angle << " ";

    outFile << red << " ";
    outFile << gre << " ";
    outFile << blu << " ";

    outFile << w1 << " "; //wheel speeds
    outFile << w2 << " ";
    outFile << boost << " ";

    outFile << spikeLength << " ";
    outFile << age << " ";

    outFile << spiked << " ";

    for (std::vector<float>::iterator it = in.begin(); it != in.end(); ++it) {
        outFile << *it << " ";
    }
    for (std::vector<float>::iterator it = out.begin(); it != out.end(); ++it) {
        outFile << *it << " ";
    }

    outFile << repcounter << " ";
    outFile << gencount << " "; //generation counter
    outFile << hybrid << " "; //is this agent result of crossover?
    outFile << clockf1 << " ";
    outFile << clockf2 << " "; //the frequencies of the two clocks of this bot
    outFile << soundmul << " "; //sound multiplier of this bot. It can scream, or be very sneaky. This is actually always set to output 8

    //variables for drawing purposes
    outFile << indicator << " ";
    outFile << ir << " ";
    outFile << ig << " ";
    outFile << ib << " ";
    outFile << selectflag << " ";
    outFile << dfood << " ";

    outFile << give << " ";

    outFile << id << " ";

    //inhereted stuff
    outFile << herbivore << " "; //is this agent a herbivore? between 0 and 1
    outFile << MUTRATE1 << " "; //how often do mutations occur?
    outFile << MUTRATE2 << " "; //how significant are they?
    outFile << temperature_preference << " "; //what temperature does this agent like? [0 to 1]

    outFile << smellmod << " ";
    outFile << soundmod << " ";
    outFile << hearmod << " ";
    outFile << eyesensmod << " ";
    outFile << bloodmod << " ";

    for (std::vector<float>::iterator it = eyefov.begin(); it != eyefov.end(); ++it) {
        outFile << *it << " ";
    }
    for (std::vector<float>::iterator it = eyedir.begin(); it != eyedir.end(); ++it) {
        outFile << *it << " ";
    }
    brain->printToFile(outFile);
    outFile << "AgentEnd" << std::endl;
}

Agent::Agent(std::ifstream &inFile)
{
    std::string wordBuff;

    inFile >> wordBuff;
    if (wordBuff != "AgentBegin")
        throw std::runtime_error("bad format : AgentBegin");

    float x, y;
    inFile >> x;
    inFile >> y;
    pos = Vector2f(x, y);

    inFile >>  health;
    inFile >>  angle;

    inFile >>  red;
    inFile >>  gre;
    inFile >>  blu;

    inFile >>  w1; //wheel speeds
    inFile >>  w2;
    inFile >>  boost;

    inFile >>  spikeLength;
    inFile >>  age;

    inFile >>  spiked;

    float val;
    in.resize(INPUTSIZE, 0);
    out.resize(OUTPUTSIZE, 0);
    for (int i = 0; i < INPUTSIZE; ++i) {
        inFile >> val;
        in[i] = val;
    }
    for (int i = 0; i < OUTPUTSIZE; ++i) {
        inFile >> val;
        out[i] = val;
    }

    inFile >> repcounter;
    inFile >> gencount; //generation counter
    inFile >> hybrid; //is this agent result of crossover?
    inFile >> clockf1;
    inFile >> clockf2; //the frequencies of the two clocks of this bot
    inFile >> soundmul; //sound multiplier of this bot. It can scream, or be very sneaky. This is actually always set to output 8

    //variables for drawing purposes
    inFile >> indicator;
    inFile >> ir;
    inFile >> ig;
    inFile >> ib;
    inFile >> selectflag;
    inFile >> dfood;

    inFile >> give;

    inFile >> id;

    //inhereted stuff
    inFile >> herbivore; //is this agent a herbivore? between 0 and 1
    inFile >> MUTRATE1; //how often do mutations occur?
    inFile >> MUTRATE2; //how significant are they?
    inFile >> temperature_preference; //what temperature does this agent like? [0 to 1]

    inFile >> smellmod;
    inFile >> soundmod;
    inFile >> hearmod;
    inFile >> eyesensmod;
    inFile >> bloodmod;

    eyefov.resize(NUMEYES, 0);
    eyedir.resize(NUMEYES, 0);
    for (int i = 0; i < NUMEYES; ++i) {
        inFile >> val;
        eyefov[i] = val;
    }
    for (int i = 0; i < NUMEYES; ++i) {
        inFile >> val;
        eyedir[i] = val;
    }

    brain = new NEATBrain(inFile);
    inFile >> wordBuff;
    if (wordBuff != "AgentEnd")
        throw std::runtime_error("bad format : AgentEnd");
}