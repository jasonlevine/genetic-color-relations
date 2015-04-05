//
//  Track.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//
#include "DNA.h"



class genImg {
public:
    genImg(ofImage &src, int scale, int nColors);
    void expressGenes(vector<ofColor> &colors);
    void draw(int x, int y){ img.draw(x,y); }
    float getFitness();
    void setFitness(float fit) { fitness = fit; }
    DNA getDNA();
    
    DNA dna;
    ofImage img;
    
//private:
    
    float fitness;

};
