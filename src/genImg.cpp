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


DNA genImg::getDNA() {
    return dna;
}



float genImg::getFitness() {
    return fitness;
}
