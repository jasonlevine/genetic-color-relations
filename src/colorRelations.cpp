//
//  colorRelations.cpp
//  geneticColorRelations
//
//  Created by Jason Levine on 2015-04-08.
//
//

#include "colorRelations.h"

#define topLeft nPos[0]
#define top nPos[1]
#define topRight nPos[2]
#define left nPos[3]
#define right nPos[4]
#define bottomLeft nPos[5]
#define bottom nPos[6]
#define bottomRight nPos[7]




//--------------------------------------------------------------
void colorRelations::setup(){
    //setup neighbor positions
    nPos.assign(8, ofVec2f(0,0));
    
    nPos[0].set(-1, -1); //top left
    nPos[1].set(0, -1); // top
    nPos[2].set(1, -1); // top right
    nPos[3].set(-1, 0); // left
    nPos[4].set(1, 0); // right
    nPos[5].set(-1, 1); // bottom left
    nPos[6].set(0, 1); // bottom
    nPos[7].set(1, 1); // bottom right
    
    numN = 0;
}

//--------------------------------------------------------------
void colorRelations::findRelations(genImg &img, vector<colorTable> &colorT){
    //
    int w = img.img.getWidth();
    int h = img.img.getHeight();
    
    //start with the edge cases
//    float t1 = t;
    //top left corner
    ofVec2f tlc = ofVec2f(0, 0);
    int topLeftID = findColor(img.getColor(tlc.x, tlc.y), colorT);
    findNColor(topLeftID, img.getColor(tlc.x + right.x, tlc.y + right.y), colorT);
    findNColor(topLeftID, img.getColor(tlc.x + bottomRight.x, tlc.y + bottomRight.y), colorT);
    findNColor(topLeftID, img.getColor(tlc.x + bottom.x, tlc.y + bottom.y), colorT);
    
    //top right corner
    ofVec2f trc = ofVec2f(w-1, 0);
    int topRightID = findColor(img.getColor(trc.x, trc.y), colorT);
    findNColor(topRightID, img.getColor(trc.x + left.x, trc.y + left.y), colorT);
    findNColor(topRightID, img.getColor(trc.x + bottomLeft.x, trc.y + bottomLeft.y), colorT);
    findNColor(topRightID, img.getColor(trc.x + bottom.x, trc.y + bottom.y), colorT);
    
    //bottom left corner
    ofVec2f blc = ofVec2f(0, h-1);
    int bottomLeftID = findColor(img.getColor(blc.x, blc.y), colorT);
    findNColor(bottomLeftID, img.getColor(blc.x + right.x, blc.y + right.y), colorT);
    findNColor(bottomLeftID, img.getColor(blc.x + topRight.x, blc.y + topRight.y), colorT);
    findNColor(bottomLeftID, img.getColor(blc.x + top.x, blc.y + top.y), colorT);
    
    //bottom right corner
    ofVec2f brc = ofVec2f(w-1, h-1);
    int bottomRightID = findColor(img.getColor(brc.x, brc.y), colorT);
    findNColor(bottomRightID, img.getColor(brc.x + left.x, brc.y + left.y), colorT);
    findNColor(bottomRightID, img.getColor(brc.x + topLeft.x, brc.y + topLeft.y), colorT);
    findNColor(bottomRightID, img.getColor(brc.x + top.x, brc.y + top.y), colorT);
    //    cout << div << endl;
    //    cout << "corners - " << t - t1 << endl;
    
    
//    float t2 = t;
    //top edge
    for (int x = 1; x < w-1; x++){
        int topID = findColor(img.getColor(x,0), colorT);
        
        for (int n = 3; n < nPos.size(); n++){
            findNColor(topID, img.getColor(x+nPos[n].x, nPos[n].y), colorT);
        }
    }
    
    //left edge
    for (int y = 1; y < h-1; y++){
        int leftID = findColor(img.getColor(0,y), colorT);
        
        for (int n = 1; n < nPos.size(); n++){
            if (n != 3 && n != 5) findNColor(leftID, img.getColor(nPos[n].x, y+nPos[n].y), colorT);
        }
    }
    
    //bottom edge
    for (int x = 1; x < w-1; x++){
        int bottomID = findColor(img.getColor(x,h-1), colorT);
        
        for (int n = 0; n < nPos.size()-3; n++){
            findNColor(bottomID, img.getColor(x+nPos[n].x, h-1+nPos[n].y), colorT);
        }
    }
    
    //left edge
    for (int y = 1; y < h-1; y++){
        int leftID = findColor(img.getColor(w-1,y), colorT);
        
        for (int n = 0; n < nPos.size(); n++){
            if (n != 2 && n != 4 && n != 7) findNColor(leftID, img.getColor(w-1+nPos[n].x, y+nPos[n].y), colorT);
        }
    }
    //    cout << "edges - " << t - t2 << endl;
    
//    float t3 = t;
    //everything else
    for (int y = 1; y < h-1; y++){
        for (int x = 1; x < w-1; x++){
            int colID = findColor(img.getColor(x, y), colorT);
            
            // for every neighbor
            for (int n = 0; n < nPos.size(); n++){
                findNColor(colID, img.getColor(x+nPos[n].x, y+nPos[n].y), colorT);
            }
        }
    }
    //    cout << "the rest - " << t - t3 << endl;
}


//--------------------------------------------------------------
int colorRelations::findColor(int col, vector<colorTable> &colorT) {
    bool colFound = false;
    int colID = -1;
    
    //check if the color is already in the color table
    for (int i = 0; i < colorT.size(); i++){
        if (colorT[i].mainColor == col){
            colFound = true;
            colID = i;
            break;
        }
    }
    
    // if not, add it to the table
    if (!colFound){
        //        if (count) imgColors.push_back(col);
        
        colorTable newNc;
        newNc.mainColor = col;
        colorT.push_back(newNc);
        colID = colorT.size()-1;
    }
    
    return colID;
}

//--------------------------------------------------------------
void colorRelations::findNColor(int colID, int nCol, vector<colorTable> &colorT){
    bool nColFound = false;
    
    //check if the neigbor is in the neighbor list
    for (int nc = 0; nc < colorT[colID].neighborColors.size(); nc++){
        if (colorT[colID].neighborColors[nc] == nCol){
            nColFound = true;
            colorT[colID].neighborCount[nc]++;
            break;
        }
    }
    
    //if not, add it to the list, and increment the neighbor count
    if (!nColFound){
        colorT[colID].neighborColors.push_back(nCol);
        colorT[colID].neighborCount.push_back(1);
        
        if (count) numN++;
    }
    
}
