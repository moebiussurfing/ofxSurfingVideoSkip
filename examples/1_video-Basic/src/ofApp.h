#pragma once

#include "ofMain.h"

/*

	This example requires, to run out-of-the-box,

	That means to use addons.make as is:
	ofxGui
	ofxHapPlayer
	ofxImGui
	ofxSurfingHelpers
	ofxSurfingImGui
	ofxSurfingVideoSkip

	to uncomment the line
	#define USE_MINIMAL_ofxSurfingVideoSkip
	in ofxSurfingVideoSkip.h
	That's to disable all the optional add-ons.
	Like that:
	//#define USE_ofxSurfingPresets__VIDEO_SKIP //-> Recommended! for gui integration
	..

*/

//----

#include "ofxSurfingVideoSkip.h"

class ofApp : public ofBaseApp {
public:

	void setup();
	void update();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;
};