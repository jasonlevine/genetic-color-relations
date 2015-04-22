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
void colorRelations::setup(int colorTableSize){
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
    
    for (int i = 0; i < colorTableSize; i++) {
        colorTable temp;
        temp.mainColor = i;
        colorT.push_back(temp);
    }

}

//--------------------------------------------------------------
void colorRelations::findRelations(genImg &img){
    //
    int w = img.img.getWidth();
    int h = img.img.getHeight();
    
    //start with the edge cases
//    float t1 = t;
    //top left corner
    ofVec2f tlc = ofVec2f(0, 0);
    int topLeftID = img.getColor(tlc.x, tlc.y);//findColor(img.getColor(tlc.x, tlc.y));
    findNColor(topLeftID, img.getColor(tlc.x + right.x, tlc.y + right.y));
    findNColor(topLeftID, img.getColor(tlc.x + bottomRight.x, tlc.y + bottomRight.y));
    findNColor(topLeftID, img.getColor(tlc.x + bottom.x, tlc.y + bottom.y));
    
    //top right corner
    ofVec2f trc = ofVec2f(w-1, 0);
    int topRightID = img.getColor(trc.x, trc.y);//findColor(img.getColor(trc.x, trc.y));
    findNColor(topRightID, img.getColor(trc.x + left.x, trc.y + left.y));
    findNColor(topRightID, img.getColor(trc.x + bottomLeft.x, trc.y + bottomLeft.y));
    findNColor(topRightID, img.getColor(trc.x + bottom.x, trc.y + bottom.y));
    
    //bottom left corner
    ofVec2f blc = ofVec2f(0, h-1);
    int bottomLeftID = img.getColor(blc.x, blc.y);//findColor(img.getColor(blc.x, blc.y));
    findNColor(bottomLeftID, img.getColor(blc.x + right.x, blc.y + right.y));
    findNColor(bottomLeftID, img.getColor(blc.x + topRight.x, blc.y + topRight.y));
    findNColor(bottomLeftID, img.getColor(blc.x + top.x, blc.y + top.y));
    
    //bottom right corner
    ofVec2f brc = ofVec2f(w-1, h-1);
    int bottomRightID = img.getColor(brc.x, brc.y);//findColor(img.getColor(brc.x, brc.y));
    findNColor(bottomRightID, img.getColor(brc.x + left.x, brc.y + left.y));
    findNColor(bottomRightID, img.getColor(brc.x + topLeft.x, brc.y + topLeft.y));
    findNColor(bottomRightID, img.getColor(brc.x + top.x, brc.y + top.y));
    //    cout << div << endl;
    //    cout << "corners - " << t - t1 << endl;
    
    
//    float t2 = t;
    //top edge
    for (int x = 1; x < w-1; x++){
        int topID = img.getColor(x,0);//findColor(img.getColor(x,0));
        
        for (int n = 3; n < nPos.size(); n++){
            findNColor(topID, img.getColor(x+nPos[n].x, nPos[n].y));
        }
    }
    
    //left edge
    for (int y = 1; y < h-1; y++){
        int leftID = img.getColor(0,y);//findColor(img.getColor(0,y));
        
        for (int n = 1; n < nPos.size(); n++){
            if (n != 3 && n != 5) findNColor(leftID, img.getColor(nPos[n].x, y+nPos[n].y));
        }
    }
    
    //bottom edge
    for (int x = 1; x < w-1; x++){
        int bottomID = img.getColor(x,h-1);//findColor(img.getColor(x,h-1));
        
        for (int n = 0; n < nPos.size()-3; n++){
            findNColor(bottomID, img.getColor(x+nPos[n].x, h-1+nPos[n].y));
        }
    }
    
    //left edge
    for (int y = 1; y < h-1; y++){
        int leftID = img.getColor(w-1,y);//findColor(img.getColor(w-1,y));
        
        for (int n = 0; n < nPos.size(); n++){
            if (n != 2 && n != 4 && n != 7) findNColor(leftID, img.getColor(w-1+nPos[n].x, y+nPos[n].y));
        }
    }
    //    cout << "edges - " << t - t2 << endl;
    
//    float t3 = t;
    //everything else
    for (int y = 1; y < h-1; y++){
        for (int x = 1; x < w-1; x++){
            int colID = img.getColor(x, y);//findColor(img.getColor(x, y));
            
            // for every neighbor
            for (int n = 0; n < nPos.size(); n++){
                findNColor(colID, img.getColor(x+nPos[n].x, y+nPos[n].y));
            }
        }
    }
    //    cout << "the rest - " << t - t3 << endl;
}


