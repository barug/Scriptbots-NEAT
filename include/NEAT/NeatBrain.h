//
// Created by barth on 8/15/18.
//

#ifndef NEATBRAIN_H
#define NEATBRAIN_H

#include <vector>
#include <stdio.h>
#include "genome.h"
#include "network.h"


class NEATBrain {
public:
    NEAT::Genome *_gen;
    NEAT::Network *_net;


    NEATBrain();
    NEATBrain(const NEATBrain &other);
    virtual NEATBrain& operator=(const NEATBrain& other);

    ~NEATBrain();
    void tick(std::vector<float>& in, std::vector<float>& out);
    void mutate(float MR, float MR2, std::vector<NEAT::Innovation*> &innovations, int &cur_node_id, double &cur_innov_num);
    NEATBrain crossover( const NEATBrain &other );
};


#endif //NEATBRAIN_H
