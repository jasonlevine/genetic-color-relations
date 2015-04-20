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



#define t ofGetElapsedTimef()
#define div "--------------------"
   
// Create the population
//--------------------------------------------------------------
Population::Population(float m, int num, string imgPath) {
    ofSeedRandom();
    mutationRate = m;
    generations = 0;
    
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
    
    // add all colors to colrtable
    for (int i = 0; i < imgColors.size(); i++) {
        colorTable temp;
        temp.mainColor = i;
        colorTables.push_back(temp);
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
    
    
    CR.setup();
    //find the relations of the course image
//    CR.count = true;
    CR.findRelations(srcDNA, colorTables);
//    CR.count = false;
     //CR.numN;//pow(2, (float)numN);
    
    for (int i = 0; i < num; i++) {
        population.push_back(genImg(srcImg, 4, imgColors.size()));
    }

    perfectScore = (population[0].img.width - 2) * (population[0].img.height - 2);
}

#pragma mark draw-routines
//--------------------------------------------------------------
void Population::draw() {

    float spacing = 30;
    float scale = 10;

    //srcImg.draw(100, 50);
    drawImgScaled(srcImg, 50, 50, 12);
    
    //draw fittest image
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness == getMaxFitness()){
            population[i].expressGenes(imgColors);
            drawImgScaled(population[i].img, 150, 50, 3);
            //population[i].img.draw(150, 50);
            population[i].generateHeatMap();
            drawImgScaled(population[i].heatmap, 250, 50, 3);
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

#pragma mark GA
// Generate a mating pool
void Population::selection() {
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
        DNA child = momgenes.crossover(dadgenes, mutationRate);
        // Mutate their genes
//        child.mutate(mutationRate);
        // Fill the new population with the new child
        population[i].dna = child;
        //population[i].expressGenes(imgColors);
    }
                   
    generations++;
}

void Population::calcFitness(){
    dispatch_queue_t gcdq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    int start = 0;
    int end = population.size();
    dispatch_apply(end-start, gcdq, ^(size_t blockIdx){
        int i = start+blockIdx;
        
        population[i].fitness = CR.calcFitness(population[i], colorTables);
        
        /*
        //make an empty colortable
        vector<colorTable> genImgColors;
        for (int i = 0; i < imgColors.size(); i++) {
            colorTable temp;
            //temp.mainColor = i;
            genImgColors.push_back(temp);
        }
        
        //find relations of pop[i]
        CR.findRelations(population[i], genImgColors);
        
        population[i].fitness = 0;
        float correct = 0;
        
        
        //calc fitness
        for (int c = 0; c < colorTables.size(); c++){
            for (int gnc = 0; gnc < genImgColors[c].neighborColors.size(); gnc++){
                int gnCol = genImgColors[c].neighborColors[gnc];
                bool gnColFound = false;
                
                for (int nc = 0; nc < colorTables[c].neighborColors.size(); nc++){
                    if (colorTables[c].neighborColors[nc] == gnCol){
                        gnColFound = true;
                        correct++; //plus one for every similar relationship
                        break;
                    }
                }
                
                if (!gnColFound) {
                    correct -= genImgColors[c].neighborCount[gnc] * 0.05; //minus 0.05 for every difference
                }
            }
        }
        
        population[i].fitness = correct;*/
    });
        
}



#pragma mark getters
//--------------------------------------------------------------
int Population::getGenerations() {
    return generations;
}

// Find highest fintess for the population
//--------------------------------------------------------------
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
//--------------------------------------------------------------
float Population::getMinFitness() {
    float record = getMaxFitness();
    for (int i = 0; i < population.size(); i++) {
        if (population[i].getFitness() < record) {
            record = population[i].getFitness();
        }
    }
    return record;
}

//--------------------------------------------------------------
float Population::getAvgFitness() {
    float avg = 0;
    for (int i = 0; i < population.size(); i++) {
        avg += population[i].getFitness();
    }
    
    avg /= population.size();
    return avg;
}


#pragma mark color-relations

#pragma mark load-save
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