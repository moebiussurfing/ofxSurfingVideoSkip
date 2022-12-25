//
// Created by moebiusSurfing, 2019-2022.
//

#pragma once

#include "ofMain.h"

/*

	IDEAS:
	mark some word with another color
	add center offset or duration, to translate a clip/loop

	TODO:
	+ make thumbs using threading

	UI:
	fix custom range big slider, un exact position

	OSC:
	bang next makes double jump!
	fix targets draw for button testing
	fix / add enablers
		add all/none

	MOOD:
	link manual slider to value_0
	preview widget blink faster

	PRESETS:
	preset is overwriting sometimes
	edit mode not working when using inner presets panel

	BUGS:
	+ zero time loop wrong back...
	+ fine tunes: kicks working weird

	ENGINE:
	+ should be oriented more to start point vs end/duration..
	+ improve Edit mode and lock
		+ when edit mode, and dragging start/end, move position
	+ add shift loop
		+ add control to move the loop clip with same duration?
		+ lock start/end then move "centered" on edit
	+ add slow skip timers
	+ add new fx skip-engine jump bpm timed shutter
	+ zoom on loop zone. maybe with stills
	+ mark start points on bar. add one/first still for each clip

	APP:
	+ full screen/dual window. external output.
	+ populate midi notes for available presets
		or make an engine to centralize all midi stuff for different modules.
	+ fix docking for all panels. Should split main 3 panels too..
		+ preview window and central docking layouting

*/


//----------------------------------------------
//
//	OPTIONAL DEFINES

// MAIN
// 
//#define USE_MINIMAL__VIDEO_SKIP
 // -> Uncomment this above line to: 
// Force and disable ALL the below optional add-ons / compatible modules. 
// Then allows only the use of the MINIMAL CORE stuff of the add-on.

#ifndef USE_MINIMAL__VIDEO_SKIP

//--

// Optional compatible add-ons list:

// Presets. 
// Two alternatives:
//
// -> 1A. Simple Presets
// -> Recommended! for GUI integration
#define USE_ofxSurfingPresets__VIDEO_SKIP 
// -> 1B. Power Presets
//#define USE_ofxPresetsManager__VIDEO_SKIP 

// -> 2. Mood Machine
#define USE_ofxSurfingMoods__VIDEO_SKIP 

// -> 3. Beat Clock
#define USE_ofxBeatClock__VIDEO_SKIP

// -> 4. MIDI Input to control GUI and switch presets
#define USE_MIDI_PARAMS__VIDEO_SKIP 

// -> 5. FX Channel
#define USE_ofxSurfingFxChannel__VIDEO_SKIP 

//TODO:
// -> 6. Remote Parameters
//#define USE_ofxRemoteParameters 
// This add-on will server some parameters to allow remote control fast.
// Will serve OSC/MIDI

// -> 7. FX Pro
#define USE_ofxSurfingFxPro__VIDEO_SKIP

// -> 8. OSC Remote
//#define USE_ofxSurfingOsc

// -> 9. NDI 
//#define USE_ofxNDI

// -> 10. SRT subtitler
//#define USE_ofxSurfingTextSubtitle__VIDEO_SKIP

//--

#endif // end of no minimal stuff

//----------------------------------------------

//#include "ofApp.h"

// Core Stuff

#include "PlayerAV.h"

#include "ofxInteractiveRect.h"
#include "ofxSurfingImGui.h"
#include "WindowFbo.h"

//#define AUTO_GENERATE_THUMBS_ON_LOADING

//--

// Optional Stuff

#ifdef USE_ofxNDI
#include "SurfingNDIManager.h"
#endif

#ifdef USE_ofxSurfingFxPro__VIDEO_SKIP
#include "ofxSurfingFxPro.h"
#endif

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
#include "ofxSurfingPresets.h"
#endif

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
#include "ofxPresetsManager.h"
#endif

#ifdef USE_ofxSurfingMoods__VIDEO_SKIP
#include "ofxSurfingMoods.h"
#endif

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP 
#include "ofxMidiParams.h"
#endif

#ifdef USE_ofxBeatClock__VIDEO_SKIP
#include "ofxBeatClock.h"
#endif

#ifdef USE_ofxSurfingFxChannel__VIDEO_SKIP
#include "ofxSurfingFxChannel.h"
#endif

#ifdef USE_ofxRemoteParameters
#include "ofxRemoteParameters/Server.h"
#endif

#ifdef USE_ofxSurfingOsc
#include "ofxSurfingOsc.h"
#endif

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
#include "ofxSurfingTextSubtitle.h"
#endif

//----


#define BAR_INSET 4
//#define BAR_INSET 15

//#define BAR_HEIGHT 50
#define BAR_HEIGHT 30
//#define BAR_HEIGHT 60
//#define BAR_HEIGHT 80
//#define BAR_HEIGHT 40

