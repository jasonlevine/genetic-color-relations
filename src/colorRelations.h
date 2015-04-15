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
    void setup();
    void findRelations(genImg &img, vector<colorTable> &colorT);
    int findColor(int col, vector<colorTable> &colorT);
    void findNColor(int colID, int nCol, vector<colorTable> &colorT);
    
    vector<ofVec2f> nPos;
    
    bool count;
    
    int numN;
};