//
//  population.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "ofMain.h"
#include "genImg.h"
#include "colorRelations.h"

#include "ofxXmlSettings.h"


class Population {
public:
    
    Population(float m, int num, string imgPath);
    
//    void generateHeatmap(genImg &img, vector<colorTable> &colorT);
    
    //draw routines
    void draw();
    void drawImgScaled(ofImage &img, float x, float y, float scale);
    void drawBreakdown(vector<colorTable> &colorT, float x, float y, float scale);

    //GA
    void calcFitness();
    void selection();
    void reproduction();
    
    //Getters
    int getGenerations();
    float getMaxFitness();
    float getMinFitness();
    float getAvgFitness();
    float getMutationRate() { return mutationRate; }
    
    
    void decayMutationRate() { mutationRate *= 0.999; }
    
    //save
    void saveSession();
    void loadSession();
    
    bool count;
    int numN;
    float perfectScore;
    
private:
    ofxXmlSettings session;
    
    float mutationRate;          // Mutation rate
    vector<genImg> population;   // array to hold the current population
    vector<genImg> matingPool;   // ArrayList which we will use for our "mating pool"
    int generations;             // Number of generations
    
    ofImage srcImg, heatmap;
    
    vector<colorTable> colorTables;
    vector<ofColor> imgColors;
    
    colorRelations CR;
};
