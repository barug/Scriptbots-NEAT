#ifndef MLPBRAIN_H
#define MLPBRAIN_H

#include "settings.h"
#include "helpers.h"

#include <vector>
#include <stdio.h>

class MLPBox {
public:

    MLPBox();

    std::vector<float> w; //weight of each connecting box
    std::vector<int> id; //id in boxes[] of the connecting box
    std::vector<int> type; //0: regular synapse. 1: change-sensitive synapse
    float kp; //damper
    float gw; //global w
    float bias;

    //state variables
    float target; //target value this node is going toward
    float out; //current output
    float oldout; //output a tick ago
};

/**
 * Damped Weighted Recurrent AND/OR Network
 */
class MLPBrain
{
public:

    std::vector<MLPBox> boxes;

    MLPBrain();
    MLPBrain(const MLPBrain &other);
    virtual MLPBrain& operator=(const MLPBrain& other);

    void tick(std::vector<float>& in, std::vector<float>& out);
    void mutate(float MR, float MR2);
    MLPBrain crossover( const MLPBrain &other );
private:
    void init();
};

#endif
