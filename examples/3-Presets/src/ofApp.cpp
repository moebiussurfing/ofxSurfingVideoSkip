#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifndef INCLUDE_ofxWindowApp
	ofSetFrameRate(60);
#endif
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	myHapSkipper.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	myHapSkipper.dragEvent(dragInfo);
}
