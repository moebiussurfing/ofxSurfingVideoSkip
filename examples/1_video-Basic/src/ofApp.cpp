#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowShape(1920, 1060);
	ofSetWindowPosition(-1920, 25);

	player.setup();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	player.dragEvent(dragInfo);
}
