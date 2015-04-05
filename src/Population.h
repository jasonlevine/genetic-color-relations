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
    void findRelations(genImg &img, vector<colorTable> &colorT, bool count);
    int findColor(int col, vector<colorTable> &colorT, bool count);
    void findNColor(int colID, int nCol, vector<colorTable> &colorT, bool count);
    
    //sort
    static bool sortOnLightness(const ofColor &a, const ofColor &b);
    
    //GUI
    void draw();
    void drawImgScaled(ofImage &img, float x, float y, float scale);
    void drawBreakdown(vector<colorTable> &colorT, float x, float y, float scale);
    
    int rollover(int mouseX, int mouseY);
    //GA
    void calcPerfectScore();
    void calcFitness();
    int findColor(ofImage img, ofVec2f pos);
    int incFitness(ofImage img, int colID, ofVec2f pos, ofVec2f neighbor);
    void selection();
    void reproduction();
    //Getters
    int getGenerations();
    float getMaxFitness();
    float getMinFitness();
    float getAvgFitness();
    
    void saveSession();
    void loadSession();
    
    int numN;
    float perfectScore;
    
private:
    ofxXmlSettings session;
    
    float mutationRate;          // Mutation rate
    vector<genImg> population;   // array to hold the current population
    vector<genImg> matingPool;   // ArrayList which we will use for our "mating pool"
    int generations;             // Number of generations
    
    ofImage srcImg;
    vector<colorTable> colorRelations;
    vector<colorTable> fittestColorRelations;
    vector<ofColor> imgColors;
    vector<ofVec2f> nPos;
    
    
    
};
