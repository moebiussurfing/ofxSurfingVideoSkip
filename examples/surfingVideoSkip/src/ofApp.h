/*
	This example requires, to run out-of-the-box,

	That means to use addons.make as is:
	ofxGui
	ofxHapPlayer
	ofxImGui
	ofxSurfingHelpers
	ofxSurfingImGui
	ofxSurfingVideoSkip

	ofxMidi
	ofxMidiOutClock
	ofxSurfingBox
	ofxSurfingMoods
	ofxSurfingPresets
	ofxBeatClock
	ofxWindowApp

	remember to uncomment the line
	#define USE_MINIMAL_ofxSurfingVideoSkip
	in ofxSurfingVideoSkip.h
	That's to disable all the optional addons.
*/

#define USE_NDI_HelperSender
#define USE_SURFING_OFX_WINDOW_APP

#pragma once

#include "ofMain.h"

//----

#include "ofxSurfingVideoSkip.h"

#ifdef USE_NDI_HelperSender
	#include "NDI_HelperSender.h"
#endif

#ifdef USE_SURFING_OFX_WINDOW_APP
	#include "ofxWindowApp.h"
#endif

//--

class ofApp : public ofBaseApp {
public:

	void setup();
	void update();
	void draw();
	void drawGui();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;

#ifdef USE_NDI_HelperSender
	NDI_HelperSender ndi;
#endif

#ifdef USE_SURFING_OFX_WINDOW_APP
	ofxWindowApp w;
#endif
};
