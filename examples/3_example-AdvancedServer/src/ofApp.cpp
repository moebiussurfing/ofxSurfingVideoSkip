#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() 
{
	windowApp.setEnableTimerSaver(true);

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
