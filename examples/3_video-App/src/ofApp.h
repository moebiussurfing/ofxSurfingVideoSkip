#pragma once

#include "ofMain.h"


//----


/*

	This example requires, to run out-of-the-box,
	go to ofxSurfingVideoSkip.h and comment the line
	//#define USE_MINIMAL__VIDEO_SKIP
	That's to allow enable some of the optional add-ons.

	Then uncomment only these lines to enable the optional add-ons like i.e.:

		// Optional compatible add-ons list:

		// -> 1A. Simple Presets
		#define USE_ofxSurfingPresets__VIDEO_SKIP

		// -> 2. Mood Machine
		#define USE_ofxSurfingMoods__VIDEO_SKIP

		// -> 3. Beat Clock
		#define USE_ofxBeatClock__VIDEO_SKIP

		// -> 5. FX Channel
		#define USE_ofxSurfingFxChannel__VIDEO_SKIP

	Let the other Optional Modules commented/disabled!

	Anyway you can/should modify the addons.make and use the PROJECT GENERATOR
	to enable the selected optional add-ons from the list.

*/


//----


#include "ofxSurfingVideoSkip.h"

#include "ofxWindowApp.h"
#include "ofxSurfingSplashScreen.h"

class ofApp : public ofBaseApp
{
public:

	void setup();
	void draw();
	void keyPressed(int key);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;

	ofxWindowApp w;

	ofxSurfingSplashScreen splash;
	ofColor c2 = ofColor::orange;
	ofColor c1 = ofColor::violet;
};