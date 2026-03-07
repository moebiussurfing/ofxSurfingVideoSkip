#include "NDI_HelperSender.h"

//--------------------------------------------------------------
void NDI_HelperSender::setup(int w, int h, string s = "") {
	if (s == "") {
		senderName = "Openframeworks NDI Sender";
	} else
		senderName = s;

	ofSetWindowTitle(senderName); // show it on the title bar

#ifdef _WIN64
	std::cout << "\nofxNDI example sender - 64 bit" << std::endl;
#else // _WIN64
	std::cout << "\nofxNDI example sender - 32 bit" << std::endl;
#endif // _WIN64

	// Set the dimensions of the sender output here.
	// This is independent of the display window size.
	// It can be changed using the 'S' key.
	// 4K (3840x2160) can help assess performance of different options.
	senderWidth = w;
	senderHeight = h;

	// Create an RGBA fbo for collection of data
	m_fbo.allocate(senderWidth, senderHeight, GL_RGBA);

	// Option : set readback
	// Pixel data extraction from fbo or texture
	// is optimised using two OpenGL pixel buffers (pbo's)
	// Note that the speed can vary with different CPUs
	ndiSender.SetReadback(true);

	// Option : set the framerate
	// NDI sending will clock at the set frame rate
	// The application cycle will also be clocked at that rate
	//
	// Can be set as a whole number, e.g. 60, 30, 25 etc
	// ndiSender.SetFrameRate(30);
	//
	// Or as a decimal number e.g. 29.97
	// ndiSender.SetFrameRate(29.97);
	//
	// Or as a fraction numerator and denominator
	// as specified by the NDI SDK - e.g.
	// NTSC 1080 : 30000, 1001 for 29.97 fps
	// NTSC  720 : 60000, 1001 for 59.94fps
	// PAL  1080 : 30000, 1200 for 25fps
	// PAL   720 : 60000, 1200 for 50fps
	// ndiSender.SetFrameRate(30000, 1001);
	//
	// Note that the NDI sender frame rate should match the render rate
	// so that it's displayed smoothly with NDI Studio Monitor.
	//
	// ndiSender.SetFrameRate(30); // Enable this line for 30 fps instead of default 60.

	// Option : set NDI asynchronous sending
	// If disabled, the render rate is clocked to the sending framerate.
	// Note that when sending is asynchronous, frames can be sent at a higher
	// rate than the receiver can process them and hesitations may be evident.
	// ndiSender.SetAsync(true);

	// Create a sender with RGBA output format
	bInitialized = ndiSender.CreateSender(senderName.c_str(), senderWidth, senderHeight);

	// A Sender with the same name cannot be created.
	// In case the executable has been renamed, and there
	// is already a sender of the same NDI name running,
	// increment the NDI name and try again.
	if (!bInitialized) {
		printf("Could not create [%s]\n", senderName.c_str());
		senderName += "_2";
		bInitialized = ndiSender.CreateSender(senderName.c_str(), senderWidth, senderHeight);
		// If that still fails warn the user and quit
		if (!bInitialized) {
			MessageBoxA(NULL, "Could not create sender", "", MB_ICONWARNING | MB_OK);
			//exit();
		}
	}
	printf("Created sender   [%s]\n", senderName.c_str());
}

//--------------------------------------------------------------
void NDI_HelperSender::begin() {
	m_fbo.begin();
}

//--------------------------------------------------------------
void NDI_HelperSender::end() {
	m_fbo.end();
}

void NDI_HelperSender::draw() {
	m_fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	ndiSender.SendImage(m_fbo);
}
//--------------------------------------------------------------
void NDI_HelperSender::exit() {
	ndiSender.ReleaseSender();
}
