//
//  DNA.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//
#pragma once
#include "ofMain.h"

class DNA {
public:
    DNA(){}
    DNA(int _num, int _nColors);
    void expressGenes();
    void testFitness();
    DNA crossover(DNA partner, float mutationRate);
//    DNA crossover(DNA partner);
    void mutate(float mutationRate);
    
    
    vector<int> genes;
    int num;
    int nColors;
    float fitness;
    
};


