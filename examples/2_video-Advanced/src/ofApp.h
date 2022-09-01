#pragma once

#include "ofMain.h"

//----

/*

	This example requires, to run out-of-the-box,
	go to ofxSurfingVideoSkip.h and comment the line
	//#define USE_MINIMAL_ofxSurfingVideoSkip
	That's to allow enable some of the optional add-ons.

	Then uncomment only these lines to enable the optional add-ons like i.e.:
	#define USE_ofxSurfingPresets__VIDEO_SKIP // -> 1A. simple presets

	Let the other commented.
	Anyway you can modify the addons.make and use the PROJECT GENERATOR
	to enable the selected optional add-ons from the list.

*/

//----

#include "ofxSurfingVideoSkip.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp
{
public:

	void setup();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;

	ofxWindowApp w;
};