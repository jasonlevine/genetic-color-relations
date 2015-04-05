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

DNA DNA::crossover(DNA partner){
    DNA child(num, nColors);
    int midpoint = ofRandom(num);
    
    for (int i = 0; i < num; i++) {
        if (i > midpoint) child.genes[i] = genes[i];
        else child.genes[i] = partner.genes[i];
    }
    
    return child;
}

void DNA::mutate(float mutationRate){
    for (int i = 0; i < num; i++) {
        if (ofRandomuf() < mutationRate) {
            genes[i] = ofRandom(nColors);
            if (genes[i] >= nColors || genes[i] < 0) {
                cout << "gotcha!!!" << endl;
            }
        }
    }
}