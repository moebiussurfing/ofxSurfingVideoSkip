#pragma once
#include "ofxNDI.h" // NDI classes
class NDI_HelperSender {
public:
	void setup(int w, int h, string s);
	void begin();
	void end();
	void draw();
	void exit();

	ofxNDIsender ndiSender; // NDI sender
	std::string senderName; // Sender name
	unsigned int senderWidth = 0; // Width of the sender output
	unsigned int senderHeight = 0; // Height of the sender output
	bool bInitialized = false;
	ofFbo m_fbo; // Fbo used for graphics and sending

	bool bReadback = true;
	bool bAsync = true;
	double framerate = 60.0;
};
