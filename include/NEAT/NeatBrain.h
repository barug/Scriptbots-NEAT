//
// Created by barth on 8/15/18.
//

#ifndef NEATBRAIN_H
#define NEATBRAIN_H

#include <vector>
#include <stdio.h>
#include "genome.h"
#include "network.h"

namespace NEAT {
    class NEATBrain {
    public:
        NEAT::Genome *_gen;
        NEAT::Network *_net;


        NEATBrain();

        NEATBrain(std::ifstream &inFile);

        NEATBrain(const NEATBrain &other);

        NEATBrain &operator=(const NEATBrain &other);

        ~NEATBrain();

        void initiateBasicBrain();

        NEATBrain *duplicate();

        void tick(std::vector<float> &in, std::vector<float> &out);

        void mutate(float MR, float MR2, std::vector<NEAT::Innovation *> &innovations, double &cur_innov_num);

        void generateNetwork();

        double compatibility(NEATBrain *other);

        NEATBrain *crossover(const NEATBrain *other);

        void printToFile(std::ofstream &outFile);
    };
}

#endif //NEATBRAIN_H
