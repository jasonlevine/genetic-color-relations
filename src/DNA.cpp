//
//  DNA.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "DNA.h"

DNA::DNA(int _num, int _nColors) {
    num = _num;
    nColors = _nColors;
    for (int i = 0; i < num; i++) {
//        genes.push_back(ofRandomuf());
        genes.push_back(ofRandom(nColors));
    }
}

void DNA::expressGenes() {
    
}

void DNA::testFitness() {
    float score = 0;
}

//DNA DNA::crossover(DNA partner){
//    
//    
//    dispatch_queue_t gcdq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
//    int start = 0;
//    int end = num;
//    DNA *child = new DNA(num, nColors);
//    int midpoint = ofRandom(num);
//    dispatch_apply(end-start, gcdq, ^(size_t blockIdx){
//        int i = start+blockIdx;
//        //    for (int i = 0; i < num; i++) {
//        if (i > midpoint) child->genes[i] = genes[i];
//        else child->genes[i] = partner.genes[i];
//    });
//    
//    return *child;
//}

DNA DNA::crossover(DNA partner, float mutationRate){ //crossover + mutate

    dispatch_queue_t gcdq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    int start = 0;
    int end = num;
    DNA *child = new DNA(num, nColors);
    int midpoint = ofRandom(num);
    dispatch_apply(end-start, gcdq, ^(size_t blockIdx){
        int i = start+blockIdx;

        if (ofRandomuf() < mutationRate) { //the worse the fitness the higher the mutation rate
            genes[i] = ofRandom(nColors);
        }
        else {
            if (i > midpoint) child->genes[i] = genes[i];
            else child->genes[i] = partner.genes[i];
        }
    });
    
    DNA returnChild = *child;
    delete child;
    return returnChild;
    
}

void DNA::mutate(float mutationRate){
    dispatch_queue_t gcdq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    int start = 0;
    int end = num;
    dispatch_apply(end-start, gcdq, ^(size_t blockIdx){
        int i = start+blockIdx;
//    for (int i = 0; i < num; i++) {
        if (ofRandomuf() < mutationRate * (1.0 - geneFitness[i]) + mutationRate) {
            genes[i] = ofRandom(nColors);
        }
    });
}