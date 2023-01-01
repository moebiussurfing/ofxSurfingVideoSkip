#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//w.setEscapeQuitsApp(false);
	//w.setConsoleVisible(false); // breaks thumbs thread
	
	//--

	// Customize
	// Uncomment to disable
	//if (0) 
	{
		//splash.setDebug(true);

		//splash.setDuration(4000); 

		// No floating mode (false) 
		// means window is not resized nor border less control!
		splash.setModeFloating(false);
		splash.setAlphaBgMax(170);
		splash.setColorBorder1(ofColor(c1, 100));
		splash.setColorBorder2(ofColor(c2, 70));
	}

	splash.setup("assets/images/cover.png");

	//--

	player.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//ofClear(c1);

	// B.
	// Draw under (before) the Scene
	//if (splash.draw()) return;
	//// Skipped during splashing

	//--

	player.draw();

	//--

	// A.
	// Draw on top (after) the Scene
	splash.draw();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	player.dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'T') w.setToggleAlwaysOnTop();

	// Splash with timer to hide!
	if (key == OF_KEY_F11) splash.start();

	// Splash latched! 
	// Mouse click or trig again to hide!
	if (key == OF_KEY_F10) splash.doToggleLatch();
}