//
// Created by moebiusSurfing, 2019/2020/2021/2022.
//

#pragma once

#include "ofMain.h"

/*

	TODO:

	+ fix imgui layout presets


	+ add OSC control or control add-on mixing midi+osc

	+ should be oriented more to start point vs end/duration..

	+ improve Edit mode and lock
		+ when edit mode, and dragging start/end, move position

	+ add shift loop
		+ add control to move the loop clip with same duration?
		+ lock start/end then move "centered" on edit

	+ fix route paths for Windows/macOS

	+ full screen/dual window. external output.

	+ add tap tempo or surfingPlayer add-on

	+ add slow skip timers

	+ populate midi notes for available presets
		or make an engine to centralize all midi stuff for different modules.

	+ fix docking for all panels. Should split main 3 panels too..
		+ preview window and central docking layouting

	+ add new fx skip-engine jump bpm timed shutter

	+ zoom on loop zone. maybe with stills

	+ mark start points on bar


	BUGS:

	+ zero time loop wrong back...
	+ fine tunes: kicks working weird

*/


//----------------------------------------------
//
//	OPTIONAL DEFINES

//#define USE_MINIMAL_ofxSurfingVideoSkip
// -> Uncomment above line to 
// Force disable ALL the optional add-ons below. To use only the minimal core add-on.

#ifndef USE_MINIMAL_ofxSurfingVideoSkip

//--

// Optional compatible add-ons list:

// Presets. 
// Two alternatives:

// -> 1A. Simple Presets
#define USE_ofxSurfingPresets__VIDEO_SKIP //-> Recommended! for gui integration

// -> 1B. Power Presets
//#define USE_ofxPresetsManager__VIDEO_SKIP 

// -> 2. Mood Machine
#define USE_ofxSurfingMoods 

// -> 3. Beat Clock
//#define USE_OF_BEAT_CLOCK__VIDEO_SKIP

// -> 4. MIDI Input to control GUI and switch presets
//#define USE_MIDI_PARAMS__VIDEO_SKIP 

// -> 5. FX
//#define USE_ofxSurfingFxChannel 

// -> 6. Remote Parameters
//#define USE_ofxRemoteParameters 
//TODO:
// This add-on will server some parameters to allow remote control fast.
// Will serve OSC/MIDI

#endif

//----------------------------------------------

#include "ofxSurfingFxPro.h"

#include "ofxHapPlayer.h"
#include "ofxInteractiveRect.h"
#include "ofxSurfingImGui.h"
#include "ofxSurfing_ImGui_WindowFbo.h"

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
#include "ofxSurfingPresets.h"
#endif

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
#include "ofxPresetsManager.h"
#endif

#ifdef USE_ofxSurfingMoods
#include "ofxSurfingMoods.h"
#endif

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP 
#include "ofxMidiParams.h"
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
#include "ofxBeatClock.h"
#endif

#ifdef USE_ofxSurfingFxChannel
#include "ofxSurfingFxChannel.h"
#endif

#ifdef USE_ofxRemoteParameters
#include "ofxRemoteParameters/Server.h"
#endif

#define SPEED_MIN 0.20f
#define SPEED_MAX 50.0f


//--------------------------------------------------------------
class ofxSurfingVideoSkip
{
	//--

public:

	//--------------------------------------------------------------
	ofxSurfingVideoSkip()
	{
		ofAddListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);

		addMouseListeners();
	}

	//--------------------------------------------------------------
	~ofxSurfingVideoSkip()
	{
		ofRemoveListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		ofRemoveListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);

		removeMouseListeners();

		exit();
	}

	//--

public:

	void setup();

private:

	void setup_AppSettings();
	void setup_Remote();
	void setup_Preset();
	void setup_PresetsStuff();

	void update(ofEventArgs& args);
	void draw(ofEventArgs& args);

	void exit();

private:

	// Keys
	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs) {
		const int& key = eventArgs.key;
		fxPro.keyReleased(key);
	};
	void addKeysListeners();
	void removeKeysListeners();

	// Mouse
	void mouseDragged(ofMouseEventArgs& eventArgs);
	void mousePressed(ofMouseEventArgs& eventArgs);
	void mouseReleased(ofMouseEventArgs& eventArgs);
	void mouseMoved(ofMouseEventArgs& eventArgs);
	void mouseRefresh(int button, float position);
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

	ofxSurfingFxPro fxPro;

	//--

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
public:
	ofxSurfingPresets presetsManager;
#endif

	//--

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
private:
	ofxMidiParams mMidiParams;
#endif

	//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	ofxBeatClock beatClock;
	ofEventListener listenerBeat;
	ofEventListener listenerBpm;
	void Changed_BeatTick();
	void Changed_BeatBpm();
#endif

	//--

#ifdef USE_ofxRemoteParameters
	ofxRemoteParameters::Server remoteServer;
	ofParameterGroup params_Remote;
	bool bSyncRemote = false;
