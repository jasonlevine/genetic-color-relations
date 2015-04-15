#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    
    int popmax = 80;
    float mutationRate = 0.0001;
    GA = new Population(mutationRate,popmax,"gradient.jpg");
    
    GA->calcFitness();
    GA->selection();
    GA->reproduction();
    GA->calcFitness();
    
    saveFbo.allocate(640, 480);
    counter = 0;
    lastMaxFitness = GA->getMaxFitness();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if (GA->getMaxFitness() < GA->perfectScore) {
//        float t1 = ofGetElapsedTimef();
        GA->selection();
//        cout << "------------------" << endl;
//        cout << "selection - " << ofGetElapsedTimef() - t1<< endl;
        
//        float t2 = ofGetElapsedTimef();
        GA->reproduction();
//        cout << "reproduction - " << ofGetElapsedTimef() - t2 << endl;
        
        
//        float t3 = ofGetElapsedTimef();
        GA->calcFitness();
//        cout << "fitness - " << ofGetElapsedTimef() - t3 << endl;
        
        
        elapsedTime = ofGetElapsedTimef();
        seconds = elapsedTime % 60;
        minutes = (elapsedTime / 60) % 60;
        hours = (elapsedTime / 3600);
        

//        cout  << GA->getMaxFitness() << " " << GA->getAvgFitness() << " -------- " << GA->getGenerations() << endl;
    }

    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    saveFbo.begin();
    ofClear(0);
    ofBackground(120, 20 , 0);
    
    GA->draw();
    
    ofSetColor(255);
    ofDrawBitmapString( ofToString(GA->getMaxFitness(),1) + "/" + ofToString(GA->perfectScore) + " --- gen " + ofToString(GA->getGenerations()) + " --- " + ofToString(hours) + " hours " + ofToString(minutes) + " minutes " + ofToString(seconds) + " seconds", 10, 10);
    
    saveFbo.end();

    
    if (GA->getMaxFitness() > lastMaxFitness) {
        ofImage saveImage;
        saveImage.allocate(640, 480, OF_IMAGE_COLOR);
        saveFbo.readToPixels(saveImage.getPixelsRef());
        saveImage.update();
        saveImage.saveImage("sequence9/evo" + ofToString(counter) + ".png");
        
        counter++;
        lastMaxFitness = GA->getMaxFitness();
    }
    
    ofSetColor(255);
    saveFbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 32) {
        GA->selection();
        GA->reproduction();
        GA->calcFitness();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
