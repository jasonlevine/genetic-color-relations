//
//  population.h
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "ofMain.h"
#include "genImg.h"
#include "ofxXmlSettings.h"


struct colorTable {
    int mainColor;
    vector<int> neighborColors;
    vector<int> neighborCount;
};




class Population {
public:
    
    Population(float m, int num, string imgPath);
    
    //find relations
    void findRelations(genImg &img, vector<colorTable> &colorT);
    int findColor(int col, vector<colorTable> &colorT);
    void findNColor(int colID, int nCol, vector<colorTable> &colorT);
    void generateHeatmap(genImg &img, vector<colorTable> &colorT);
    
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
    vector<colorTable> colorRelations;
    vector<colorTable> fittestColorRelations;
    vector<ofColor> imgColors;
    vector<ofVec2f> nPos;
    
    
    
};
