//
//  Track.cpp
//  emptyExample
//
//  Created by Jason Levine on 2013-05-29.
//
//




#include "genImg.h"

genImg::genImg(ofImage &src, int scale, int nColors) {
    dna = DNA(src.getWidth()*scale * src.getHeight()*scale, nColors);
    img.allocate(src.getWidth()*scale, src.getHeight()*scale, OF_IMAGE_COLOR);
    heatmap.allocate(src.getWidth()*scale-2, src.getHeight()*scale-2, OF_IMAGE_COLOR);
    fitness = 1;
    
    
}

void genImg::expressGenes(vector<ofColor> &colors) {
   
    for (int g = 0; g < dna.genes.size(); g++){
        int x = g % (int)img.getWidth();
        int y = g / (int)img.getWidth();
        
//        int colId = ofMap(dna.genes[g], 0.0, 1.0, 0, colors.size());
        
        img.setColor(x, y, colors[dna.genes[g]]);
    }
    img.update();
        
}

void genImg::generateHeatMap(){
    for (int i = 0; i < dna.geneFitness.size(); i++){
        int x = i % (int)heatmap.getWidth();
        int y = i / (int)heatmap.getHeight();
        
        ofColor col;
        col.setHsb( 255, 0, dna.geneFitness[i]*255);
        heatmap.setColor(x, y, col);
    }
}


DNA genImg::getDNA() {
    return dna;
}



float genImg::getFitness() {
    return fitness;
}

int genImg::getColor(int x, int y){
    int i = x + y * img.width;
    return dna.genes[i];
}
