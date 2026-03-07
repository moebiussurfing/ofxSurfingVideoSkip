#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef USE_SURFING_OFX_WINDOW_APP
	w.setup(&w);
	ofSetWindowShape(1080, 1920); //force portrait mode
#else
	ofSetWindowShape(1080, 1920);
#endif

	//--

#ifdef USE_NDI_HelperSender
	ndi.setup(1080, 1920, "surfingVideoSkip");
#endif

	//--

	player.setup();

	//ofSetEscapeQuitsApp(false);

	ofSetLogLevel("ofxSurfingVideoSkip", OF_LOG_WARNING);
	ofSetLogLevel("ofxSurfingMoods", OF_LOG_WARNING);
	ofSetLogLevel("ofxSurfingHelpers", OF_LOG_WARNING);
	ofSetLogLevel("ofxSurfingPresets", OF_LOG_WARNING);
	ofSetLogLevel("surfingParamIndexToggles", OF_LOG_WARNING);
}

//--------------------------------------------------------------
void ofApp::update() {
	string s = ofToString(ofGetFrameRate(), 0) + " FPS";
	s += " - ";
	s += player.getVideoFilepath();
	ofSetWindowTitle(s);

#ifdef USE_NDI_HelperSender
	ndi.begin();
	{
		player.draw();
	}
	ndi.end();
#endif
}

//--------------------------------------------------------------
void ofApp::draw() {

#ifdef USE_NDI_HelperSender
	ndi.draw();
#else
	player.draw();
#endif

	//--

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	player.drawGui();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	player.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	player.dragEvent(dragInfo);
}