#endif

	//--

public:

	// GUI

	ofxSurfing_ImGui_Manager guiManager;

	void setup_ImGui();

	void draw_ImGui();
	void draw_ImGuiControls();
	void draw_ImGuiSkipTimers();

public:

	ofParameter<bool> bGui; // independent to auto hide state
	ofParameter<bool> bGui_SurfingVideo;
	ofParameter<bool> bGui_SkipTimers;
	ofParameter<bool> bGui_Previews;

	//--

private:

	// Preview Monitor
	SurfingPreview surfingPreview;

	std::vector<std::string> previewSources = { "Source", "Processed" };
	ofParameter<int> indexPreviewSource;

	//--

public:

	void draw_ImGuiPreview();

	void draw_ImGuiMenu();

private:

	bool bDockingReset = false;
	void dockingReset();

	//-

public:

	//--------------------------------------------------------------
	ofParameterGroup& getParams() {
		return params_Preset;
	}

	void doOpenDialogToSetPath();

private:

	void setPath_GlobalFolder(std::string folder);//path for root container folder
	std::string path_GLOBAL_Folder;//main folder where nested folder goes inside

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	std::string path_fileName_ChannelFX;
	std::string path_Preset;
#endif

	bool bActive = true;
	bool bLoaded = false;

	ofParameter<bool> bKeys{ "Keys", true };

#ifdef USE_ofxSurfingFxChannel
	ofxSurfingFxChannel channelFx;
#endif

	//----

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
		bAutoHideVideoBar = b;
		//lastMovement = ofGetSystemTimeMillis();
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

#ifdef USE_ofxSurfingMoods

	//--------------------------------------------------------------
	void setPlay_MoodMachine(bool b)
	{
		if (b) surfingMoods.play();
		else surfingMoods.stop();
	}

	//--------------------------------------------------------------
	bool isPlaying_MoodMachine()
	{
		return surfingMoods.isPlaying();
	}

	////--------------------------------------------------------------
	//bool get_MoodMachine_EditorMode()
	//{
	//	return SHOW_MoodMachine.get();
	//}

#endif

	//--

private:

#ifdef USE_ofxSurfingFxChannel
	void setup_ChannelFx();
	//ofParameter<bool> ENABLE_Video_FX;
	//ofParameter<bool> SHOW_Video_FX;
#endif

	//--

private:

	// Hap video player
	void loadMovie(std::string movie);
	ofRectangle getBarRectangle() const;
	ofxHapPlayer player;
	uint64_t lastMovement;
	bool wasPaused;

	//--

	// auto hide
	bool ENABLE_GuiVisibleByAutoHide = false;
	ofParameter<bool> bGui_VideoBarControl;
	bool bGui_VideoControlBar_PRE;
	//void Changed_draw_Autohide(bool &b);
	int time_autoHide = 2500;
	bool inScrub;
	bool ENABLE_AutoHide_external = false;

	//--

private:

	// video control bar size and margins
	int BarInset = 10;
	int BarHeight = 30;

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

	ofParameter<bool> bAutoHideVideoBar;
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
	ofParameter<float> bpmTimer;
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
	bool bDISABLECALLBACKS;

	ofParameterGroup params_Control;

#ifdef USE_ofxSurfingFxChannel
	ofParameterGroup _param_ChannelFx{ "FX" };
#endif
#ifdef USE_ofxSurfingMoods
	ofParameterGroup _param_MoodMachine{ "MOOD-MACHINE" };
#endif

	ofParameterGroup _param_Keys{ "KEYS" };
	ofParameterGroup _param_SkipEngine{ "TIMERS" };
	ofParameterGroup _param_Clock{ "CLOCK" };

	//-

	// Video file

	ofParameter<std::string> videoFilePath;
	ofParameter<std::string> videoName;

	//--

	// Presets Manager

	ofParameterGroup params_Preset; // -> the params to create presets
	//ofParameterGroup params_Remote; // -> the params to control externally by user. 
	// ie: to assign to OSC/midi control

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	void setup_PresetsManager();
	ofxPresetsManager presetsManager;
	void Changed_DONE_load(bool& DONE_load);
	void Changed_DONE_save(bool& DONE_save);
#endif

	//--

#ifdef USE_ofxSurfingMoods
	ofxSurfingMoods surfingMoods;

	void Changed_Mood_RANGE(int& targetVal);
	void Changed_Mood_TARGET(int& targetVal); // listener for inside class surfingMoods
	void Changed_Mood_PRESET_A(int& targetVal);
	void Changed_Mood_PRESET_B(int& targetVal);
	void Changed_Mood_PRESET_C(int& targetVal);
#endif

private:

	ofEventListener listener_SHOW_gui;
	void Changed_bGui();

	//ofParameter<int> MODE_App;
	//ofEventListener listener_MODE_App;
	//void Changed_MODE_App();

	// Settings
	std::string path_AppSettings;
};