////--------------------------------------------------------------
//int colorRelations::findColor(int col) {
//    bool colFound = false;
//    int colID = -1;
//    
//    //check if the color is already in the color table
//    for (int i = 0; i < colorT.size(); i++){
//        if (colorT[i].mainColor == col){
//            colFound = true;
//            colID = i;
//            break;
//        }
//    }
//    
//    // if not, add it to the table
//    if (!colFound){
//        //        if (count) imgColors.push_back(col);
//        
//        colorTable newNc;
//        newNc.mainColor = col;
//        colorT.push_back(newNc);
//        colID = colorT.size()-1;
//    }
//    
//    return colID;
//}

//--------------------------------------------------------------
void colorRelations::findNColor(int colID, int nCol){
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

#pragma mark fitness

//--------------------------------------------------------------
void colorRelations::calcFitness(genImg &img){
    int w = img.img.getWidth();
    int h = img.img.getHeight();
    
    img.fitness = 0.0;
    img.dna.geneFitness.assign(w*h, 0.0);
    
    
    //top left corner
    ofVec2f tlc = ofVec2f(0, 0);
    calcNFitness(img, tlc, right, 0.33333);
    calcNFitness(img, tlc, bottomRight, 0.33333);
    calcNFitness(img, tlc, bottom, 0.33333);
    
    //top right corner
    ofVec2f trc = ofVec2f(w-1, 0);
    calcNFitness(img, trc, left, 0.33333);
    calcNFitness(img, trc, bottomLeft, 0.33333);
    calcNFitness(img, trc, bottom, 0.33333);

    
    //bottom left corner
    ofVec2f blc = ofVec2f(0, h-1);
    calcNFitness(img, blc, right, 0.33333);
    calcNFitness(img, blc, topRight, 0.33333);
    calcNFitness(img, blc, top, 0.33333);
    
    //bottom right corner
    ofVec2f brc = ofVec2f(w-1, h-1);
    calcNFitness(img, brc, left, 0.33333);
    calcNFitness(img, brc, topLeft, 0.33333);
    calcNFitness(img, brc, top, 0.33333);
    
    //    float t2 = t;
    //top edge
    for (int x = 1; x < w-1; x++){
        ofVec2f topEdge(x, 0);
        calcNFitness(img, topEdge, left, 0.2);
        calcNFitness(img, topEdge, bottomLeft, 0.2);
        calcNFitness(img, topEdge, bottom, 0.2);
        calcNFitness(img, topEdge, bottomRight, 0.2);
        calcNFitness(img, topEdge, right, 0.2);
    }
    
    //left edge
    for (int y = 1; y < h-1; y++){
        ofVec2f leftEdge(0, y);
        calcNFitness(img, leftEdge, bottom, 0.2);
        calcNFitness(img, leftEdge, bottomRight, 0.2);
        calcNFitness(img, leftEdge, right, 0.2);
        calcNFitness(img, leftEdge, topRight, 0.2);
        calcNFitness(img, leftEdge, top, 0.2);
    }
    
    //bottom edge
    for (int x = 1; x < w-1; x++){
        ofVec2f bottomEdge(x, h-1);
        calcNFitness(img, bottomEdge, right, 0.2);
        calcNFitness(img, bottomEdge, topRight, 0.2);
        calcNFitness(img, bottomEdge, top, 0.2);
        calcNFitness(img, bottomEdge, topLeft, 0.2);
        calcNFitness(img, bottomEdge, left, 0.2);
    }
    
    //right edge
    for (int y = 1; y < h-1; y++){
        ofVec2f rightEdge(w-1, y);
        calcNFitness(img, rightEdge, top, 0.2);
        calcNFitness(img, rightEdge, topLeft, 0.2);
        calcNFitness(img, rightEdge, left, 0.2);
        calcNFitness(img, rightEdge, bottomLeft, 0.2);
        calcNFitness(img, rightEdge, bottom, 0.2);
    }
    
    for (int y = 1; y < h-1; y++){
        for (int x = 1; x < w-1; x++){
            for (int n = 0; n < nPos.size(); n++){
                calcNFitness(img, ofVec2f(x,y), nPos[n], 0.125);
            }
        }
    }
    
    //return fitness;
}

void colorRelations::calcNFitness(genImg &img, ofVec2f pos, ofVec2f npos, float inc){
    int colID = img.getColor(pos.x, pos.y);
    int nCol = img.getColor(pos.x + npos.x, pos.y + npos.y);
    int i = pos.x + pos.y * img.img.width;
    
    for (int nc = 0; nc < colorT[colID].neighborColors.size(); nc++){
        if (colorT[colID].neighborColors[nc] == nCol){
            img.dna.geneFitness[i] += inc;
            img.fitness += inc;
            break;
        }
    }
}
