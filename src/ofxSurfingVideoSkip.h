//
// Created by moebiusSurfing, 2019/2020/2021.
//

#pragma once

#include "ofMain.h"


/*

	TODO:

	+ add slow skip timers
	+ add jumpt bpm timed stutter
	+ handle different videos/projects:
	video file & presets files path
	duplicate project, undo

	+ add locker for duration locked to bpm ? 
	could be better to link bpm to skippers only, not to loop duration. easy to user

*/


//----------------------------------------------
//
//	OPTIONAL DEFINES

#define USE_ofxSurfingMoods // -> mood machine

#define USE_OF_BEAT_CLOCK__VIDEO_SKIP // -> beat clock

#define USE_ofxSurfingPresets__VIDEO_SKIP // -> simple presets
//#define USE_ofxPresetsManager__VIDEO_SKIP // -> power presets

//#define USE_MIDI_PARAMS__VIDEO_SKIP // -> midi input to control gui and switch presets

//#define USE_ofxChannelFx // fx

//#define USE_ofxGuiExtended // -> deprectaed

//----------------------------------------------


#include "ofxSurfingImGui.h"

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP 
#include "ofxMidiParams.h"
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
#include "ofxBeatClock.h"
#endif

#ifdef USE_ofxChannelFx
#include "ofxChannelFx.h"
#endif

#ifdef USE_ofxSurfingMoods
#include "ofxSurfingMoods.h"
#endif

#ifdef USE_ofxGuiExtended
#include "ofxGuiExtended2.h"
#endif

#include "ofxHapPlayer.h"

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
#include "ofxSurfingPresets.h"
#endif

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
#include "ofxPresetsManager.h"
#endif

//#define USE_ofxGui // not implemented
//#ifdef USE_ofxGui
//#include "ofxGui.h"
//#endif

class ofxSurfingVideoSkip
{
	//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
public:

	ofxSurfingPresets presets;
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

	void setup_ImGui();

	void draw_ImGui();
	void draw_ImGuiPanels();
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

	void setPathOpenDialog();

private:

	void setPath_GlobalFolder(std::string folder);//path for root container folder
	std::string path_GLOBAL_Folder;//main folder where nested folder goes inside
#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	std::string path_fileName_ChannelFX;
	std::string path_Preset;
#endif

	bool ENABLE_Active = true;

	ofParameter<bool> ENABLE_Keys_Player{ "Keys", true };
	ofParameter<bool> ENABLE_Keys_Presets{ "ENABLE KEYS PRESETS", false };
	ofParameter<bool> ENABLE_Keys_Fx{ "ENABLE KEYS FX", false };

#ifdef USE_ofxChannelFx
	ofxChannelFx channelFx;
#endif

public:

	//--------------------------------------------------------------
	ofxSurfingVideoSkip() {
		//ofAddListener(ofEvents().setup, this, &ofxSurfingVideoSkip::setup);
		ofAddListener(ofEvents().update, this, &ofxSurfingVideoSkip::update);
		ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_APP);
		//ofAddListener(ofEvents().draw, this, &ofxSurfingVideoSkip::draw, OF_EVENT_ORDER_AFTER_APP);

		addMouseListeners();

		setup();
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

	//--------------------------------------------------------------
	void setMODE_App(int m)
	{
		MODE_App = m;
	}

	// auto hide controlled outside the class. internal by default
	//--------------------------------------------------------------
	void setAudoHideExternal(bool b)
	{
		ENABLE_AutoHide_external = b;
	}

	//--------------------------------------------------------------
	void setAudoHide(bool b)
	{
		ENABLE_AutoHide = b;
		//lastMovement = ofGetSystemTimeMillis();
	}

	//--------------------------------------------------------------
	//bool isPlaying_MoodMachine()
	//{
	//	return moodsSurfer.isPlaying();
	//}
	//--------------------------------------------------------------
	void play()
	{
		PLAYING = true;
	}
	//--------------------------------------------------------------
	void stop()
	{
		PLAYING = false;
	}

