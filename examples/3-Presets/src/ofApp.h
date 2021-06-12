#pragma once

#include "ofMain.h"

//----

#define INCLUDE_ofxWindowApp

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
	
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	ofxSurfingVideoSkip myHapSkipper;

#ifdef INCLUDE_ofxWindowApp
	ofxWindowApp windowApp;
#endif

};