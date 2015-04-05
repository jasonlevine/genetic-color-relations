//
//  population.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-28.
//
//

#include "Population.h"
//#include "../osd/gcdKernel.h"
//#include "../osd/cpuKernel.h"

#define topLeft nPos[0]
#define top nPos[1]
#define topRight nPos[2]
#define left nPos[3]
#define right nPos[4]
#define bottomLeft nPos[5]
#define bottom nPos[6]
#define bottomRight nPos[7]

#define t ofGetElapsedTimef()
#define div "--------------------"


//--------------------------------------------------------------
bool Population::sortOnLightness(const ofColor &a, const ofColor &b){
    return a.getLightness() < b.getLightness();
}
   
// Create the population
//--------------------------------------------------------------
Population::Population(float m, int num, string imgPath) {
    ofSeedRandom();
    mutationRate = m;
    generations = 0;
    numN = 0;
    
    
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
    
    //load image
    srcImg.loadImage(imgPath);
    
    //set up list of all unique colors
    for (int x = 0; x < srcImg.getWidth(); x++) {
        for (int y = 0; y < srcImg.getHeight(); y++) {
            ofColor col = srcImg.getColor(x,y);
            bool colFound = false;
            
            for (int c = 0; c < imgColors.size(); c++){
                if (imgColors[c] == col){
                    colFound = true;
                    break;
                }
            }
            
            if (!colFound){
                imgColors.push_back(col);
            }
        }
    }
    
    
//  make a genImg that has the DNA to express the source image
    //todo: make this dna and then pass the dna to a genimg afterwards
    genImg srcDNA(srcImg, 1, imgColors.size());
    
    for (int g = 0; g < srcDNA.dna.genes.size(); g++){
        int x = g % (int)srcImg.getWidth();
        int y = g / (int)srcImg.getWidth();
        
        ofColor col = srcImg.getColor(x,y);
        
        for (int c = 0; c < imgColors.size(); c++){
            if (imgColors[c] == col) {
                srcDNA.dna.genes[g] = c;
                break;
            }
        }
    }
    
    //find the relations of the course image
    count = true;
    findRelations(srcDNA, colorRelations);
    count = false;
    perfectScore = numN;//pow(2, (float)numN);
    
    for (int i = 0; i < num; i++) {
        population.push_back(genImg(srcImg, 2, imgColors.size()));
        //population[i].expressGenes(imgColors);
    }

}


void Population::draw() {
//    srcImg.draw(0,150, 300, 300);
//    ofSetColor(255);
    float spacing = 30;
    float scale = 10;
//    //draw population
    //float scale = ofGetWidth() / ((srcImg.width * 2) * population.size() * 0.5);
    for (int i = 0; i < population.size()*0.5; i++) {
        //population[i].img.draw(i * (population[i].img.width + spacing) + spacing + 10, 30);
        //population[i + population.size()*0.5].img.draw(i * (population[i].img.width + spacing) + spacing + 10, 65 + srcImg.width * 2);
        drawImgScaled(population[i].img, i * ((srcImg.width * 2 + 5) * scale), 30, scale);
        drawImgScaled(population[i + population.size()*0.5].img, i * ((srcImg.width * 2 + 5) * scale), 150, scale);
//        ofDrawBitmapString(ofToString(population[i].fitness), i * ((srcImg.width + 2) * scale), srcImg.getHeight()*2 + 100);
    }
    
    //draw source image
//    drawImgScaled(srcImg,25, 150, 40);
//    drawBreakdown(colorRelations, 350, 150, 25);
    
//    drawImgScaled(srcImg,10,400, 20);
//    drawImgScaled(evoImg,0, 15, 20);
    
    srcImg.draw(100, 15);
    
    //draw fittest image
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness == getMaxFitness()){
            population[i].img.draw(300 + 150 * 2, 15);
//            drawImgScaled(population[i].img,400, 10, 20);
//            for (int x = 0; x < 5; x++) {
//                for (int y = 0; y < 5; y++) {
//                    float x1, y1, w, h;
//                    if (x%2 == 0) {
//                        x1 = x;
//                        w = population[i].img.width;
//                    }
//                    else {
//                        x1 = x+1;
//                        w = -population[i].img.width;
//                    }
//                    if (y%2 == 0) {
//                        y1 = y;
//                        h = population[i].img.height;
//                    }
//                    else {
//                        y1 = y+1;
//                        h = -population[i].img.height;
//                    }
//
//                    
//                    population[i].img.draw(x1*64*2, y1*48*2, w, h);
//                }
//            }
//            population[i].img.draw(0, 0);
//            population[i].img.draw(0, 0);
//            population[i].img.draw(0, 0);
//            fittestColorRelations.clear();
//            for (int c = 0; c < colorRelations.size(); c++){
//                colorTable newNc;
//                newNc.mainColor = colorRelations[c].mainColor;
//                fittestColorRelations.push_back(newNc);
//            }
//            findRelations(population[i].img, fittestColorRelations, false);
//            drawBreakdown(fittestColorRelations, 350, 500, 25);
            break;
        }
    }


}

