#pragma once

#include "ofMain.h"

//----

/*

This example requires, to run out-of-the-box,
to go to ofxSurfingVideoSkip.h and comment the line
//#define USE_MINIMAL_ofxSurfingVideoSkip
That's to allow enable some of the optional add-ons.
In this example we will enable all the optional add-ons.

Then we uncomment these lines to enable the add-ons like i.e.:

// -> 1A. simple presets
#define USE_ofxSurfingPresets__VIDEO_SKIP
// -> 2. Mood machine
#define USE_ofxSurfingMoods

Let the other commented.
Anyway you can modify the addons.make and use the PROJECT GENERATOR
to enable the selected optional add-ons from the list.

*/

//----

#include "ofxSurfingVideoSkip.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp {
public:

	void setup();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxSurfingVideoSkip player;

	ofxWindowApp windowApp;
};