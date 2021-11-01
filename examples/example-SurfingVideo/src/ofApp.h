#pragma once

#include "ofMain.h"

//----

#define INCLUDE_ofxWindowApp // -> Not required

//----

#include "ofxSurfingVideoSkip.h"

#ifdef INCLUDE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

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