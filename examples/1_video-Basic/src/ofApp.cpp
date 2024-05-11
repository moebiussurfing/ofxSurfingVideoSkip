#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	#if 0
	ofSetWindowShape(1920, 1060);
	ofSetWindowPosition(-1920, 25);
	#else
	#endif

	player.setup();

	ofSetEscapeQuitsApp(false);
}

//--------------------------------------------------------------
void ofApp::update()
{
	string s = ofToString(ofGetFrameRate(), 0) + " Fps";
	ofSetWindowTitle(s);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	player.dragEvent(dragInfo);
}
