#pragma once

#include "ofMain.h"

//----

#include "ofxSurfingVideoSkip.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp {
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;

	ofxWindowApp windowApp;
};