	//--------------------------------------------------------------
	void setPlay(bool b)
	{
		PLAYING = b;
	}
	//--------------------------------------------------------------
	void setTogglePlay()
	{
		PLAYING = !PLAYING;
	}
	//--------------------------------------------------------------
	bool isPlaying()
	{
		return PLAYING;
	}

#ifdef USE_ofxSurfingMoods
	//--------------------------------------------------------------
	void setPlay_MoodMachine(bool b)
	{
		if (b)
			moodsSurfer.play();
		else
			moodsSurfer.stop();
	}

	//--------------------------------------------------------------
	bool isPlaying_MoodMachine()
	{
		return moodsSurfer.isPlaying();
	}

	////--------------------------------------------------------------
	//bool get_MoodMachine_EditorMode()
	//{
	//	return SHOW_MoodMachine.get();
	//}
#endif

	//--

private:

#ifdef USE_ofxChannelFx
	void setup_ChannelFx();
	ofParameter<bool> ENABLE_Video_FX;
	ofParameter<bool> SHOW_Video_FX;
#endif

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
	void removeKeysListeners() ;

	// mouse
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void mouseMoved(ofMouseEventArgs &eventArgs);
	
	void refreshMouse(int button, float position);

	void addMouseListeners();
	void removeMouseListeners();

public:

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

private:

	void startup();
	void updateVideoPLayer();
	void updateTimers();

public:

	void draw_Gui();
	void draw_Video();
	void draw_VideoControls();

private:

	// hap video player
	void loadMovie(std::string movie);
	ofRectangle getBarRectangle() const;
	ofxHapPlayer player;
	uint64_t lastMovement;
	bool wasPaused;

	// autohide
	bool ENABLE_GuiVisibleByAutoHide = false;
	ofParameter<bool> ENABLE_drawVideoControlBar;
	bool draw_Autohide_PRE;
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

	//to jumps frames with keys
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

	//skipper engine
	float positionSeconds;
	ofParameter<std::string> videoTIME;
	ofParameter<std::string> videoFRAME;

	ofParameter<bool> bKickL;
	ofParameter<bool> bKickR;

	ofParameter<bool> PLAYING;
	ofParameter<bool> MODE_EDIT;
	ofParameter<bool> MODE_LOOP;
	ofParameter<bool> loopedBack;
	ofParameter<float> POSITION_Start;
	ofParameter<float> POSITION_End;
	ofParameter<float> POSITION;
	ofParameter<bool> bSET_START;
	ofParameter<bool> bSET_END;
	ofParameter<float> speed;
	ofParameter<float> speedNorm;
	ofParameter<bool> speed_Reset;
	ofParameter<bool> ENABLE_AutoHide;
	ofParameter<bool> ENABLE_TimersSkipRev;
	ofParameter<bool> TRIG_time_Skiper;
	ofParameter<bool> TRIG_bReverseSkipper;
	ofParameter<bool> reverseSpeed;
	ofParameter<bool> MODE_SkipTime;
	ofParameter<bool> MODE_SkipReverse;
	ofParameter<bool> TRIG_bResetEngine;
	ofParameter<bool> TRIG_Reset_Bpm;

#define USE_BPM_TIMER_MODE
#ifndef USE_BPM_TIMER_MODE
	ofParameter<int> timePeriod_skipper;
	ofParameter<int> timePeriod_reverser;
#else
	//60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms
	ofParameter<float> bpmTimer;
	ofParameter<int> bpmDivider;
	ofParameter<float> timer_SkipTime;
	ofParameter<float> timer_SkipRev;
	ofParameter<int> divBeatSkipper;//skiper
	ofParameter<int> divBeatReverse;//reverse
#endif
	ofParameter<bool> SHOW_Presets;
	//ofParameter<bool> SHOW_MoodMachine;
	ofParameter<bool> SHOW_Advanced;

