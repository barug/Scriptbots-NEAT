//
// Created by barth on 8/15/18.
//

#include <stdio.h>
#include <iostream>

#include "NEAT/NeatBrain.h"
#include "settings.h"
#include "helpers.h"

using namespace NEAT;
using namespace std;

NEATBrain::NEATBrain()
{
    _gen = new Genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    _gen->mutate_link_weights(1, 1, GAUSSIAN);
    _net = _gen->genesis(0);
}

NEATBrain::NEATBrain(const NEATBrain &other)
{
    _gen = other._gen->duplicate(0);
    _net = other._net;
}

NEATBrain& NEATBrain::operator=(const NEATBrain& other) {
    if( this != &other ) {
        _gen = other._gen->duplicate(0);
        _net = NULL;
    } else {
        cout << "copy of brain failed" << endl;
    }
    return *this;
}

NEATBrain::~NEATBrain() {
    delete _gen;
    if (_net)
        delete _net;
}



void NEATBrain::tick(std::vector<float>& in, std::vector<float>& out)
{
    _net->load_sensors(in);
    _net->activate();
    _net->copy_outputs(out);
}

void NEATBrain::mutate(float MR, float MR2, std::vector<NEAT::Innovation*> &innovations, int &cur_node_id, double &cur_innov_num)
{
    if (randf(0,1)<MR) {
        std::cout << "adding node" << endl;
        _gen->mutate_add_node(innovations, cur_innov_num);
    }

/*
    else if (randf(0,1)<MR) {
        std::cout << "mutating random trait" << endl;
        _gen->mutate_random_trait();
    }
*/

    else if (randf(0, 1) < MR) {
        //Only do other mutations when not doing sturctural mutations
        std::cout << "adding link" << endl;
        // todo : find about meaning of the number of tries parameter
        _gen->mutate_add_link(innovations, cur_innov_num, 5);
    }

    if (_net)
        delete _net;
    _net = _gen->genesis(0);
}

NEATBrain NEATBrain::crossover( const NEATBrain &other )
{

}