//--------------------------------------------------------------
void Population::drawImgScaled(ofImage &img, float x, float y, float scale){
    for (int i = 0; i < img.width; i++){
        for (int j = 0; j < img.height; j++){
            ofSetColor(img.getColor(i, j));
            ofRect(x + i * scale, y + j * scale, scale, scale);
        }
    }
}

//--------------------------------------------------------------
void Population::drawBreakdown(vector<colorTable> &colorT, float x, float y, float scale){
    
    for (int c = 0; c < colorT.size(); c++){
//        ofSetColor(0);
//        ofDrawBitmapString(ofToString((int)colorT[c].correct), x + c * scale * 2, y - 20);
//        ofSetColor(255, 0, 0);
//        ofDrawBitmapString(ofToString((int)colorT[c].incorrect), x + c * scale * 2, y - 10);
        
        ofSetColor(colorT[c].mainColor);
        ofRect(x + c * scale * 2, y, scale, scale);
        
        for (int nc = 0; nc < colorT[c].neighborColors.size(); nc++){
            ofSetColor(colorT[c].neighborColors[nc]);
            ofRect(x + c * scale * 2, y + scale * 2 + nc * scale * 1.5, scale, scale);
            
            ofSetColor(0);
            ofDrawBitmapString(ofToString(colorT[c].neighborCount[nc]), x + c * scale * 2 + scale, y + scale * 2 + nc * scale * 1.5 + scale * 0.5);
        }
    }
    
}


// Generate a mating pool
void Population::selection() {
    // Clear the ArrayList
    matingPool.clear();
    
    // Calculate total fitness of whole population
    float maxFitness = getMaxFitness();
    float minFitness = getMinFitness();

    if (maxFitness == minFitness){
        matingPool = population;
    }
    else {
        for (int i = 0; i < population.size(); i++) {
            float popFit = population[i].getFitness();
//            popFit = pow(2, popFit);
            float fitnessNormal = ofMap(popFit, minFitness, maxFitness, 0, 1);
            float f = fitnessNormal;
//            cout << "fitness normal = " << fitnessNormal << endl;
            fitnessNormal = f*f*f*f*f;
            int n = (int) (fitnessNormal * 100);  // Arbitrary multiplier
//            cout << "n = " << n << endl;
            for (int j = 0; j < n; j++) {
                matingPool.push_back(population[i]);
            }
        }
        
    }
}

// Making the next generation
void Population::reproduction() {
    // Refill the population with children from the mating pool
    for (int i = 0; i < population.size(); i++) {
        // Sping the wheel of fortune to pick two parents
        int m = int(ofRandom(matingPool.size()));
        int d = int(ofRandom(matingPool.size()));
        // Pick two parents
        genImg mom = matingPool[m];
        genImg dad = matingPool[d];
        // Get their genes
        DNA momgenes = mom.getDNA();
        DNA dadgenes = dad.getDNA();
                // Mate their genes
        DNA child = momgenes.crossover(dadgenes);
        // Mutate their genes
        child.mutate(mutationRate);
        // Fill the new population with the new child
        population[i].dna = child;
        population[i].expressGenes(imgColors);
    }
    generations++;
}