	// engine
	uint64_t last_TRIG_time = 0;
	uint64_t last_TRIG_reverse = 0;

	ofParameterGroup params_Engine;
	void Changed_Params(ofAbstractParameter &e);
	bool DISABLE_CALLBACKS;

	ofParameterGroup params_Control;
	ofParameter<bool> SHOW_ControlPanel;
	ofParameter<bool> SHOW_ControlPanel_Header;
	ofParameter<glm::vec2> positionGui_ControlPanel;
	ofParameter<glm::vec2> positionGui_Engine;

#ifdef USE_ofxChannelFx
	ofParameterGroup _param_ChannelFx{ "FX" };
#endif
#ifdef USE_ofxSurfingMoods
	ofParameterGroup _param_MoodMachine{ "MOOD-MACHINE" };
#endif

	ofParameterGroup _param_Keys{ "KEYS" };
	ofParameterGroup _param_SkipEngine{ "TIMERS" };
	ofParameterGroup _param_Clock{ "CLOCK" };

	//-

	//video
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
	ofxSurfingMoods moodsSurfer;

	void Changed_Mood_RANGE(int & targetVal);
	void Changed_Mood_TARGET(int &targetVal);//listener for inside class moodsSurfer
	void Changed_Mood_PRESET_A(int &targetVal);
	void Changed_Mood_PRESET_B(int &targetVal);
	void Changed_Mood_PRESET_C(int &targetVal);
#endif

	ofParameter<bool> bGui_SurfingVideo;
	ofParameter<bool> bGui_SkipTimers;

	ofParameter<bool> bGui{ "SURFING VIDEO", true };//independent to autohide state

	ofEventListener listener_SHOW_gui;
	void Changed_bGui();

	ofParameter<int> MODE_App;
	ofEventListener listener_MODE_App;
	void Changed_MODE_App();

	//settings
	std::string path_AppSettings;
	void saveGroup(ofParameterGroup &g, std::string path);
	void loadGroup(ofParameterGroup &g, std::string path);

	ofParameter<int> window_W, window_H, window_X, window_Y;

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

#ifdef USE_ofxGuiExtended

private:

	//gui
	ofxGui gui;
	ofxGuiPanel *panel_Engine;
	//ofxGuiGroup2 *panel_Engine;
	ofxGuiPanel *panel_Control;

private:

	//customize guiExtended
	void gui_CustomizeDefine();
	void gui_CustomizeApply();
	bool bLoadedGuiFont = false;
	ofJson jConf_BigBut1;//center text
	ofJson jConf_BigBut2;//highlighted
	ofJson jConf_BigBut3;//play
	ofJson jConf_Labl;//labels
	//ofJson jConf_Highligthed;//highlighted color
	ofJson jConf_Labl_Hide;//labels

public:

	//--------------------------------------------------------------
	glm::vec2 getGuiPosition()
	{
		glm::vec2 gPos = glm::vec2(panel_Engine->getPosition().x, panel_Engine->getPosition().y);
		return gPos;
	}
	//--------------------------------------------------------------
	float getGuiWidth()
	{
		float gwidth = panel_Engine->getWidth();
		return gwidth;
	}

private:

	std::string path_Theme;

public:

	//--------------------------------------------------------------
	void loadTheme(std::string _path) {
		path_Theme = _path;
		ofLogNotice(__FUNCTION__) << path_Theme;
		panel_Control->loadTheme(path_Theme);
		//crash!
		panel_Engine->loadTheme(path_Theme);
		}

#endif

	float _w100;
	float _w50;
	float _h = WIDGETS_HEIGHT;
	void refreshLayout() {
		// update sizes to current window shape
		widgetsManager.refreshPanelShape();
		_w100 = ofxImGuiSurfing::getWidgetsWidth(1);
		_w50 = ofxImGuiSurfing::getWidgetsWidth(2);
	}

	};

