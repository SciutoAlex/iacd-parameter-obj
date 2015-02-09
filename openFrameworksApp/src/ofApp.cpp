#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
    
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    
    bLearnBakground = true;
    threshold = 30;
    
    f.open("opencvtest.json",ofFile::WriteOnly);
    

    
    f << "{" << std::endl;
    f << "\"data\":[" << std::endl;
    
   }

//--------------------------------------------------------------
void ofApp::update(){
    
    bool bNewFrame = false;
    
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
    
    if (bNewFrame){
        
        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
        
        
        grayImage = colorImg;
        if (bLearnBakground == true){
            grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }
        
        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 60, (340*240)/3, 1, false, true);	// find holes
    }
}

void record_blobs(ofxCvContourFinder &cf, ofFile &f) {
    
    if(cf.nBlobs) {
        f << "[" << std::endl;
        
        
        for (int i = 0; i < cf.nBlobs; i++){
            
            string blobName = ofToString(i);
            
            
            for (int j = 0; j < cf.blobs[i].nPts; j++){
                string ptName = ofToString(j);
                
                vector<string> concats;
                concats.resize(3);
                concats[0] = blobName;
                concats[1] = "-";
                concats[2] = ptName;
                
                vector<string> values;
                values.resize(2);
                values[0] = ofToString(cf.blobs[i].pts[j].x);
                values[1] = ofToString(cf.blobs[i].pts[j].y);
                if(j == cf.blobs[i].nPts-1) {
                    f << "[" << ofJoinString(values,",") << "]" << std::endl;
                } else {
                    f << "[" << ofJoinString(values,",") << "]," << std::endl;
                }
                
                
 
            }
            
        }
        
        
        
        f << "]," << std::endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    colorImg.draw(20,20);
    
    ofFill();
    ofSetHexColor(0x333333);
    ofSetHexColor(0xffffff);
    
    // we could draw the whole contour finder
    //contourFinder.draw(360,540);
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    
    if(record && (ofGetElapsedTimef() - lastDrawTime > .5)) {
        record_blobs(contourFinder, f);
        lastDrawTime = ofGetElapsedTimef();
    }
    
        
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(20,20);
        // draw over the centroid if the blob is a hole
        ofSetColor(255);
    }
    

    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
        case 'd':
            f << "]" << std::endl;
            f << "}" << std::endl;
            f.close();
        case 'b':
            record = !record;
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