void Population::calcFitness(){
    dispatch_queue_t gcdq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    int start = 0;
    int end = population.size();
    dispatch_apply(end-start, gcdq, ^(size_t blockIdx){
        int i = start+blockIdx;
        
        vector<colorTable> genImgColors;
        for (int c = 0; c < colorRelations.size(); c++){
            colorTable newNc;
            newNc.mainColor = colorRelations[c].mainColor;
            genImgColors.push_back(newNc);
        }
        
        findRelations(population[i], genImgColors);
        
        population[i].fitness = 0;
        float correct = 0;
        
        
        for (int c = 0; c < colorRelations.size(); c++){
            //plus one for every similar relationship
            for (int nc = 0; nc < colorRelations[c].neighborColors.size(); nc++){
                int nCol = colorRelations[c].neighborColors[nc];
                for (int gnc = 0; gnc < genImgColors[c].neighborColors.size(); gnc++){
                    if (genImgColors[c].neighborColors[gnc] == nCol){
                        //population[i].fitness++;
                        correct++;
                        break;
                    }
                }
            }
            
            //minus one for every difference
            for (int gnc = 0; gnc < genImgColors[c].neighborColors.size(); gnc++){
                int gnCol = genImgColors[c].neighborColors[gnc];
                bool gnColFound = false;
                
                for (int nc = 0; nc < colorRelations[c].neighborColors.size(); nc++){
                    if (colorRelations[c].neighborColors[nc] == gnCol){
                        gnColFound = true;
                        break;
                    }
                }
                
                if (!gnColFound) {
                    correct -= genImgColors[c].neighborCount[gnc] * 0.05;
                }
            }
            
        }
        
        population[i].fitness = correct;
    });
}


////-----------------------------------
//int Population::findColor(genImg img, ofVec2f pos){
//    int i = pos.x + pos.y * srcImg.width;
//    int col = img.dna.genes[i];
//    
//    int colID;
//    for (colID = 0; colID < colorRelations.size(); colID++){
//        if (colorRelations[colID].mainColor == col) return colID;
//    }
//    
//    return -1; //just in case
//}


int Population::getGenerations() {
    return generations;
}

// Find highest fintess for the population
float Population::getMaxFitness() {
    float record = -100000000;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].getFitness() > record) {
            record = population[i].getFitness();
        }
    }
    return record;
}

// Find lowest fintess for the population
float Population::getMinFitness() {
    float record = getMaxFitness();
    for (int i = 0; i < population.size(); i++) {
        if (population[i].getFitness() < record) {
            record = population[i].getFitness();
        }
    }
    return record;
}

float Population::getAvgFitness() {
    float avg = 0;
    for (int i = 0; i < population.size(); i++) {
        avg += population[i].getFitness();
    }
    
    avg /= population.size();
    return avg;
}



//--------------------------------------------------------------
void Population::findRelations(genImg &img, vector<colorTable> &colorT){
// 
    int w = img.img.getWidth();
    int h = img.img.getHeight();
    
    //start with the edge cases
    float t1 = t;
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
    findNColor(bottomRightID, img.getColor(brc.x + bottomLeft.x, brc.y + topLeft.y), colorT);
    findNColor(bottomRightID, img.getColor(brc.x + bottom.x, brc.y + bottom.y), colorT);
//    cout << div << endl;
//    cout << "corners - " << t - t1 << endl;

    
    float t2 = t;
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
    
    float t3 = t;
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
int Population::findColor(int col, vector<colorTable> &colorT) {
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
void Population::findNColor(int colID, int nCol, vector<colorTable> &colorT){
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

//--------------------------------------------------------------
void Population::saveSession() {
    session.clear();
    
    session.addTag("mutationRate");
    session.setValue("mutationRate", mutationRate);
    
    session.addTag("generations");
    session.setValue("generations", generations);
    
    int numGenes = population[0].dna.num;
    session.addTag("numGenes");
    session.setValue("numGenes", numGenes);
    
    session.addTag("Tracks");
    session.pushTag("Tracks");
    for (int i = 0; i < population.size(); i++){
        
        session.addTag("Track" + ofToString(i));
        session.pushTag("Track" + ofToString(i));
        session.addTag("fitness");
        session.setValue("fitness", population[i].getFitness());
        for (int j = 0; j < numGenes; j++) {
            session.addTag("gene" + ofToString(j));
            session.setValue("gene" + ofToString(j), population[i].dna.genes[j]);
        }
        session.popTag();
    }
    session.popTag();
    
    session.saveFile("session.xml");
    cout << "session saved" << endl;
}

//--------------------------------------------------------------
void Population::loadSession() {
    session.loadFile("session.xml");
    mutationRate = session.getValue("mutationRate", 0.05);
    generations = session.getValue("generations", 1);
    int numGenes = session.getValue("numGenes", 68);
    
    session.pushTag("Tracks");
    for (int i = 0; i < population.size(); i++) {
        session.pushTag("Track" + ofToString(i));
        population[i].setFitness(session.getValue("fitness", 1.0));
        for (int j = 0; j < numGenes; j++) {
            population[i].dna.genes[j] = session.getValue("gene" + ofToString(j), 0.0);
        }
        population[i].expressGenes(imgColors);
        session.popTag();
    }
    session.popTag();
    
    cout << "session loaded" << endl;
    
}