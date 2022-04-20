#pragma once

#include "ofMain.h"

/*

This example requires, to run out-of-the-box,
to uncomment the line
#define USE_MINIMAL_ofxSurfingVideoSkip
in ofxSurfingVideoSkip.h
That's to disable all the optional add-ons.

*/

//----

#include "ofxSurfingVideoSkip.h"

class ofApp : public ofBaseApp {
public:

	void setup();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;
};