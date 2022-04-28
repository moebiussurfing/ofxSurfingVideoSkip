//
// Created by moebiusSurfing, 2019/2020/2021.
//

#pragma once

#include "ofMain.h"

/*

	TODO:

	+ add preset retrig when same preset index

	+ add OSC control or control add-on mixing midi+osc

	+ should be oriented more to start point vs end/duration..

	+ add/search disable widgets

	+ improve Edit mode and lock
	
	+ fix imgui layout windows: store settings, hide/disable modes

	+ add locker for duration locked (opened/without ending) to bpm ? 
		+ could be better to link bpm to skippers only, not to loop duration. easy to user
		+ add control to move the loop clip with same duration

	+ add shift loop
		+ lock start/end then move "centered" on edit

	+ when edit mode, and dragging start/end, move position
	+ start / end buttons with ctrl to go/set

	+ fix presets path empty on startup
		+ fix route paths for Windows/macOS

	+ full screen/dual window. external output.

	+ add tap tempo or surfingPlayer add-on
		+ add slow skip timers

	+ populate midi notes for available presets
		or make an engine to centralize all midi stuff for different modules.

	+ fix docking for all panels. Should split main 3 panels too..

	+ add new fx skip-engine jump bpm timed shutter

	+ zoom on loop zone. maybe with stills


	BUGS:

	 zero time loop wrong back...
*/


//----------------------------------------------
//
//	OPTIONAL DEFINES

//#define USE_MINIMAL_ofxSurfingVideoSkip
// -> Uncomment above line to 
// Force disable ALL the optional add-ons below. To use only the minimal core add-on.

#ifndef USE_MINIMAL_ofxSurfingVideoSkip

//-

// Optional compatible add-ons list:

// Presets. Two alternatives:
// -> 1A. simple presets
#define USE_ofxSurfingPresets__VIDEO_SKIP
// -> 1B. power presets
//#define USE_ofxPresetsManager__VIDEO_SKIP 

// -> 2. Mood machine
//#define USE_ofxSurfingMoods 

// -> 3. Beat clock
//#define USE_OF_BEAT_CLOCK__VIDEO_SKIP

// -> 4. MIDI input to control gui and switch presets
//#define USE_MIDI_PARAMS__VIDEO_SKIP 

// -> 5. FX
#define USE_ofxSurfingFxChannel 

#endif

//----------------------------------------------


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


//--------------------------------------------------------------
class ofxSurfingVideoSkip
{
	//--

public:
	SurfingPreview surfingPreview;
	void draw_ImGuiPreview();

	//--

	//--------------------------------------------------------------
	ofxSurfingVideoSkip()
	{
		//ofAddListener(ofEvents().setup, this, &ofxSurfingVideoSkip::setup);
		ofAddListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);
		//ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_AFTER_APP);

		addMouseListeners();

		//setup();
	};

	//--------------------------------------------------------------
	~ofxSurfingVideoSkip()
	{
		//ofRemoveListener(ofEvents().setup, this, &ofxSurfingVideoSkip::setup);
		ofRemoveListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		ofRemoveListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);
		//ofRemoveListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_AFTER_APP);		

		removeMouseListeners();

		exit();
	};

	//--

public:

	void setup();

	//void update();
	//void draw();

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	void exit();

private:

	// keys
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs) {};
	void addKeysListeners();
	void removeKeysListeners();

	// mouse
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void mouseMoved(ofMouseEventArgs &eventArgs);

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
	void draw_VideoControls();

	//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
public:

	ofxSurfingPresets presetsManager;
#endif

	//-

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
private:
	ofxMidiParams mMidiParams;
#endif

	//-

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	ofxBeatClock beatClock;	
	ofEventListener listenerBeat;
	ofEventListener listenerBpm;
	void Changed_BeatTick();
	void Changed_BeatBpm();
#endif

	//-

public:

	// gui

	ofxSurfing_ImGui_Manager guiManager;

	void draw_ImGuiMenu();
	bool bDockingReset = false;
	void dockingReset();
	//void dockingPopulate();

	void setup_ImGui();

	void draw_ImGui();
	void draw_ImGuiControls();
	void draw_ImGuiSkipTimers();

private:

	bool bOpen0 = true;
	bool bOpen1 = true;
	bool bOpen2 = true;

	//-

public:

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
	//ofParameter<bool> bKeys_Presets{ "Keys Presets", false };
	//ofParameter<bool> bKeys_Fx{ "Keys Fx", false };

#ifdef USE_ofxSurfingFxChannel
	ofxSurfingFxChannel channelFx;
#endif

	//----

public:

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//--------------------------------------------------------------
	void loadPreset(int p)
	{
		presetsManager.loadPreset(p);
	}
