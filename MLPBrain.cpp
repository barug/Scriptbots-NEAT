#include "MLPBrain.h"
using namespace std;


MLPBox::MLPBox()
{

    w.resize(CONNS,0);
    id.resize(CONNS,0);

    //constructor
    for (int i=0;i<CONNS;i++) {
        w[i]= randf(-1,1);
        id[i]= randi(0,BRAINSIZE);
        if (randf(0,1)<0.2) id[i]= randi(0,INPUTSIZE); //20% of the brain AT LEAST should connect to input.
    }

    kp= randf(0.1,1);
    gw= randf(0,3);
    bias= randf(-3,3);

    out=0;
    target=0;
}

MLPBrain::MLPBrain()
{

    //constructor
    for (int i=0;i<BRAINSIZE;i++) {
        MLPBox a; //make a random box and copy it over
        boxes.push_back(a);

        boxes[i].out= a.out;
        boxes[i].target= a.target;
        boxes[i].kp= a.kp;
        boxes[i].gw= a.gw;
        boxes[i].bias= a.bias;
        for (int j=0;j<CONNS;j++) {
            boxes[i].w[j]= a.w[j];
            boxes[i].id[j]= a.id[j];
            if (i<BRAINSIZE/2) {
                boxes[i].id[j]= randi(0,INPUTSIZE);
            }
        }
    }

    //do other initializations
    init();
}

MLPBrain::MLPBrain(const MLPBrain& other)
{
    boxes = other.boxes;
}

MLPBrain& MLPBrain::operator=(const MLPBrain& other)
{
    if( this != &other )
        boxes = other.boxes;
    return *this;
}


void MLPBrain::init()
{

}

void MLPBrain::tick(vector< float >& in, vector< float >& out)
{
    //do a single tick of the brain

    //take first few boxes and set their out to in[].
    for (int i=0;i<INPUTSIZE;i++) {
        boxes[i].out= in[i];
    }

    //then do a dynamics tick and set all targets
    for (int i=INPUTSIZE;i<BRAINSIZE;i++) {
        MLPBox* abox= &boxes[i];

        float acc=0;
        for (int j=0;j<CONNS;j++) {
            int idx=abox->id[j];
            float val= boxes[idx].out;
            acc= acc + val*abox->w[j];
        }
        acc*= abox->gw;
        acc+= abox->bias;
        
        //put through sigmoid
        acc= 1.0/(1.0+exp(-acc));
        
        abox->target= acc;
    }

    //make all boxes go a bit toward target
    for (int i=INPUTSIZE;i<BRAINSIZE;i++) {
        MLPBox* abox= &boxes[i];
        abox->out =abox->out + (abox->target-abox->out)*abox->kp;
    }

    //finally set out[] to the last few boxes output
    for (int i=0;i<OUTPUTSIZE;i++) {
        out[i]= boxes[BRAINSIZE-1-i].out;
    }
}

void MLPBrain::mutate(float MR, float MR2)
{
    for (int j=0;j<BRAINSIZE;j++) {

        if (randf(0,1)<MR*3) {
            boxes[j].bias+= randn(0, MR2);
//             a2.mutations.push_back("bias jiggled\n");
        }

        if (randf(0,1)<MR*3) {
            boxes[j].kp+= randn(0, MR2);
            if (boxes[j].kp<0.01) boxes[j].kp=0.01;
            if (boxes[j].kp>1) boxes[j].kp=1;
//             a2.mutations.push_back("kp jiggled\n");
        }
        
        if (randf(0,1)<MR*3) {
            boxes[j].gw+= randn(0, MR2);
            if (boxes[j].gw<0) boxes[j].gw=0;
//             a2.mutations.push_back("kp jiggled\n");
        }

        if (randf(0,1)<MR*3) {
            int rc= randi(0, CONNS);
            boxes[j].w[rc]+= randn(0, MR2);
//          a2.mutations.push_back("weight jiggled\n");
        }

        //more unlikely changes here
        if (randf(0,1)<MR) {
            int rc= randi(0, CONNS);
            int ri= randi(0,BRAINSIZE);
            boxes[j].id[rc]= ri;
//             a2.mutations.push_back("connectivity changed\n");
        }
    }
}

MLPBrain MLPBrain::crossover(const MLPBrain& other)
{
    //this could be made faster by returning a pointer
    //instead of returning by value
    MLPBrain newbrain(*this);
    
    for (int i=0;i<newbrain.boxes.size(); i++) {
        if(randf(0,1)<0.5){
            newbrain.boxes[i].bias= this->boxes[i].bias;
            newbrain.boxes[i].gw= this->boxes[i].gw;
            newbrain.boxes[i].kp= this->boxes[i].kp;
            for (int j=0;j<newbrain.boxes[i].id.size();j++) {
                newbrain.boxes[i].id[j] = this->boxes[i].id[j];
                newbrain.boxes[i].w[j] = this->boxes[i].w[j];
            }
        
        } else {
            newbrain.boxes[i].bias= other.boxes[i].bias;
            newbrain.boxes[i].gw= other.boxes[i].gw;
            newbrain.boxes[i].kp= other.boxes[i].kp;
            for (int j=0;j<newbrain.boxes[i].id.size();j++) {
                newbrain.boxes[i].id[j] = other.boxes[i].id[j];
                newbrain.boxes[i].w[j] = other.boxes[i].w[j];
            }
        }
    }
    return newbrain;
}