//#define SPEED_MIN 0.20f
//#define SPEED_MAX 50.0f
#define SPEED_MIN 0.25f
#define SPEED_MAX 4.0f

#include "CommandThread.h" // to call the thumbs generator without breaking drawing!

//--------------------------------------------------------------
class ofxSurfingVideoSkip
{
public:

	//--------------------------------------------------------------
	ofxSurfingVideoSkip()
	{
		ofAddListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		//ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);

		addMouseListeners();
	}

	//--------------------------------------------------------------
	~ofxSurfingVideoSkip()
	{
		ofRemoveListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		//ofRemoveListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);

		removeMouseListeners();

		exit();
	}

	//--

private:

	PlayerAV player;

	//--

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
public:
	ofxSurfingTextSubtitle subs;
	string pathSubs;
	//--------------------------------------------------------------
	void refreshTimerPlayForced()
	{
		//TODO: pass time to subtitler
		auto t = ((60000 / bpmDivider.get()) / (bpm.get()));
		//auto t  = divBeatSkipper.get() * ((60000 / bpmDivider.get()) / (bpm.get()));
		subs.durationPlayForced = t;
	}
#endif

	//--

public:

	ofDirectory dirThumbs;
	vector<ofImage> imgThumbs;
	void loadThumbs();
	//float thumbGap=0;
	bool bFit = true;

	bool bError = false;

	//TODO:
	// https://github.com/ocornut/imgui/issues/5627#event-7303371114
	//--------------------------------------------------------------
	static void AspectRatio(ImGuiSizeCallbackData* data) {
		float aspect_ratio = *(float*)data->UserData;
		data->DesiredSize.x = IM_MAX(data->CurrentSize.x, data->CurrentSize.y);
		data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio);
	}

	//--

#ifdef USE_ofxSurfingOsc
private:
	ofxSurfingOsc oscHelper;
	void setup_Osc();
	//ofParameter<bool> bBypass{ "ByPass", false };
	void Changed_Targets(ofAbstractParameter& e);
#endif

	//--

#ifdef USE_ofxNDI
	SurfingNDIManager ndi;
#endif

	//--

public:

	void setup();
	void draw();

private:

	void setup_AppSettings();
	void setup_Remote();
	void setup_Preset();
	void setup_PresetsStuff();

	void update(ofEventArgs& args);

	//void draw(ofEventArgs& args);

	void exit();

private:

	bool bToolTips = false;

	// Keys
	bool mod_CONTROL;
	bool mod_ALT;

	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);

	void addKeysListeners();
	void removeKeysListeners();

	// Mouse

	void mouseDragged(ofMouseEventArgs& eventArgs);
	void mousePressed(ofMouseEventArgs& eventArgs);
	void mouseReleased(ofMouseEventArgs& eventArgs);
	void mouseMoved(ofMouseEventArgs& eventArgs);
	void mouseScrolled(ofMouseEventArgs& eventArgs);
	void mouseRefreshPressed(int button, float position);
	void addMouseListeners();
	void removeMouseListeners();

public:

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

private:

	void startup();
	void updateVideoPlayer();
	void updateTimers();
	void updateConstraints();

public:

	void draw_Gui();
	void draw_Video();
	void draw_VideoBarControl();

	//--

	// FX Processor

#ifdef USE_ofxSurfingFxPro__VIDEO_SKIP
	ofxSurfingFxPro fxPro;
#endif

	//--

	// Presets

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
public:
	ofxSurfingPresets presetsManager;
#endif

	//--

	// Midi

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
private:
	ofxMidiParams mMidiParams;
#endif

	//--

	// Clock

#ifdef USE_ofxBeatClock__VIDEO_SKIP
	ofxBeatClock beatClock;
	ofEventListener listenerBeat;
	void Changed_BeatTick();
	//ofEventListener listenerBpm;
	//void Changed_BeatBpm();
#endif

	//--

	// Remote

#ifdef USE_ofxRemoteParameters
	ofxRemoteParameters::Server remoteServer;
	ofParameterGroup params_Remote;
	bool bSyncRemote = false;
#endif

	//--

public:

	// GUI

	ofxSurfingGui ui;

	void setup_ImGui();

	void draw_ImGui();
	void draw_ImGui_Docking();
	void draw_ImGui_Main();
	void draw_ImGui_SkipTimers();
	void draw_ImGui_Preview();
	void draw_ImGui_Menu();

	void draw_ImGui_GameMode(); // final user selected / most important controls!

public:

	ofParameter<bool> bGui; // independent to auto hide state
	ofParameter<bool> bGui_Main;
	ofParameter<bool> bGui_SkipTimers;
	ofParameter<bool> bGui_Previews;

	//--

private:

	ofParameter<bool> bLinkAllFiles; // video, audio and srt.

	// Preview Monitor
	SurfingPreview surfingPreview;

	// If some Fx enabled