#endif

	void setActive(bool b);
	void setKeysEnable(bool b);
	void setGuiVisible(bool b);
	//--------------------------------------------------------------
	void setGuiToggleVisible() {
		bGui = !bGui;
	}
	//--------------------------------------------------------------
	void setUserVisible(bool b)
	{
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
		// presets
		presetsManager.setVisible_PresetClicker(b);
		presetsManager.setEnableKeys(b);
#endif
	}

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
		bAutoHide = b;
		//lastMovement = ofGetSystemTimeMillis();
	}

	//--------------------------------------------------------------
	//bool isPlaying_MoodMachine()
	//{
	//	return surfingMoods.isPlaying();
	//}
	//--------------------------------------------------------------
	void play()
	{
		bPlay = true;
	}
	//--------------------------------------------------------------
	void stop()
	{
		bPlay = false;
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

#ifdef USE_ofxSurfingMoods
	//--------------------------------------------------------------
	void setPlay_MoodMachine(bool b)
	{
		if (b)
			surfingMoods.play();
		else
			surfingMoods.stop();
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

private:

	// hap video player
	void loadMovie(std::string movie);
	ofRectangle getBarRectangle() const;
	ofxHapPlayer player;
	uint64_t lastMovement;
	bool wasPaused;

	// autohide
	bool ENABLE_GuiVisibleByAutoHide = false;
	ofParameter<bool> bGui_VideoControlBar;
	bool bGui_VideoControlBar_PRE;
	//void Changed_draw_Autohide(bool &b);
	int time_autoHide = 2500;
	bool inScrub;
	bool ENABLE_AutoHide_external = false;

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

	//-

	// transport

public:

	//--------------------------------------------------------------
	void goStart()
	{
		POSITION = POSITION_Start;
	}

private:

	// preset params
	ofParameterGroup params_AppSettings;

	//--

	// to jumps frames with keys
	void calculateKick();
	int numFramesToKick = 1;//size of frames seek
	float kickSizeFrame;//size of frames seek normalized to full video player normalized duration
	int totalNumFrames = 0;
	float frameSizeNorm = 0;

	//-

	// labels
	std::string myTTF;// gui font for all gui theme
	int sizeTTF;
	std::string filename_Font;

	//--

private:

	float positionSeconds;
	ofParameter<std::string> videoTIME;
	ofParameter<std::string> videoFRAME;

	ofParameter<bool> bKickL;
	ofParameter<bool> bKickR;

	ofParameter<float> POSITION_Start;
	ofParameter<float> POSITION_End;
	ofParameter<float> POSITION;

	ofParameter<bool> bSET_START;
	ofParameter<bool> bSET_END;

	ofParameter<bool> bGo_START;
	ofParameter<bool> bGo_END;

	ofParameter<float> speed;
	ofParameter<float> speedNorm;
	
	ofParameter<bool> bPlay;
	
	ofParameter<bool> bMODE_Edit;
	ofParameter<bool> bMODE_Loop;
	ofParameter<bool> bMODE_LoopedBack;
	ofParameter<bool> bMODE_Reversed;
	ofParameter<bool> bMODE_SkipTime;
	ofParameter<bool> bMODE_SkipReverse;

	ofParameter<bool> bReset_Speed;
	ofParameter<bool> bTRIG_SkipTime;
	ofParameter<bool> bTRIG_SkipReverse;
	ofParameter<bool> bTRIG_ResetEngine;
	ofParameter<bool> bTRIG_ResetAll;
	ofParameter<bool> bTRIG_Reset_Bpm;

	ofParameter<bool> bAutoHide;
	ofParameter<bool> bENABLE_TimersGlobal;

	ofParameter<bool> bMODE_Beat;
	ofParameter<bool> bMODE_Lock;
	ofParameter<int> beatDuration;
	ofParameter<int> beatRescale;

	//TODO:
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
	ofParameter<int> divBeatSkipper;//skiper
	ofParameter<int> divBeatReverse;//reverse
#endif

	ofParameter<bool> bGui_Presets;
	//ofParameter<bool> SHOW_MoodMachine;
	ofParameter<bool> bGui_Advanced;

	// engine
	uint64_t last_TRIG_time = 0;
	uint64_t last_TRIG_reverse = 0;

	ofParameterGroup params_Engine;
	void Changed_Params(ofAbstractParameter &e);
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

	// video file

	ofParameter<std::string> videoFilePath;
	ofParameter<std::string> videoName;

	//--

	// presetsManager

	ofParameterGroup params_Preset; // -> the params to create presets
	ofParameterGroup params_ControlRemote; // -> the params to control externally by user. ie: to assign to OSC/midi control

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	void setup_PresetsManager();
	ofxPresetsManager presetsManager;
	void Changed_DONE_load(bool &DONE_load);
	void Changed_DONE_save(bool &DONE_save);
#endif

	//--

#ifdef USE_ofxSurfingMoods
	ofxSurfingMoods surfingMoods;

	void Changed_Mood_RANGE(int & targetVal);
	void Changed_Mood_TARGET(int &targetVal);//listener for inside class surfingMoods
	void Changed_Mood_PRESET_A(int &targetVal);
	void Changed_Mood_PRESET_B(int &targetVal);
	void Changed_Mood_PRESET_C(int &targetVal);
#endif

	ofParameter<bool> bGui_SurfingVideo;
	ofParameter<bool> bGui_SkipTimers;

	ofParameter<bool> bGui{ "SURFING VIDEO", true };//independent to autohide state

	ofEventListener listener_SHOW_gui;
	void Changed_bGui();

	//ofParameter<int> MODE_App;
	//ofEventListener listener_MODE_App;
	//void Changed_MODE_App();

	//settings
	std::string path_AppSettings;
	void saveGroup(ofParameterGroup &g, std::string path);
	void loadGroup(ofParameterGroup &g, std::string path);

	//ofParameter<int> window_W, window_H, window_X, window_Y;

	//--

private:

	// check if a folder path exist and creates one if not
	// many times when you try to save a file, this is not possible and do not happens bc the container folder do not exist
	//--------------------------------------------------------------
	void CheckFolder(std::string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		ofDirectory dataDirectory(ofToDataPath(_path, true));

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}

	//--

	float _w100;
	float _w50;
	float _h = WIDGETS_HEIGHT;
	//--------------------------------------------------------------
	void refreshLayout() {
		// update sizes to current window shape
		guiManager.refreshLayout();
		_w100 = ofxImGuiSurfing::getWidgetsWidth(1);
		_w50 = ofxImGuiSurfing::getWidgetsWidth(2);
	}

	};

