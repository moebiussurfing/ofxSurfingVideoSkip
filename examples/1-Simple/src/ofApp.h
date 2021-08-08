#pragma once

#include "ofMain.h"

#include "ofxSurfingVideoSkip.h"

#define INCLUDE_ofxWindowApp
#ifdef INCLUDE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

/*

About this example '1-Simple':
Go to ofxSurfingVideoSkip.h and
comment all the 'OPTIONAL DEFINES'
to disable all the optional add-ons!

*/


class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	
	ofxSurfingVideoSkip player;

#ifdef INCLUDE_ofxWindowApp
	ofxWindowApp windowApp;
#endif
};