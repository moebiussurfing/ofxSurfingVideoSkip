/*
	This example requires, to run out-of-the-box,

	That means to use addons.make as is:
	ofxGui
	ofxHapPlayer
	ofxImGui
	ofxSurfingHelpers
	ofxSurfingImGui
	ofxSurfingVideoSkip

	remember to uncomment the line
	#define USE_MINIMAL_ofxSurfingVideoSkip
	in ofxSurfingVideoSkip.h
	That's to disable all the optional addons.
*/

#pragma once

#include "ofMain.h"

//----

#include "ofxSurfingVideoSkip.h"

class ofApp : public ofBaseApp {
public:

	void setup();
	void update();
	void draw();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;
};