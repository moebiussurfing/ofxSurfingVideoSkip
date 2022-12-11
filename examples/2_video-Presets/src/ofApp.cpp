#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	w.setEscapeQuitsApp(false);
	w.setConsoleVisible(false);

	player.setup();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	player.dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'T') w.setToggleAlwaysOnTop();
}