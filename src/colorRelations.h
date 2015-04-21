//
//  colorRelations.h
//  geneticColorRelations
//
//  Created by Jason Levine on 2015-04-08.
//
//

#pragma once
#include "ofMain.h"
#include "genImg.h"

struct colorTable {
    int mainColor;
    vector<int> neighborColors;
    vector<int> neighborCount;
};

class colorRelations {
public:
    void setup(int colorTableSize);
    void findRelations(genImg &img);
    int findColor(int col);
    void findNColor(int colID, int nCol);
    float calcFitness(genImg &img);
    //void calcNFitness(genImg &img, int i, int colID, int nCol, float &fitness, float inc);
    void calcNFitness(genImg &img, ofVec2f pos, ofVec2f npos, float &fitness, float inc);
    
    vector<ofVec2f> nPos;
    vector<colorTable> colorT;
    
    bool count;
    int numN;
};