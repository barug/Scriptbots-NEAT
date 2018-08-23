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
    _gen = nullptr;
    _net = nullptr;
}

NEATBrain::NEATBrain(const NEATBrain &other)
{
    _gen = other._gen->duplicate(0);
    _net = nullptr;
}

NEATBrain& NEATBrain::operator=(const NEATBrain& other) {
    if( this != &other ) {
        _gen = other._gen->duplicate(0);
        _net = nullptr;
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

void NEATBrain::initiateBasicBrain() {
    _gen = new Genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    _gen->mutate_link_weights(1, 1, GAUSSIAN);
    _net = _gen->genesis(0);
}

NEATBrain *NEATBrain::duplicate() {
    NEATBrain *newBrain = new NEATBrain();
    newBrain->_gen = _gen->duplicate(0);
    return newBrain;
}

void NEATBrain::tick(std::vector<float>& in, std::vector<float>& out)
{
    _net->load_sensors(in);
    _net->activate();
    _net->copy_outputs(out);
}

void NEATBrain::mutate(float MR, float MR2, std::vector<NEAT::Innovation*> &innovations, double &cur_innov_num)
{
    if (randf(0,1)<MR) {
        _gen->mutate_add_node(innovations, cur_innov_num);
    }

    else if (randf(0, 1) < MR) {
        // todo : find about meaning of the number of tries parameter
        _gen->mutate_add_link(innovations, cur_innov_num, 5);
    }

    else {
        //Only do other mutations when not doing structural mutations

        if (randf(0, 1) < MR) {
            _gen->mutate_random_trait();
        }

        else if (randf(0,1) < MR) {
            _gen->mutate_link_trait(1);
        }

        else if (randf(0,1) < MR) {
            _gen->mutate_node_trait(1);
        }

        else if (randf(0,1) < MR) {
            _gen->mutate_toggle_enable(1);
        }
    }

    if (_net)
        delete _net;
}

void NEATBrain::generateNetwork() {
    _net = _gen->genesis(0);
}

double NEATBrain::compatibility(NEATBrain *other) {
    return _gen->compatibility(other->_gen);
}

NEATBrain *NEATBrain::crossover( const NEATBrain *other )
{
    NEATBrain *newBrain = new NEATBrain();
    newBrain->_gen = other->_gen->mate_multipoint(other->_gen, randf(0, 1), 0, randf(0, 1), 0);
    return newBrain;
}