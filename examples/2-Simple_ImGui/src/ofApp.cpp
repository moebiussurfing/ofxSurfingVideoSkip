#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//ofBackground(0);

#ifndef INCLUDE_ofxWindowApp
	ofSetFrameRate(60);
#endif

	myHapSkipper.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	myHapSkipper.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	myHapSkipper.draw();
	myHapSkipper.draw_Gui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//myHapSkipper.keyPressed(key);
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	myHapSkipper.windowResized(w, h);
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	myHapSkipper.dragEvent(dragInfo);
}
//mouse
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	myHapSkipper.mouseMoved(x, y);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	myHapSkipper.mouseDragged(x, y, button);
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	myHapSkipper.mousePressed(x, y, button);
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	myHapSkipper.mouseReleased(x, y, button);
}