#ifndef USE_MINIMAL__VIDEO_SKIP
#if defined(USE_ofxSurfingFxChannel__VIDEO_SKIP) || defined(USE_ofxSurfingFxPro__VIDEO_SKIP) 
	std::vector<std::string> previewSources = { "Source", "Processed" };
#else
	std::vector<std::string> previewSources = { "Source" };
#endif
	//#else
	//	std::vector<std::string> previewSources = { "Source" };
#endif

	ofParameter<int> indexPreviewSource; // selector

	//--

private:

	bool bDockingReset = false;
	void dockingReset();

	//--

public:

	//--------------------------------------------------------------
	ofParameterGroup& getParams() {
		return params_Preset;
	}

	void doOpenDialogToSetPath();

	bool bThumbsReadyFbo = false;
	void doGenerateThumbs();

	//--------------------------------------------------------------
	void doRunCommand(string s) {
		cout << (s) << endl << endl;
		cout << ofSystem(s) << endl;
	};
	
	CommandThread commandThread;

private:

	ofTrueTypeFont font;
	int fontSize = 30;//big

	void setPath_GlobalFolder(std::string folder);//path for root container folder
	std::string path_GLOBAL_Folder;//main folder where nested folder goes inside

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	std::string path_fileName_ChannelFX;
	std::string path_Preset;
#endif

	bool bActive = true;
	bool bLoaded = false;

	ofParameter<bool> bKeys{ "Keys", true };

#ifdef USE_ofxSurfingFxChannel__VIDEO_SKIP
	ofxSurfingFxChannel channelFx;
#endif

	//----

	ofParameter<bool>bTheme{ "Theme", false };

public:

	void setActive(bool b);
	void setKeysEnable(bool b);
	void setGuiVisible(bool b);

	//--------------------------------------------------------------
	void setGuiToggleVisible() {
		bGui = !bGui;
	}

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//--------------------------------------------------------------
	void loadPreset(int p)
	{
		presetsManager.loadPreset(p);
	}
	//--------------------------------------------------------------
	void setUserVisible(bool b)
	{
		// presets
		presetsManager.setVisible_PresetClicker(b);
		presetsManager.setEnableKeys(b);
	}
#endif

	////--------------------------------------------------------------
	//void setMODE_App(int m)
	//{
	//	MODE_App = m;
	//}

	// auto hide controlled outside the class. internal by default
	//--------------------------------------------------------------
	void setAudoHideExternal(bool b)
	{
		ENABLE_AutoHide_external = b;
	}

	//--------------------------------------------------------------
	void setAudoHide(bool b)
	{
		bAutoHide_BarControl = b;
		//tLastMovement = ofGetSystemTimeMillis();
	}

	//--------------------------------------------------------------
	void setPlay(bool b)
	{
		bPlay = b;
	}

	//--------------------------------------------------------------
	void setTogglePlay()
	{
		bPlay = !bPlay;
	}

	//--------------------------------------------------------------
	bool isPlaying()
	{
		return bPlay;
	}

	//--

#ifdef USE_ofxSurfingMoods__VIDEO_SKIP

	//--------------------------------------------------------------
	void setPlay_MoodMachine(bool b)
	{
		if (b) moods.play();
		else moods.stop();
	}

	//--------------------------------------------------------------
	bool isPlaying_MoodMachine()
	{
		return moods.isPlaying();
	}

	////--------------------------------------------------------------
	//bool get_MoodMachine_EditorMode()
	//{
	//	return SHOW_MoodMachine.get();
	//}

#endif

	//--

private:

#ifdef USE_ofxSurfingFxChannel__VIDEO_SKIP
	void setup_ChannelFx();
	//ofParameter<bool> ENABLE_Video_FX;
	//ofParameter<bool> SHOW_Video_FX;
#endif

	//--

private:

	ofRectangle getBarRectangle() const;
	uint64_t tLastMovement;
	bool wasPaused;

	//--

	// auto hide
	bool ENABLE_GuiVisibleByAutoHide = false;
	ofParameter<bool> bGui_BarControl;
	void Changed_VideoBarControl(bool& b);
	uint64_t tAutoHide = 2500;
	bool inScrub;
	bool ENABLE_AutoHide_external = false;
	float scrubDragStart;
	float scrubDragDelta;

	//--

private:

	// video control bar size and margins
	//int BarInset = 15;
	//int BarHeight = 25;
	int BarHeight = BAR_HEIGHT;
	int BarInset = BAR_INSET;
	int BarAlpha = 128;
	float BarRounded = 3.0f;


public:

	//--------------------------------------------------------------
	void setBarInset(int _w)
	{
		BarInset = _w;
	}
	//--------------------------------------------------------------
	void setBarHeight(int _h)
	{
		BarHeight = _h;
	}

	//--

	// Transport

