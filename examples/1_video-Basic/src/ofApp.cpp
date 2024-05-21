#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	player.setup();

	//ofSetEscapeQuitsApp(false);
}

//--------------------------------------------------------------
void ofApp::update() {
	string s = ofToString(ofGetFrameRate(), 0) + " Fps";
	s += "    ";
	s += player.getVideoFilepath();
	ofSetWindowTitle(s);
}

//--------------------------------------------------------------
void ofApp::draw() {
	player.draw();
	player.drawGui();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	player.dragEvent(dragInfo);
}