public:

	//--------------------------------------------------------------
	void goStart()
	{
		position = position_In;
	}

private:

	// Session params
	ofParameterGroup params_AppSettings;

	//--

	// To jump frames with keys
	void calculateKick();
	int numFramesToKick = 1; // size of frames seek
	float kickSizeFrame; // size of frames seek normalized to full video player normalized duration
	int totalNumFrames = 0;
	float frameSizeNorm = 0;

	//--

private:

	float positionSeconds;
	ofParameter<std::string> videoTIME;
	ofParameter<std::string> videoFRAME;

	ofParameter<bool> bDoKickL;
	ofParameter<bool> bDoKickR;

	ofParameter<float> position_In;
	ofParameter<float> position_Out;
	ofParameter<float> position;

	ofParameter<bool> bSet_In;
	ofParameter<bool> bSet_Out;

	ofParameter<bool> bGo_In;
	ofParameter<bool> bGo_Out;

	ofParameter<float> speed;
	ofParameter<float> speedNorm;

	ofParameter<bool> bPlay;

	ofParameter<bool> bMODE_Edit;
	ofParameter<bool> bMODE_Loop;
	ofParameter<bool> bMODE_LoopedBack;
	ofParameter<bool> bMODE_Reversed;
	ofParameter<bool> bMODE_SkipTime;
	ofParameter<bool> bMODE_SkipReverse;

	ofParameter<bool> bDoResetSpeed;
	ofParameter<bool> bDoSkipTime;
	ofParameter<bool> bDoSkipReverse;
	ofParameter<bool> bDoResetEngine;
	ofParameter<bool> bDoResetAll;
	ofParameter<bool> bDoResetBpm;

	ofParameter<bool> bAutoHide_BarControl;
	ofParameter<bool> bENABLE_TimersGlobal;

	ofParameter<bool> bMODE_Beat;
	ofParameter<bool> bMODE_Lock;
	ofParameter<int> beatDuration;
	ofParameter<int> beatRescale;

	ofParameter<bool> bMODE_SkipLooped;
	ofParameter<bool> bMODE_SkipPowered;
	ofParameter<float> skipPower;

#define USE_BPM_TIMER_MODE
#ifndef USE_BPM_TIMER_MODE
	ofParameter<int> timePeriod_skipper;
	ofParameter<int> timePeriod_reverser;
#else
	// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms
	ofParameter<float> bpm;
	ofParameter<int> bpmDivider;
	ofParameter<float> timer_SkipTime;
	ofParameter<float> timer_SkipRev;
	ofParameter<int> divBeatSkipper; // skipper
	ofParameter<int> divBeatReverse; // reverse
#endif

	//ofParameter<bool> bGui_Advanced;

	// Engine
	uint64_t last_TRIG_time = 0;
	uint64_t last_TRIG_reverse = 0;

	ofParameterGroup params_Engine;
	void Changed_Params(ofAbstractParameter& e);
	bool bDISABLE_CALLBACKS;

	ofParameterGroup params_Control;

#ifdef USE_ofxSurfingFxChannel__VIDEO_SKIP
	ofParameterGroup _param_ChannelFx{ "FX" };
#endif
#ifdef USE_ofxSurfingMoods__VIDEO_SKIP
	ofParameterGroup _param_MoodMachine{ "MOOD-MACHINE" };
#endif

	ofParameterGroup _param_Keys{ "KEYS" };
	ofParameterGroup _param_SkipEngine{ "TIMERS" };
	ofParameterGroup _param_Clock{ "CLOCK" };

	//--

	// Video file

	ofParameter<std::string> videoFilePath;
	ofParameter<std::string> videoName;

	void loadMovie(std::string movie);

	//--

	// Presets Manager

	ofParameterGroup params_Preset; // -> the params to create presets
	//ofParameterGroup params_Remote; // -> the params to control externally by user. 
	// i.e: to assign to OSC/midi control

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	void setup_PresetsManager();
	ofxPresetsManager presetsManager;
	void Changed_DONE_load(bool& DONE_load);
	void Changed_DONE_save(bool& DONE_save);
#endif

	//--

#ifdef USE_ofxSurfingMoods__VIDEO_SKIP
	ofxSurfingMoods moods;

	void Changed_Mood_RANGE(int& i);
	void Changed_Mood_TARGET(int& i); // listener for inside class surfingMoods
	void Changed_Mood_PRESET_A(int& i);
	void Changed_Mood_PRESET_B(int& i);
	void Changed_Mood_PRESET_C(int& i);
#endif

private:

	ofEventListener listener_bGui;
	void Changed_bGui();

	//ofParameter<int> MODE_App;
	//ofEventListener listener_MODE_App;
	//void Changed_MODE_App();

	// Settings
	std::string path_AppSettings;
};

