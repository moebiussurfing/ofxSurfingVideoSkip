#include "ofxSurfingVideoSkip.h"
//--------------------------------------------------------------
void ofxSurfingVideoSkip::addMouseListeners()
{
	ofAddListener(ofEvents().mouseMoved, this, &ofxSurfingVideoSkip::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxSurfingVideoSkip::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxSurfingVideoSkip::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxSurfingVideoSkip::mouseReleased);
}
//--------------------------------------------------------------
void ofxSurfingVideoSkip::removeMouseListeners()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxSurfingVideoSkip::mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxSurfingVideoSkip::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxSurfingVideoSkip::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxSurfingVideoSkip::mouseReleased);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingVideoSkip::keyPressed);
}
//--------------------------------------------------------------
void ofxSurfingVideoSkip::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingVideoSkip::keyPressed);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup()
{
	DISABLE_CALLBACKS = true;

	//settings folder
	setPath_GlobalFolder("ofxSurfingVideoSkip");

	path_AppSettings = "ofxSurfingVideoSkip_App_Settings.xml";

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	path_Preset = "ofxSurfingVideoSkip_Engine_Settings.xml";
#ifdef USE_ofxChannelFx
	path_fileName_ChannelFX = "ofxSurfingVideoSkip_ChannelFX_Settings.xml";
#endif
#endif

	//-

	window_W = ofGetWidth();
	window_H = ofGetHeight();

	positionGui_ControlPanel.set("Gui Panel Position", glm::vec2(500, 500), glm::vec2(0), glm::vec2(1920, 1080));
	positionGui_Engine.set("Gui Engine Position", glm::vec2(700, 500), glm::vec2(0), glm::vec2(1920, 1080));

	//--

	inScrub = false;

	ENABLE_drawVideoControlBar.set("ENABLE_drawVideoControlBar", true);//var used to hide/show player bar and gui also
	//ENABLE_drawVideoControlBar.addListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//--

	//skipper engine
	videoFilePath.set("videoFilePath", "NO FILE");
	videoName.set("FILE", "NO FILE");
	videoTIME.set("", ""); // current time position

	bGui_SkipTimers.set("SKIP TIMERS", true);

	SHOW_Presets.set("SHOW PRESETS", true);

	ENABLE_AutoHide.set("AutoHide Bar", true);

	bKickL.set("FRAME L", false);
	bKickR.set("FRAME R", false);

	PLAYING.set("PLAY", false);
	MODE_EDIT.set("EDIT", false);
	MODE_LOOP.set("LOOP", false);

	speedNorm.set("SPEED", 0.0f, -1.0f, 1.0f);
#define SPEED_MIN 0.20f
#define SPEED_MAX 50.0f
	speed.set("SPEED_", 1.0f, SPEED_MIN, SPEED_MAX);
	speed_Reset.set("RESET SPEED", false);

	loopedBack.set("LOOPED BACK", false);
	reverseSpeed.set("REVERSE", false);
	TRIG_bResetEngine.set("RESET ENGINE", false);
	TRIG_Reset_Bpm.set("RESET BPM", false);
	SHOW_Advanced.set("SHOW ADVANCED", false);

	POSITION_Start.set("START", 0.0f, 0.0f, 1.0f);
	POSITION_End.set("END", 1.0f, 0.0f, 1.0f);
	POSITION.set("POSITION", 0.0f, 0.0f, 1.0f);
	bSET_START.set("SET START", false);
	bSET_END.set("SET END", false);
	ENABLE_TimersSkipRev.set("ENABLE TIMERS", false);
	TRIG_time_Skiper.set("TRIG SKIP", false);
	TRIG_bReverseSkipper.set("TRIG REV", false);
	MODE_SkipTime.set("A MODE SKIP", false);
	MODE_SkipReverse.set("B MODE REV", false);

#ifndef USE_BPM_TIMER_MODE
	timePeriod_skipper.set("SKIP-TIME TIME", 1000, 1, 4000);
	timePeriod_reverser.set("SKIP-REVERSE TIME", 1000, 1, 4000);
#else

	// these are global. will not be included into presets
	bpmTimer.set("BPM", 120.0f, 40.f, 400.0f);
	bpmDivider.set("DIV", 2, 1, 8);

	timer_SkipTime.set("_timer1_", 0, 0, 1);
	timer_SkipRev.set("_timer2_", 0, 0, 1);

	divBeatSkipper.set("T SKIP", 4, 1, 8);
	divBeatReverse.set("T REV", 2, 1, 8);
#endif

	last_TRIG_time = 0;
	last_TRIG_reverse = 0;
	MODE_EDIT.setSerializable(false);
	speed.setSerializable(false);
	speed_Reset.setSerializable(false);
	TRIG_bResetEngine.setSerializable(false);
	TRIG_Reset_Bpm.setSerializable(false);
	timer_SkipTime.setSerializable(false);
	timer_SkipRev.setSerializable(false);

	bKickL.setSerializable(false);
	bKickR.setSerializable(false);

	//modes. not included into presets

#ifdef USE_ofxChannelFx
	ENABLE_Video_FX.set("ENABLE FX", false);
	SHOW_Video_FX.set("SHOW FX", false);
#endif

	SHOW_ControlPanel.set("SHOW CONTROL PANEL", true);
	SHOW_ControlPanel_Header.set("SHOW HEADER", true);

	//#ifdef USE_ofxSurfingMoods
	//	SHOW_MoodMachine.set("SHOW MOOD-MACHINE", true);
	//#endif

	bGui_SurfingVideo.set("SKIP PLAYER", true);

	//ofParameter<std::string> label_FX{ "FX", "" };

	//--

	//video controls
	params_Engine.setName("SKIP PLAYER");

	params_Engine.add(bGui_SkipTimers);

	params_Engine.add(PLAYING);
	params_Engine.add(MODE_EDIT);
	params_Engine.add(videoName);//NOTE: a longer string will resize the gui panel width!
	params_Engine.add(POSITION);
	//ofParameter<std::string> _labelName_{ "NAME"," " };
	//params_Engine.add(_labelName_);
	params_Engine.add(videoTIME);

	//-

	//for presets settings

	ofParameter<std::string> label_Engine{ "ENGINE", "" };
	params_Engine.add(label_Engine);

	params_Engine.add(speedNorm);
	params_Engine.add(speed);
	params_Engine.add(speed_Reset);

	params_Engine.add(MODE_LOOP);
	params_Engine.add(bSET_START);
	params_Engine.add(bSET_END);
	params_Engine.add(POSITION_Start);
	params_Engine.add(POSITION_End);

	params_Engine.add(loopedBack);
	params_Engine.add(reverseSpeed);

	params_Engine.add(bKickL);
	params_Engine.add(bKickR);

	//----

	// skip engine

	_param_SkipEngine.add(ENABLE_TimersSkipRev);

	//-

	// spacers
	//ofParameter<std::string> labelSpacer1{ "_spacer1_", "" };
	//_param_SkipEngine.add(labelSpacer1);
	//ofParameter<std::string> labelSpacer2{ "_spacer2_", "" };
	//_param_SkipEngine.add(labelSpacer2);

	//-

	// skippers

	// time

	_param_SkipEngine.add(MODE_SkipTime);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_skipper);
#else
	_param_SkipEngine.add(divBeatSkipper);
#endif
	_param_SkipEngine.add(timer_SkipTime);
	_param_SkipEngine.add(TRIG_time_Skiper);

	//-

	// reverse

	_param_SkipEngine.add(MODE_SkipReverse);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_reverser);
#else
	_param_SkipEngine.add(divBeatReverse);
#endif
	_param_SkipEngine.add(timer_SkipRev);
	_param_SkipEngine.add(TRIG_bReverseSkipper);

	_param_SkipEngine.add(TRIG_bResetEngine);

#ifdef USE_BPM_TIMER_MODE
	_param_Clock.add(bpmTimer);
	_param_Clock.add(bpmDivider);
#endif
	_param_Clock.add(TRIG_Reset_Bpm);
	_param_SkipEngine.add(_param_Clock);

	//_param_SkipEngine.add(SHOW_Advanced);

	params_Engine.add(_param_SkipEngine);

	//-

	ofAddListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	//this params are not stored. just from gui panels here

	//--

	// presetsManager 

	//params to save
	//name will be use to prefix file names at /group folder
	params_Preset.setName("Clips");
	//params_Preset.setName("ofxSurfingVideoSkip");
	params_Preset.add(POSITION_Start);
	params_Preset.add(POSITION_End);

	params_Preset.add(speed_Reset);
	params_Preset.add(speedNorm);
	//params_Preset.add(speed);

	params_Preset.add(loopedBack);
	params_Preset.add(reverseSpeed);
	params_Preset.add(ENABLE_TimersSkipRev);

#ifndef USE_BPM_TIMER_MODE
	params_Preset.add(timePeriod_skipper);
	params_Preset.add(timePeriod_reverser);

	params_Preset.add(MODE_SkipTime);
	params_Preset.add(divBeatSkipper);
	params_Preset.add(MODE_SkipReverse);
	params_Preset.add(divBeatReverse);
#else

	//// exclude
	//params_Preset.add(bpmTimer);
	//params_Preset.add(bpmDivider);

	params_Preset.add(MODE_SkipTime);
	params_Preset.add(divBeatSkipper);
	params_Preset.add(MODE_SkipReverse);
	params_Preset.add(divBeatReverse);
#endif

	//should add more params like direction/reverse/..

	//--

	//gui

	//-

#ifdef USE_ofxGuiExtended
	//ofxGuiExtended
	gui_CustomizeDefine();

	//-

	//panel engine
	panel_Engine = gui.addPanel(params_Engine);
	//panel_Engine = gui.addGroup(params_Engine);
#endif

	//----

	//main gui panel params

	params_Control.setName("SURFING VIDEO SKIP");

	//params_Control.add(SHOW_ControlPanel);//TODO: same than SHOW_GUI ?

	params_Control.add(bGui_SurfingVideo);
	params_Control.add(bGui_SkipTimers);
	params_Control.add(bGui);
	//params_Control.add(MODE_EDIT);

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	params_Control.add(SHOW_Presets);
#endif

#ifdef USE_ofxChannelFx
	_param_ChannelFx.add(SHOW_Video_FX);
	_param_ChannelFx.add(ENABLE_Video_FX);
#endif

	//#ifdef USE_ofxSurfingMoods
	//	_param_MoodMachine.add(SHOW_MoodMachine);
	//#endif

	_param_Keys.add(ENABLE_Keys_Player);

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	_param_Keys.add(ENABLE_Keys_Presets);
#endif

#ifdef USE_ofxChannelFx
	_param_Keys.add(ENABLE_Keys_Fx);
#endif

#ifdef USE_ofxChannelFx
	params_Control.add(_param_ChannelFx);
#endif

#ifdef USE_ofxSurfingMoods
	params_Control.add(_param_MoodMachine);
#endif
	params_Control.add(_param_Keys);
	params_Control.add(ENABLE_AutoHide);
	params_Control.add(SHOW_ControlPanel_Header);

#ifdef USE_ofxGuiExtended
	panel_Control = gui.addPanel(params_Control);
#endif

	//--

	// channel fx
#ifdef USE_ofxChannelFx
	setup_ChannelFx();
#endif

	//--

#ifdef USE_ofxSurfingMoods
	moodsSurfer.setup();

	// customize
	//moodsSurfer.setGui_Visible(SHOW_MoodMachine);
	//moodsSurfer.setGui_AdvancedVertical_MODE(true);

	// listeners for inside class moodsSurfer
	moodsSurfer.RANGE_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	moodsSurfer.TARGET_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	moodsSurfer.PRESET_A_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	moodsSurfer.PRESET_B_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	moodsSurfer.PRESET_C_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);

	//moodsSurfer.setTickMode(true);
#endif

	//----

	// app settings

	// main app parameters to store recall on exit and start app
	MODE_App.set("MODE_App", 0, 0, 1);

	//-

	// external control
	// to link to OSC/midi control
	params_ControlRemote.setName("Remote Control");
	params_ControlRemote.add(PLAYING);
	params_ControlRemote.add(MODE_EDIT);
	params_ControlRemote.add(bKickL);
	params_ControlRemote.add(bKickR);
	params_ControlRemote.add(bSET_START);
	params_ControlRemote.add(bSET_END);
	params_ControlRemote.add(POSITION);

	// app settings to handle store/recall only
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(PLAYING);
	params_AppSettings.add(MODE_LOOP);
	params_AppSettings.add(videoName);
	params_AppSettings.add(videoFilePath);
	params_AppSettings.add(MODE_App);
	params_AppSettings.add(SHOW_Presets);
	params_AppSettings.add(ENABLE_Keys_Player);
	params_AppSettings.add(bGui_SkipTimers);
	//params_AppSettings.add(bGui);

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	params_AppSettings.add(ENABLE_Keys_Presets);
#endif

#ifdef USE_ofxChannelFx
	params_AppSettings.add(ENABLE_Keys_Fx);
#endif

	params_AppSettings.add(SHOW_ControlPanel);
	params_AppSettings.add(SHOW_ControlPanel_Header);
	params_AppSettings.add(ENABLE_AutoHide);

#ifdef USE_ofxChannelFx
	params_AppSettings.add(ENABLE_Video_FX);
	params_AppSettings.add(SHOW_Video_FX);
#endif

	//#ifdef USE_ofxSurfingMoods
	//	params_AppSettings.add(SHOW_MoodMachine);
	//#endif

	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_SurfingVideo);

	params_AppSettings.add(positionGui_ControlPanel);
	params_AppSettings.add(positionGui_Engine);

	params_AppSettings.add(guiManager.bAutoResize);
	params_AppSettings.add(guiManager.bDebug);
	params_AppSettings.add(guiManager.bExtra);

	//--

	// callbacks

	// gui
	listener_SHOW_gui = bGui.newListener([this](bool &)
	{
		this->Changed_bGui();
	});

	// app mode
	listener_MODE_App = MODE_App.newListener([this](int &)
	{
		this->Changed_MODE_App();
	});

	//--

	// presetsManager
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//for video mark/loops engine
	setup_PresetsManager();
#endif

	//--

	// gui

#ifdef USE_ofxGuiExtended
	gui_CustomizeApply();
#endif

	// ofApp.cpp
	setup_ImGui();

	//--

	if (ENABLE_Keys_Player) addKeysListeners();

	//-

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	mMidiParams.connect();
	mMidiParams.add(params_ControlRemote);
	mMidiParams.add(params_Preset);
	//mMidiParams.add(presets.getParametersSelectorToggles());
#endif

	//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.setup();

	// callback to receive BeatTicks
	listenerBeat = beatClock.BeatTick_TRIG.newListener([&](bool&) {this->Changed_BeatTick(); });
	listenerBpm = beatClock.BPM_Global.newListener([&](float&) {this->Changed_BeatBpm(); });
#endif

	//----

	// startup 
	startup(); // video file path will (must) be loaded now
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::startup()
{
	DISABLE_CALLBACKS = false;

	//load settings
	//if (0) //bypass 
	{
		//-

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
		loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_Preset);

		//#ifdef USE_ofxChannelFx
		//		loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_ChannelFX);
		//#endif
#endif

		//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
		//presets.setImGuiAutodraw(true); // -> required true when using only one ImGui instance inside the add-ons of your ofApp
		presets.addGroup(params_Preset);
#endif

		// add after created object
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		mMidiParams.add(presets.getParametersSelectorToggles());
#endif

		//-

		loadGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

		//-

#ifdef USE_ofxGuiExtended
		panel_Control->setPosition(positionGui_ControlPanel.get().x, positionGui_ControlPanel.get().y);
		panel_Engine->setPosition(positionGui_Engine.get().x, positionGui_Engine.get().y);

		//-

		//theme
		path_Theme = "assets/theme/";
		path_Theme += "theme_ofxGuiExtended2.json";
		loadTheme(path_Theme);

		//panel_Control->setPosition(900, 600);//temp placement
#endif
	}


	//----

	// hap video player

	// A. hardcoded video file
	//videoFilePath="/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov";//default
	//videoFilePath="\movies\SampleHap.mov";
	//videoName="NightmoVES4";

	//// B. compose the path name from the name stored in the xml
	//// BUG: videoName do not loads because the string param is used too in the other group param above..
	//// is not stored, like if goes out of the param group
	//auto myStrings = ofSplitString(videoFilePath, "/");//not working bc '\'
	//videoName = myStrings[myStrings.size() - 1];

	//split string path using boost
	//https://stackoverflow.com/questions/10099206/howto-parse-a-path-to-a-vector-using-c
	boost::filesystem::path p1(videoFilePath.get());
	//boost::filesystem::path p1("/usr/local/bin");
	////boost::filesystem::path p2("c:\\");
	//std::cout << p1.filename() << std::endl; // prints "bin"
	//std::cout << p1.parent_path() << std::endl; // prints "/usr/local"
	videoName = ofToString(p1.filename());

	//--

	// A. load the path of the movie from xml settings
	loadMovie(videoFilePath);

	// B. hardcoded path file
	//loadMovie("movies/NightmoVES4.mov");
	//loadMovie("/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov");

	player.setLoopState(OF_LOOP_NORMAL);
	player.setVolume(0.0);

	// workflow
	// skip black intro
	//player.setPosition(0.05);

	//--

	POSITION = POSITION_Start;
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setActive(bool b)
{
	ENABLE_Active = b;
	setGuiVisible(b);
	setKeysEnable(b);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setKeysEnable(bool b)
{
	ENABLE_Keys_Player = b;
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.setEnableKeys(b);
#endif
}

#ifdef USE_ofxChannelFx
//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_ChannelFx()
{
	//ofxChannelFx
	channelFx.setPath_GlobalFolder(path_GLOBAL_Folder + "/ofxChannelFx");//ofxSurfingVideoSkip/ofxChannelFx
	//channelFx.setPath_GlobalFolder(path_GLOBAL_Folder);
	channelFx.setup();
	//channelFx.setGuiPosition(glm::vec2(ofGetWidth() - 230, 10));
	//channelFx.setPosition_PresetClicker(glm::vec2(ofGetWidth() - 230, 10));
	channelFx.setEnableKeys(true);
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::update(ofEventArgs & args)
{
	//--

	// gui
	updateVideoPLayer();

	//--
//
//#ifdef USE_ofxSurfingMoods
//	moodsSurfer.update();
//#endif

	//--
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::calculateKick()
{
	//kick-drift
	//calculate desired step size
	//by left-right arrow keys

	//just try, bc if video is not loaded will need to do it again
	totalNumFrames = player.getTotalNumFrames();
	if (totalNumFrames == 0)
		ofLogError(__FUNCTION__) << "totalNumFrames is 0. Could wait until video file is loaded...";
	else
		ofLogNotice(__FUNCTION__) << "totalNumFrames: " << totalNumFrames;

	frameSizeNorm = 1.0f / (float)totalNumFrames;//decimals size of a frame
	ofLogNotice(__FUNCTION__) << "frameSizeNorm: " << frameSizeNorm;
	kickSizeFrame = frameSizeNorm * (float)numFramesToKick;
	ofLogNotice(__FUNCTION__) << "kickSizeFrame: " << kickSizeFrame;

	//float currDur = player.getDuration();//step proportional to video secs
	//currFrame = ofMap(POSITION, 0, 1, 0, totalNumFrames);//curr frame?
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateVideoPLayer()
{
	POSITION = player.getPosition();

	//--

	// mantain start before finish
	if (POSITION_Start == POSITION_End) {
		POSITION_End += kickSizeFrame;
	}
	else if (POSITION_Start > POSITION_End) {
		float temp = POSITION_End;
		POSITION_Start = POSITION_End;
		POSITION_End = temp;
	}

	//--

	//TODO:
	//if load video fails totalNumFrames will be 0, then we will retry every x secconds!
	if (totalNumFrames == 0 && ofGetFrameNum() % (60 * 5) == 0)
	{
		calculateKick();
	}

	//--

	//display current time in minute:seconds. max minutes is 99 to nice formatting
	positionSeconds = player.getDuration() * POSITION;

	// time
	int currMin, currSec;
	std::string strMin, strSec;
	currMin = positionSeconds / 60;
	currSec = ((int)(positionSeconds)) % 60;
	strMin = (currMin < 10) ? "0" : "";
	strSec = (currSec < 10) ? "0" : "";
	strMin += ofToString(currMin);
	strSec += ofToString(currSec);
	videoTIME = /*"Time " + */strMin + ":" + strSec;//std::string

	// frame
	videoFRAME = ofToString(POSITION*totalNumFrames, 0);

	////videoTIME += "  |  Frame " + ofToString(POSITION*totalNumFrames, 0);
	//videoTIME += "          \t" + videoFRAME.get();

	//--

	//check if player is running
	bool isPlaying = player.isPlaying() && !player.isPaused();

	//TODO:
	//BUG: last/first frame get flicked..

	// playing. player running
	if (isPlaying)
	{
		//cout << "isPlaying: " << isPlaying << endl;

		//loop engine
		//TODO: 
		// workflow: added !MODE_EDIT to allow playing out of range loop
		//but requires to imrpove workflow when playing preset with EDIT MODE enabled

		if (MODE_LOOP)// && !MODE_EDIT)
		{
			if (player.getPosition() >= POSITION_End)
			{
				if (!loopedBack)
				{
					if (reverseSpeed)
						POSITION = POSITION_End;
					else
						POSITION = POSITION_Start;
				}
				else
				{
					POSITION = POSITION_End;
					reverseSpeed = !reverseSpeed;
				}

				player.setPosition(POSITION);
			}
			else if (player.getPosition() <= POSITION_Start)//? < only
			{
				if (!loopedBack)
				{
					if (reverseSpeed)
						POSITION = POSITION_End;
					else
						POSITION = POSITION_Start;
				}
				else
				{
					POSITION = POSITION_Start;
					reverseSpeed = !reverseSpeed;
				}

				player.setPosition(POSITION);
			}
		}
	}

	else // not playing. player stopped
	{
		if (!inScrub && !ENABLE_TimersSkipRev && !player.isPaused())
		{
			//clamp cursor into loop
			if (MODE_LOOP && !MODE_EDIT)
			{
				if (player.getPosition() >= POSITION_End)
				{
					POSITION = POSITION_End;
					player.setPosition(POSITION);
				}
				else if (player.getPosition() <= POSITION_Start)
				{
					POSITION = POSITION_Start;
					player.setPosition(POSITION);
				}
			}
		}
	}

	//--

	// auto hide gui system

	if (ofGetElapsedTimeMillis() - lastMovement < time_autoHide)
	{
		draw_Autohide_PRE = ENABLE_drawVideoControlBar;
		ENABLE_drawVideoControlBar = true;
	}
	else
	{
		if (ENABLE_AutoHide)
		{
			ENABLE_drawVideoControlBar = false;
			draw_Autohide_PRE = ENABLE_drawVideoControlBar;
		}
	}

	//--

	//TODO
	// disable to avoid bug with clicks l/r on gui (ofxGuiExtended2)
	// hide mouse if changed
	if ((ENABLE_drawVideoControlBar != draw_Autohide_PRE) || ENABLE_AutoHide)
	{
		ofRectangle window = ofGetWindowRect();
		if (!ENABLE_drawVideoControlBar && window.inside(ofGetMouseX(), ofGetMouseY()))
		{
			ofHideCursor();
		}
		else
		{
			ofShowCursor();
		}
	}

	//----

	// skipper engine:
	updateTimers();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateTimers()
{
	// time

	if (ENABLE_TimersSkipRev && MODE_SkipTime && !inScrub && PLAYING)
	{
		uint64_t t;
		int tmax;
		t = ofGetElapsedTimeMillis() - last_TRIG_time;

#ifndef USE_BPM_TIMER_MODE
		tmax = timePeriod_skipper;
#else
		tmax = divBeatSkipper.get()*((60000 / bpmDivider.get()) / (bpmTimer.get()));
#endif
		if (t >= tmax) // done timer 
		{
			last_TRIG_time = ofGetElapsedTimeMillis();

			// trig jump skip
			TRIG_time_Skiper = true;

			//// workflow
			//if (MODE_SkipReverse)
			//{
			//	last_TRIG_reverse = ofGetElapsedTimeMillis();
			//	TRIG_bReverseSkipper = true;
			//}
		}

		timer_SkipTime = MAX(0, t / (float)tmax);
	}

	//-

	// reverse

	if (ENABLE_TimersSkipRev && MODE_SkipReverse && !inScrub && PLAYING)
	{
		uint64_t t;
		int tmax;
		t = ofGetElapsedTimeMillis() - last_TRIG_reverse;

#ifndef USE_BPM_TIMER_MODE
		tmax = timePeriod_reverser;
#else
		tmax = divBeatReverse.get()*((60000 / bpmDivider.get()) / (bpmTimer.get()));
#endif
		if (t >= tmax)
		{
			last_TRIG_reverse = ofGetElapsedTimeMillis();

			//trig jump skip
			TRIG_bReverseSkipper = true;
		}

		timer_SkipRev = MAX(0, t / (float)tmax);
	}

	//--

	// time trigger

	if (TRIG_time_Skiper == true)
	{
		TRIG_time_Skiper = false;

		//if (MODE_SkipTime)
		{
			float skipPos;
			if (MODE_LOOP)
			{
				skipPos = ofRandom(POSITION_Start, POSITION_End);
			}
			else
			{
				skipPos = ofRandom(0.0, 1.0);
			}

			//cout << "skipPos: " << skipPos << endl;
			player.setPosition(skipPos);
		}
	}

	//-

	// reverse trigger

	if (TRIG_bReverseSkipper == true)
	{
		TRIG_bReverseSkipper = false;

		//if (MODE_SkipReverse)
		{
			reverseSpeed = !reverseSpeed;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Gui()
{
	draw_ImGui();

//#ifdef USE_ofxSurfingMoods
//	moodsSurfer.draw();
//#endif
//
	// bar controller 
	draw_VideoControls();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setGuiVisible(bool b)
{
	//bGui_SurfingVideo = b;//TODO: synced...

	//TODO:
	//ENABLE_GuiVisibleByAutoHide = b;
	if (b)
	{
		draw_Autohide_PRE = ENABLE_drawVideoControlBar;
		ENABLE_drawVideoControlBar = true;
		lastMovement = ofGetElapsedTimeMillis();
	}

	//draw control bar
	ENABLE_drawVideoControlBar = b;

	//-

	//presets
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.setVisible_PresetClicker(b);
	//presetsManager.setVisible_GUI_ImGui(b);
	//presetsManager.setEnableKeys(b);
	//if (!b)//only if must hide
	//{
	//	presetsManager.setVisible_GUI_Internal(b);
	//}
#endif

	//-

#ifdef USE_ofxGuiExtended
	//engine
	gui.getVisible().set(b);//all
	panel_Engine->getVisible().set(b);
	panel_Control->getVisible().set(b && SHOW_Advanced.get());
#endif

	//-

	//ofxChannelFx
#ifdef USE_ofxChannelFx
	if (b && SHOW_Video_FX) {
		channelFx.setVisibleGui(true);
		//channelFx.setVisible_PresetClicker(true);
	}
	else {
		channelFx.setVisibleGui(false);
		//channelFx.setVisible_PresetClicker(false);
	}
#endif

	//-

//	//ofxSurfingMoods
//#ifdef USE_ofxSurfingMoods
//	moodsSurfer.setGui_Visible(b && SHOW_MoodMachine);
//#endif
}

//--

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseMoved(ofMouseEventArgs &eventArgs)
{
	if (ENABLE_Active)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;

		{
			lastMovement = ofGetElapsedTimeMillis();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::refreshMouse(int button, float position)
{
	if (MODE_EDIT)
	{
		if (button == 0)
		{
			// workflow a
			POSITION_Start = position;
			if (POSITION_End < POSITION_Start) POSITION_End = POSITION_Start;

			// workflow b
			//if (position < POSITION_End) POSITION_Start = position;
			//else if (position > POSITION_End) 
			//{
			//	float POSITION_Start_PRE = POSITION_Start;
			//	float POSITION_End_PRE = POSITION_End;

			//	POSITION_End = position;
			//	POSITION_Start = POSITION_Start_PRE;
			//}
		}
		else if (button == 2)
		{
			POSITION_End = position;
			if (POSITION_End < POSITION_Start) POSITION_Start = POSITION_End;

		}
	}
	else
	{
		POSITION = position;
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseDragged(ofMouseEventArgs &eventArgs)
{
	if (ENABLE_Active)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;

		//cout << "button:" << button << endl;

		lastMovement = ofGetElapsedTimeMillis();

		if (inScrub)
		{
			//--

			// calculate
			float _position = static_cast<float>(x - BarInset) / getBarRectangle().width;

			// clamp
			//_position = std::max(0.0f, std::min(_position, 1.0f));
			_position = ofClamp(_position, 0, 1);

			player.setPosition(_position);

			//--

			refreshMouse(button, _position);

			//--

			// workflow
			//disable loop if cursor is out-of-loop
			if (POSITION < POSITION_Start || POSITION > POSITION_End)
			{
				if (MODE_LOOP && !MODE_EDIT) MODE_LOOP = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mousePressed(ofMouseEventArgs &eventArgs)
{
	if (ENABLE_Active)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;

		ofRectangle bar = getBarRectangle();
		if (bar.inside(x, y))
		{
			bool wasPlaying = PLAYING;

			ofLogVerbose(__FUNCTION__) << "mousePressed INSIDE: (" << x << "," << y << ")";

			inScrub = true;
			wasPaused = player.isPaused() || player.getIsMovieDone();

			// workflow
			//auto-stop
			if (PLAYING)
			{
				PLAYING = false;
			}
			//?
			//mouseDragged(x, y, button);

			//--

			// calculate
			float _position = static_cast<float>(x - BarInset) / getBarRectangle().width;

			// clamp
			//_position = std::max(0.0f, std::min(_position, 1.0f));
			_position = ofClamp(_position, 0, 1);

			player.setPosition(_position);

			//--

			refreshMouse(button, _position);

			//--

			if (wasPlaying) PLAYING = true;
		}
		else
		{
			//ofLogNotice(__FUNCTION__) << "OUTSIDE";
		}

		lastMovement = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseReleased(ofMouseEventArgs &eventArgs)
{
	if (ENABLE_Active)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;

		if (inScrub)
		{
			inScrub = false;

			//stop
			//player.setPaused(true);
			//PLAYING = false;
			//player.setPaused(wasPaused);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::windowResized(int _w, int _h)
{
	//path_GLOBAL_Folder = "ofxSurfingVideoSkip";
	window_W = _w;
	window_H = _h;

	//-

	//int xx, yy, xPad, yPad;
	//xx = 2 * BarInset;//default hardcoded pos
	//yy = 10;
	//yPad = 5;
	//xPad = 5;

	//-

	//gui

	////left aligned
	//panel_Engine->setPosition(xx, yy);//anchor panel

	////related from bottom
	////float ww = panel_Engine->getHeight();
	////panel_Engine->setPosition(xx, _h - ww - 200);//left aligned

	//-

//	//populate all gui panels from left to right of previous one
//
//#ifdef USE_ofxSurfingMoods
//	auto gPos = panel_Engine->getPosition();
//	auto gWidth = panel_Engine->getWidth();
//	moodsSurfer.setPosition(gPos.x + gWidth + xPad, gPos.y);//right to the user panel
//
//	//float pw = 400;
//	//float px = window_W * 0.5f - pw * 0.5f;//center on screen
//	//moodsSurfer.setPreviewPosition(px, 20, pw, 40);//custom preview position
//#endif

	//-

//#ifdef USE_ofxSurfingMoods
//	auto gGenWidth = moodsSurfer.getGuiUserWidth();//right to the moodsSurfer panel
//	gui_VideoFx->setPosition((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y);
//#else
//	float gGenWidth = 200;
//	float gWidth = 200;
//	glm::vec2 gPos = glm::vec2(300+ gGenWidth);
//	gui_VideoFx->setPosition((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y);
//#endif

	//--

//#ifdef USE_ofxChannelFx
//	channelFx.windowResized(window_W, window_H);
//
//	//make space
//#ifdef USE_ofxSurfingMoods
//	auto gGenWidth = moodsSurfer.getGuiUserWidth();//right to the moodsSurfer panel
//	channelFx.setGuiPosition(glm::vec2((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y));
//#else
//	auto www = panel_Engine->getWidth();
//	auto xxx = panel_Engine->getPosition().x;
//
//	//ofxChannelFx
//	channelFx.setGuiPosition(glm::vec2(xxx + www + 5, 10));
//	//channelFx.setPosition_PresetClicker(xxx + www + 5 + 325, 10, 50);
//#endif
//#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	//modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
	if (false)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//ofxChannelFx
#ifdef USE_ofxChannelFx
	if (ENABLE_Keys_Fx)
	{
		channelFx.keyPressed(key);
	}
#endif

	//----

	//ofxSurfingVideoSkip
	if (ENABLE_Keys_Player)
	{
		//-

//		//main panels show/hide
//
//		if (key == OF_KEY_F1)
//		{
//			bGui_SurfingVideo = !bGui_SurfingVideo;
//		}
//		else if (key == OF_KEY_F3)
//		{
//#ifdef USE_ofxChannelFx
//			SHOW_Video_FX = !SHOW_Video_FX;
//#endif
//		}
//#ifdef USE_ofxSurfingMoods
//		else if (key == OF_KEY_F2)
//		{
//			//SHOW_MoodMachine = !SHOW_MoodMachine;
//		}
//#endif
//#ifdef USE_ofxSurfingMoods
//		else if (key == OF_KEY_F4)
//		{
//			moodsSurfer.setPreviewToggleVisible();
//		}
//#endif
//		else if (key == OF_KEY_F5)
//		{
//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
//			presetsManager.setVisible_PresetClicker(!presetsManager.isVisible_PresetClicker());
//#endif
//		}
//		else if (key == OF_KEY_F6)
//		{
//			ENABLE_drawVideoControlBar = !ENABLE_drawVideoControlBar;
//		}
//		else if (key == OF_KEY_F7)
//		{
//			SHOW_ControlPanel = !SHOW_ControlPanel;
//		}

		//----

		//TODO:
		//must add modifiers to avoid keys collapsing...
		//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);

		//if (ENABLE_Active)
		//if (MODE_App == 0)

		//-

		//transport

		//play-stop
		if (key == ' ')
		{
			setPlay(!PLAYING);
		}

		else if (key == OF_KEY_RETURN)
		{
#ifdef USE_ofxSurfingMoods
			bool b = !moodsSurfer.isPlaying();
			setPlay_MoodMachine(b);
			setPlay(b);
#endif
		}

		//-

		//loop mode
		else if (key == 'L' || key == 'l')
		{
			MODE_LOOP = !MODE_LOOP;
		}

		//edit mode
		else if (key == 'E' || key == 'e')
		{
			MODE_EDIT = !MODE_EDIT;

			//MODE_LOOP = !MODE_LOOP;
			////if (!MODE_LOOP && ENABLE_TimersSkipRev)
			////{
			////	ENABLE_TimersSkipRev = false;
			////}
		}

		// workflow
		//auto-enable edit mode if it's disabled
		if ((key == 'i' || key == 'o') && !MODE_EDIT)
		{
			MODE_EDIT = true;
		}

		//edit mode
		if (MODE_EDIT)
		{
			if (false) {}

			//store (set)
			else if (key == 'i')
			{
				bSET_START = true;
			}
			else if (key == 'o')
			{
				bSET_END = true;
			}

			//recall (go)
			else if (key == 'I')
			{
				POSITION = POSITION_Start;
			}
			else if (key == 'O')
			{
				POSITION = POSITION_End;
			}

			////user kick-drift frame-by-frame
			//else if (key == OF_KEY_LEFT && MODE_EDIT && !mod_CONTROL)
			//{
			//	bKickL = true;
			//	//POSITION -= kickSizeFrame;
			//}
			//else if (key == OF_KEY_RIGHT && MODE_EDIT && !mod_CONTROL)
			//{
			//	bKickR = true;
			//	//POSITION += kickSizeFrame;
			//}
			//else if (key == OF_KEY_LEFT && MODE_EDIT && mod_CONTROL)
			//{
			//	POSITION -= 10 * kickSizeFrame;
			//}
			//else if (key == OF_KEY_RIGHT && MODE_EDIT && mod_CONTROL)
			//{
			//	POSITION += 10 * kickSizeFrame;
			//}

			//reset preset. basic settings only
			else if (key == 'r' || key == 'R')
			{
				TRIG_bResetEngine = true;
			}
		}

		//-

		////helpers triggers
		//else if (key == 's')
		//{
		//	TRIG_time_Skiper = true;
		//}
		//else if (key == 'S')
		//{
		//	ENABLE_TimersSkipRev = !ENABLE_TimersSkipRev;
		//}

		//autohide
		//else if (key == 'a' && !ENABLE_AutoHide_external)
		//{
		//	ENABLE_AutoHide = !ENABLE_AutoHide;
		//	if (ENABLE_AutoHide)
		//	{
		//		//ENABLE_drawVideoControlBar=true;
		//		lastMovement = ofGetElapsedTimeMillis();
		//	}
		//}

		//-

		//-

		//sound
		else if (key == OF_KEY_UP)
		{
			player.setVolume(player.getVolume() + 0.1);
		}
		else if (key == OF_KEY_DOWN)
		{
			player.setVolume(player.getVolume() - 0.1);
		}

		//-

		//show/hide gui
		else if (key == 'g')
		{
			bGui = !bGui;//independent to autohide state

	//		//hide/show cursor
	//		if (!bGui)
	//		{
	//			ofHideCursor();
	//		}
	//		else
	//		{
	//			ofShowCursor();
	//		}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::dragEvent(ofDragInfo dragInfo)//drag video to load another one
{
	vector<std::string> fileList = dragInfo.files;
	videoName = ofToString(ofFilePath::getFileName(fileList[0]));
	videoFilePath = ofFilePath::getAbsolutePath(fileList[0]);

	ofLogNotice(__FUNCTION__) << "video name: " << videoName;
	ofLogNotice(__FUNCTION__) << "video path: " << videoFilePath;
	loadMovie(fileList[0]);
}

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_PresetsManager()
{
	//presetsManager.setPath_GlobalFolder("ofxSurfingVideoSkipPresets");
	presetsManager.setPath_UserKit_Folder(path_GLOBAL_Folder + "/ofxPresetsManager");

	presetsManager.add(params_Preset, { '0', '1', '2', '3', '4', '5', '6', '7', '8' });

	//to customize name gui panel
	presetsManager.setup("ofxSurfingVideoSkip");
	//group name goes to prefix name file presets

	//easy callback when save/load is done
	presetsManager.DONE_save.addListener(this, &ofxSurfingVideoSkip::Changed_DONE_save);
	presetsManager.DONE_load.addListener(this, &ofxSurfingVideoSkip::Changed_DONE_load);

	//layout
	presetsManager.setVisible_PresetClicker(true);//default
	presetsManager.setVisible_GUI_Internal(false);//default
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_DONE_save(bool &DONE_save)
{
	ofLogNotice(__FUNCTION__) << ofToString(DONE_save ? "TRUE" : "FALSE");
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_DONE_load(bool &DONE_load)
{
	ofLogNotice(__FUNCTION__) << ofToString(DONE_load ? "TRUE" : "FALSE");

	if (player.isLoaded())
	{
		POSITION = POSITION_Start;
		player.setPosition(POSITION);

		//-

		//// workflow
		//if (!MODE_LOOP) MODE_LOOP = true;
	}
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Params(ofAbstractParameter &e) //patch change
{
	if (!DISABLE_CALLBACKS)
	{
		std::string name = e.getName();

		if (name != "" &&
			name != "TRIG SKIP" &&
			name != "TRIG REV" &&
			name != "POSITION" &&
			name != "REVERSE" &&
			name != timer_SkipTime.getName() &&
			name != timer_SkipRev.getName()
			)
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (name == PLAYING.getName())
		{
			if (!PLAYING) // stop 
			{
				player.setPaused(true);

				timer_SkipTime = 0;
				timer_SkipRev = 0;
			}
			else // play
			{
				player.setPaused(false);

				// TODO:

				// workflow
				if (!MODE_LOOP)
				{
					MODE_LOOP = true;
				}

				//if (MODE_EDIT)
				//{
				//	MODE_EDIT = false;
				//}
			}
		}
		else if (name == MODE_EDIT.getName())
		{
			if (MODE_EDIT)
			{
				// workflow
				if (!MODE_LOOP) MODE_LOOP = true;

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
				presetsManager.setEnableKeysArrowBrowse(false);
#endif
			}
			else
			{
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
				presetsManager.setEnableKeysArrowBrowse(true);
#endif
		}
	}
		else if (name == MODE_LOOP.getName())
		{
			if (MODE_LOOP)
			{
				player.setPosition(POSITION_Start);
			}
}
		else if (name == bSET_START.getName() && bSET_START)
		{
			bSET_START = false;
			POSITION_Start = player.getPosition();

			if (MODE_EDIT)
			{
				// workflow
				//enable loop
				if (!MODE_LOOP) MODE_LOOP = true;

				// workflow
				//if star/end flipped: set finish one second to the right
				if (POSITION_End < POSITION_Start)
				{
					float gap = frameSizeNorm * 60;//1sec at 60fps
					POSITION_End = POSITION_Start + gap;
				}
			}
		}
		else if (name == bSET_END.getName() && bSET_END)
		{
			bSET_END = false;
			POSITION_End = player.getPosition();

			// workflow
			//if star/end flipped: set finish one second to the right
			if (POSITION_End < POSITION_Start)
			{
				float gap = frameSizeNorm * 60;//1sec at 60fps
				POSITION_Start = POSITION_End - gap;
			}
		}

		//-

		else if (name == POSITION.getName())
		{
			player.setPosition(POSITION);
		}

		else if (name == POSITION_Start.getName())
		{
			if (!PLAYING /*&& MODE_EDIT*/)
			{
				POSITION = POSITION_Start;
			}
		}
		else if (name == POSITION_End.getName())
		{
			if (!PLAYING /*&& MODE_EDIT*/)
			{
				POSITION = POSITION_End;
			}
		}

		//TODO
		//BUG: breaks preset save/load...
		//else if (name == "START")
		//{
		//    if (start > finish)
		//    {
		//        float TEMP = start;
		//        start = finish;
		//        finish = start;
		//    }
		//}
		//else if (name == "END")
		//{
		//    if (finish < start)
		//    {
		//        float TEMP = finish;
		//        finish = start;
		//        start = finish;
		//    }
		//}

		// kick frame
		else if (name == bKickL.getName() && bKickL)
		{
			bKickL = false;
			POSITION -= kickSizeFrame;
		}
		else if (name == bKickR.getName() && bKickR)
		{
			bKickR = false;
			POSITION += kickSizeFrame;
		}

		//-

		// speed

		else if (name == speedNorm.getName())
		{
			float min, max;

			min = 1.0f;
			max = speed.getMax();
			if (speedNorm >= 0 && speedNorm < 1)
			{
				speed = ofMap(speedNorm, 0, 1, min, max, true);
			}
			else
			{
				min = speed.getMin();
				max = 1.0f;

				if (speedNorm >= -1 && speedNorm < 0) {
					speed = ofMap(speedNorm, -1, 0, min, max, true);
				}
			}
		}

		else if (name == speed.getName())
		{
			player.setSpeed((reverseSpeed ? (-1.0f) : (1.0f)) * speed);
		}
		else if (name == speed_Reset.getName() && speed_Reset)
		{
			speed_Reset = false;
			speedNorm = 0.0f;
			//speed = 1.0f;
		}

		//-

		else if (name == TRIG_bResetEngine.getName() && TRIG_bResetEngine)
		{
			TRIG_bResetEngine = false;

			//ENABLE_TimersSkipRev = false;
			//MODE_SkipTime = false;
			//MODE_SkipReverse = false;

			loopedBack = false;
			reverseSpeed = false;

			speed_Reset = true;
			//speed = 1.0f;

			divBeatSkipper = 4;
			divBeatReverse = 2;
		}
		else if (name == TRIG_Reset_Bpm.getName())
		{
			TRIG_Reset_Bpm = false;

			bpmTimer = 120.f;
			bpmDivider = 2;
		}

		else if (name == "REVERSE")
		{
			player.setSpeed((reverseSpeed ? (-1.0f) : (1.0f)) * speed);

			//if (reverseSpeed)
			//{
			//}
		}
		else if (name == MODE_SkipTime.getName())
		{
			//// workflow
			//if (MODE_SkipTime.get())
			//{
			//	if (!ENABLE_TimersSkipRev) ENABLE_TimersSkipRev = true;
			//}
		}
		else if (name == MODE_SkipReverse.getName())
		{
			//// workflow
			//if (MODE_SkipReverse.get()) {
			//	if (!ENABLE_TimersSkipRev) ENABLE_TimersSkipRev = true;
			//}
		}

		else if (name == bGui_SurfingVideo.getName())
		{
			bool b = bGui_SurfingVideo.get();

#ifdef USE_ofxGuiExtended
			//toggle
			//bool b = !panel_Engine->getVisible().get();
			//panel_Engine->getVisible().set(b);

			panel_Engine->getVisible().set(b);
			//if (b)
#endif

			ENABLE_drawVideoControlBar = b;
		}

		//#ifdef USE_ofxSurfingMoods
		//		else if (name == SHOW_MoodMachine.getName())
		//		{
		//			moodsSurfer.setGui_Visible(SHOW_MoodMachine);
		//		}
		//#endif

				//-

				// fx
#ifdef USE_ofxChannelFx
		else if (name == ENABLE_Video_FX.getName())
		{
		}
		else if (name == SHOW_Video_FX.getName())
		{
			bool b = SHOW_Video_FX.get();

			//ofxChannelFx
			channelFx.setVisibleGui(b);
			//channelFx.setVisible_PresetClicker(b);
		}
#endif
		//-

		// presets
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
		else if (name == "SHOW PRESETS")
		{
			presetsManager.setVisible_PresetClicker(SHOW_Presets);
		}
#endif
		//-

		// keys
		else if (name == ENABLE_Keys_Player.getName())
		{
			if (ENABLE_Keys_Player)addKeysListeners();
			else removeKeysListeners();

			//crashes
				//DISABLE_CALLBACKS = true;
				//if (ENABLE_Keys_Player.get())
				//{
				//	ENABLE_Keys_Presets = false;
				//	ENABLE_Keys_Fx = false;
				//}
				//DISABLE_CALLBACKS = false;
		}
		else if (name == ENABLE_Keys_Presets.getName())
		{
			DISABLE_CALLBACKS = true;
			if (ENABLE_Keys_Presets.get())
			{
				ENABLE_Keys_Player = false;
				ENABLE_Keys_Fx = false;
			}
			DISABLE_CALLBACKS = false;

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
			presetsManager.setEnableKeys(ENABLE_Keys_Presets);
#endif
		}
		else if (name == ENABLE_Keys_Fx.getName())
		{
			DISABLE_CALLBACKS = true;
			if (ENABLE_Keys_Fx.get())
			{
				ENABLE_Keys_Presets = false;
				ENABLE_Keys_Player = false;
			}
			DISABLE_CALLBACKS = false;
		}

		//-

#ifdef USE_ofxGuiExtended
		//header
		else if (name == SHOW_ControlPanel_Header.getName())
		{

			panel_Control->setShowHeader(SHOW_ControlPanel_Header.get());
			panel_Engine->setShowHeader(SHOW_ControlPanel_Header.get());
		}
#endif

		//-

		//advanced
		else if (name == SHOW_Advanced.getName())
		{

#ifdef USE_ofxGuiExtended
			panel_Control->getVisible().set(SHOW_Advanced.get() && bGui_SurfingVideo.get());
#endif
		}
		}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_bGui()
{
	ofLogNotice(__FUNCTION__) << ofToString(bGui.get());

	//hide all
	if (!bGui)
	{
		setGuiVisible(false);
	}

	//show all
	else
	{
		setGuiVisible(true);

		draw_Autohide_PRE = ENABLE_drawVideoControlBar;
		ENABLE_drawVideoControlBar = true;
		lastMovement = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_MODE_App()
{
	//ofLogNotice(__FUNCTION__) << "Changed_MODE_App:" << ofToString(MODE_App.get());

	//lastMovement = ofGetElapsedTimeMillis();

	//if (MODE_App == 1)
	//{
	//	//hide all player
	//	setGuiVisible(false);
	//}
	//else if (MODE_App == 0)
	//{
	//	//show all player
	//	if (bGui)
	//	{
	//		setGuiVisible(true);
	//	}
	//}
}
//
////--------------------------------------------------------------
//void ofxSurfingVideoSkip::Changed_draw_Autohide(bool &b)
//{
//	//	if (ENABLE_AutoHide)
//	//	{
//	//		if (ENABLE_drawVideoControlBar != draw_Autohide_PRE)
//	//		{
//	//			ofLogNotice(__FUNCTION__) << "Changed_draw_Autohide: " << ofToString(ENABLE_drawVideoControlBar ? "TRUE" : "FALSE");
//	//
//	//			if (MODE_App == 0)
//	//			{
//	//				if (!bGui && ENABLE_drawVideoControlBar)
//	//				{
//	//					draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//	//					ENABLE_drawVideoControlBar = false;
//	//				}
//	//				setGuiVisible(ENABLE_drawVideoControlBar);
//	//
//	//#ifdef USE_ofxSurfingMoods
//	//				//moodsSurfer.setGui_visible(ENABLE_drawVideoControlBar&&SHOW_MoodMachine);
//	//#endif
//	//			}
//	//			else if (MODE_App == 1)
//	//			{
//	//				if (!bGui && ENABLE_drawVideoControlBar)
//	//				{
//	//					draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//	//					ENABLE_drawVideoControlBar = false;
//	//				}
//	//			}
//	//		}
//	//
//	//		//hide/show cursor
//	//		if (!ENABLE_drawVideoControlBar)
//	//		{
//	//			ofHideCursor();
//	//		}
//	//		else
//	//		{
//	//			ofShowCursor();
//	//		}
//	//	}
//}

#ifdef USE_ofxSurfingMoods

//moodsSurfer
//listeners for inside moodsSurfer

//-------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_RANGE(int & targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;

	//if (targetVal == 0)
	//{
	//    ofBackground(color_MOOD1);
	//}
	//if (targetVal == 1)
	//{
	//    ofBackground(color_MOOD2);
	//}
	//if (targetVal == 2)
	//{
	//    ofBackground(color_MOOD3);
	//}
}
//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_TARGET(int &targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}
//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_A(int &targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.loadPreset(targetVal, 0);
#endif

	//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	presets.load(targetVal);
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_B(int &targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}

//-------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_C(int &targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw(ofEventArgs & args)
{
	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	presetsManager.draw();
	//#endif

#ifdef USE_ofxChannelFx
	if (ENABLE_Video_FX)
	{
		channelFx.begin();
		{
			draw_Video();
		}
		channelFx.end();

		//draw processed
		channelFx.draw();
	}

	//raw clean
	else
#endif
	{
		ofEnableArbTex();
		draw_Video();
	}

	//-

	if (bGui)
	{
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		mMidiParams.draw();
#endif
		draw_Gui();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Video()
{
	if (player.isLoaded())
	{
		ofPushStyle();

		//draw video frame
		ofSetColor(255, 255, 255, 255);

		ofRectangle r(0, 0, player.getWidth(), player.getHeight());
		r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_FILL);//expand
		//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_STRETCH_TO_FILL);
		//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_CENTER);
		//r.scaleTo(ofGetWindowRect());//fill window width

		//--

		player.draw(r.x, r.y, r.width, r.height);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_VideoControls()
{
	if (player.isLoaded())
	{
		int alphaBar = 128;
		float roundB = 5.0f;

		// draw the position bar if appropriate
		if (ENABLE_drawVideoControlBar)
		{
			ofPushStyle();

			//-

			// 1. border rect only. full video timeline
			ofNoFill();
			ofRectangle barFull = getBarRectangle();
			ofSetColor(ofColor::white, alphaBar);
			//ofDrawRectangle(barFull);
			ofDrawRectRounded(barFull, roundB);

			//-

			ofFill();
			ofSetColor(ofColor::white, alphaBar);
			ofRectangle barCurTime = getBarRectangle();
			barCurTime.width = barFull.width * player.getPosition();

			//// 2. filled rectangle from time 0 to current time position
			////ofDrawRectangle(barCurTime);
			//ofDrawRectRounded(barCurTime, roundB);

			int yy = ofGetWindowHeight() - BarInset - BarHeight;

			// 3. loop clip
			// don't draw loop bar if loop not enable
			if (MODE_LOOP)
			{
				// 3. markers loop rectangle: from start to end
				ofSetColor(ofColor(255), 192); // lighter grey
				//ofSetColor(ofColor(64), 192); // darker grey
				int pStart, pWidth;
				int padding = 2;
				pStart = BarInset + barFull.width * POSITION_Start;
				pWidth = (BarInset + barFull.width * POSITION_End) - pStart;
				ofRectangle barLoop = ofRectangle(pStart, yy + padding, pWidth, BarHeight - padding * 2);
				ofDrawRectangle(barLoop);

				// 3.2 red line markes to loop
				if (MODE_EDIT)
				{
					ofNoFill();
					ofSetLineWidth(2.0);
					ofSetColor(ofColor::red);
					ofDrawLine(pStart, yy + padding, pStart, yy + BarHeight - 1);
					ofDrawLine(pStart + pWidth, yy + padding, pStart + pWidth, yy + BarHeight - 1);

					float a = ofxSurfingHelpers::getFadeBlink(0.40, 0.70, 0.3);
					ofSetColor(ofColor(ofColor::red, 255 * a));
					ofFill();
					ofDrawRectangle(barLoop);
				}
			}

			//-

			// 4. red line for current video player time
			ofNoFill();
			ofSetColor(ofColor::red);
			ofSetLineWidth(3.0);
			float posTime = barCurTime.width + BarInset;
			int padding = 3;
			ofDrawLine(posTime, yy - padding, posTime, yy + BarHeight + padding);

			//-

			ofPopStyle();
		}
	}
	else
	{
		if (player.getError().length())
		{
			ofDrawBitmapStringHighlight(player.getError(), 20, 20);
		}
		else
		{
			ofDrawBitmapStringHighlight("MOVIE IS LOADING...", 20, 20);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::loadMovie(std::string _path)
{
	bool b = player.load(_path);

	if (b)
	{
		ofLogNotice(__FUNCTION__) << "LOADED VIDEO '" << _path << "'";

		calculateKick(); // just try bc if video is not loaded will need to do again
		lastMovement = ofGetElapsedTimeMillis();
	}
	else
	{
		ofLogError(__FUNCTION__) << "VIDEO NOT FOUND '" << _path << "' !";

		_path = "movies/SampleHap.mov";
		ofLogWarning(__FUNCTION__) << "...TRYING TO LOAD A BACKUP MOVIE: '" << _path << "' !";

		b = player.load(_path);
		if (!b) ofLogError(__FUNCTION__) << "BAD ERROR!";
	}
}

//--------------------------------------------------------------
ofRectangle ofxSurfingVideoSkip::getBarRectangle() const
{
	return ofRectangle(
		BarInset, ofGetWindowHeight() - BarInset - BarHeight,
		ofGetWindowWidth() - (2 * BarInset), BarHeight);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::loadGroup(ofParameterGroup &g, std::string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::saveGroup(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

#ifdef USE_ofxGuiExtended
//gui customize
//--------------------------------------------------------------
void ofxSurfingVideoSkip::gui_CustomizeDefine()
{
	//--

	//ofxGui
	//TODO: not implemented kind of lite version without ofxGuiExtended

#ifdef USE_ofxGui
	filename_Font = "telegrama_render.otf";
	std::string _path = "assets/fonts/" + filename_Font;
	ofFile file(_path);
	if (file.exists())
	{
		ofLogNotice(__FUNCTION__) << _path << " FOUND";
		ofxGuiSetFont(path, 8);
	}
	else
	{
		ofLogError(__FUNCTION__) << _path << " NOT FOUND!";
	}
#endif

#ifdef USE_ofxGui
	ofxGuiSetDefaultHeight(18);
	ofxGuiSetBorderColor(16);
	ofxGuiSetFillColor(ofColor(48));
	ofxGuiSetTextColor(ofColor::white);
	ofxGuiSetHeaderColor(ofColor(32));
#endif

	//-

	jConf_BigBut1 =
	{
		{"type", "fullsize"},
		{"text-align", "center"},
		{"height", 22},
	};

	jConf_BigBut2 =//highlighted darken widgets: fx enablers, position,
	{
		{"type", "fullsize"},
		{"height", 35},
	};

	jConf_BigBut3 =//play button
	{
		{"type", "fullsize"},
		{"text-align", "center"},
		{"height", 45},
	};

	//jConf_Highligthed =//highlighted 
	//{
	//	{"border-width", 2 },
	//	{"border-color", "rgba(0,0,0,0.5)" }//black color
	//	//{"border-color", "rgba(200,200,0,0.4)" }//yellow color
	//};

	jConf_Labl =//labels highlighted
	{
		{"text-align", "center"},
		{"height", 20},
	};

	jConf_Labl_Hide =//hiden labels
	{
		{"show-name", false},
		{"text-align", "center"},
		{"height", 5},
	};
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::gui_CustomizeApply()
{
	//if (0)
	{
		//panels control
		(panel_Control->getToggle(bGui_SurfingVideo.getName()))->setConfig(jConf_BigBut2);
#ifdef USE_ofxChannelFx
		(panel_Control->getGroup(_param_ChannelFx.getName())->getToggle("ENABLE FX"))->setConfig(jConf_BigBut2);
		(panel_Control->getGroup(_param_ChannelFx.getName())->getToggle("SHOW FX"))->setConfig(jConf_BigBut2);
#endif
#ifdef USE_ofxSurfingMoods
		(panel_Control->getGroup(_param_MoodMachine.getName())->getToggle(SHOW_MoodMachine.getName()))->setConfig(jConf_BigBut2);
#endif

		//-

		//engine
		(panel_Engine->getToggle("EDIT"))->setConfig(jConf_BigBut3);
		(panel_Engine->getToggle("PLAY"))->setConfig(jConf_BigBut3);
		(panel_Engine->getToggle("LOOP"))->setConfig(jConf_BigBut1);

		(panel_Engine->getFloatSlider("POSITION"))->setConfig(jConf_BigBut1);
		(panel_Engine->getFloatSlider("POSITION"))->unregisterMouseEvents();

		(panel_Engine)->getFloatSlider("START")->setConfig(jConf_BigBut1);
		(panel_Engine)->getFloatSlider("END")->setConfig(jConf_BigBut1);
		(panel_Engine)->getFloatSlider("SPEED")->setConfig(jConf_BigBut2);
		//(panel_Engine)->getFloatSlider("SPEED")->setConfig(ofJson{ {{"height", 35}} });
		//(panel_Engine)->getFloatSlider("SPEED")->setConfig(jConf_BigBut1);

		(panel_Engine->getToggle("SET START"))->setConfig(jConf_BigBut1);
		(panel_Engine->getToggle("SET END"))->setConfig(jConf_BigBut1);
		(panel_Engine->getToggle("LOOPED BACK"))->setConfig(jConf_BigBut1);
		(panel_Engine->getToggle("REVERSE"))->setConfig(jConf_BigBut1);

		//(panel_Engine->getControl("ENGINE"))->setConfig(jConf_Labl);//label

		//-

		//skip timers 

		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getControl("_spacer1_")->setConfig(jConf_Labl_Hide);
		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getControl("_spacer2_")->setConfig(jConf_Labl_Hide);

		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("ENABLE TIMERS")->setConfig(jConf_BigBut1);
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("A MODE SKIP")->setConfig(jConf_BigBut1);
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("TRIG SKIP")->setConfig(jConf_BigBut1);
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("B MODE REV")->setConfig(jConf_BigBut1);
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("TRIG REV")->setConfig(jConf_BigBut1);

		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("ENABLE TIMERS")->setConfig(jConf_Highligthed);
		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("A MODE SKIP")->setConfig(jConf_Highligthed);
		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getToggle("B MODE REV")->setConfig(jConf_Highligthed);

		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getIntSlider("SKIP-TIME TIME")->setConfig(jConf_BigBut1);
		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->getIntSlider("SKIP-REVERSE TIME")->setConfig(jConf_BigBut1);


		ofJson _j =//hide labels and slim sliders to debug timers progress
		{
			{"show-name", false},
			{"text-color", "transparent"},
			{"height", 8}
		};
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getControl(timer_SkipTime.getName())->setConfig(_j);
		(panel_Engine->getGroup(_param_SkipEngine.getName()))->getControl(timer_SkipRev.getName())->setConfig(_j);

		//TODO: collapse
//		//(panel_Engine->getGroup(_param_SkipEngine.getName()))->mini;

		//bpm
		(panel_Engine
			->getGroup(_param_SkipEngine.getName())
			->getGroup(_param_Clock.getName())
			->getFloatSlider(bpmTimer.getName()))
			->setConfig(jConf_BigBut2);
	}
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::exit()
{
	// settings
#ifdef USE_ofxGuiExtended
	positionGui_ControlPanel = glm::vec2(panel_Control->getPosition().x, panel_Control->getPosition().y);
	positionGui_Engine = glm::vec2(panel_Engine->getPosition().x, panel_Engine->getPosition().y);
#endif

	// save app settings
	saveGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_Preset);
#endif

	// channel fx
#ifdef USE_ofxChannelFx

//#ifndef USE_ofxPresetsManager__VIDEO_SKIP
//	saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_ChannelFX);
//#endif

	// ofxChannelFx
	channelFx.exit();
#endif

	//-

	// callbacks
	ofRemoveListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);

	//disable for the momment
	//ofRemoveListener(params_Control_VideoFX.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_params_VideoFX);

	////auto hide callback
	//ENABLE_drawVideoControlBar.removeListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//-

	//presetsManager
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.DONE_save.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_save);
	presetsManager.DONE_load.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_load);
	presetsManager.exit();
#endif

	//-

#ifdef USE_ofxSurfingMoods
	//listener for inside class moodsSurfer
	moodsSurfer.RANGE_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	moodsSurfer.TARGET_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	moodsSurfer.PRESET_A_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	moodsSurfer.PRESET_B_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	moodsSurfer.PRESET_C_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setPath_GlobalFolder(std::string folder)
{
	ofLogNotice(__FUNCTION__) << folder;
	path_GLOBAL_Folder = folder;
	CheckFolder(folder);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_ImGui()
{
	guiManager.setImGuiAutodraw(true);
	//guiManager.setSharedMode(true);
	guiManager.setup(); // this instantiates and configurates ofxImGui inside the class object.

	//-

	//widgetsManager.AddWidgetConf(bSET_START, SurfingTypes::OFX_IM_BUTTON_SMALL, true, 2);
	//widgetsManager.AddWidgetConf(bSET_END, SurfingTypes::OFX_IM_BUTTON_SMALL, false, 2);
	//widgetsManager.AddWidgetConf(MODE_LOOP, SurfingTypes::OFX_IM_BUTTON_BIG, false, 1);
	//widgetsManager.AddWidgetConf(loopedBack, SurfingTypes::OFX_IM_BUTTON_SMALL, true, 2);
	//widgetsManager.AddWidgetConf(loopedBack, SurfingTypes::OFX_IM_BUTTON_SMALL, false, 2);

	//if (bCustom2)
	//{
	//	widgetsManager.AddWidgetConf(bEnable, SurfingTypes::OFX_IM_TOGGLE_BIG, false, 1, 20);
	//	widgetsManager.AddWidgetConf(bPrevious, SurfingTypes::OFX_IM_BUTTON_SMALL, true, 2);
	//	widgetsManager.AddWidgetConf(separation, SurfingTypes::OFX_IM_STEPPER);
	//	widgetsManager.AddWidgetConf(speed, SurfingTypes::OFX_IM_DRAG, false, 1, 10);
	//	widgetsManager.AddWidgetConf(shapeType, SurfingTypes::OFX_IM_SLIDER);
	//	widgetsManager.AddWidgetConf(size, SurfingTypes::OFX_IM_STEPPER);
	//	widgetsManager.AddWidgetConf(amount, SurfingTypes::OFX_IM_DRAG, false, 1, 10);
	//	widgetsManager.AddWidgetConf(bMode1, SurfingTypes::OFX_IM_TOGGLE_BIG, true, 2);
	//	widgetsManager.AddWidgetConf(bMode2, SurfingTypes::OFX_IM_TOGGLE_BIG, false, 2);
	//	widgetsManager.AddWidgetConf(bMode3, SurfingTypes::OFX_IM_TOGGLE_BIG, true, 2);
	//	widgetsManager.AddWidgetConf(bMode4, SurfingTypes::OFX_IM_TOGGLE_BIG, false, 2);
	//	//widgetsManager.AddWidgetConf(lineWidth3, SurfingTypes::OFX_IM_DRAG); // not works?
	//	// hide some params from any on-param-group appearance
	//	widgetsManager.AddWidgetConf(speed3, SurfingTypes::OFX_IM_HIDDEN, false, -1, 50);
	//	widgetsManager.AddWidgetConf(size2, SurfingTypes::OFX_IM_HIDDEN, false, -1, 50);
	//	widgetsManager.AddWidgetConf(bPrevious, SurfingTypes::OFX_IM_HIDDEN);
	//	widgetsManager.AddWidgetConf(bNext, SurfingTypes::OFX_IM_HIDDEN);
	//	widgetsManager.AddWidgetConf(lineWidth, SurfingTypes::OFX_IM_HIDDEN);
	//}

	//-

	//guiManager.bAutoResize = false;
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiSkipTimers()
{
	// panels sizes
	float xx = 10;
	float yy = 10;
	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT;

	// widgets sizes

	static ofParameter<bool> _bAutoResize{ "Auto Resize",false };

	//-

	std::string n;

	//-

	// windows
	{
		n = "SKIP TIMERS";
		//n = params_Engine.getName();
		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (_bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
		//if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		guiManager.beginWindow(n.c_str(), (bool*)&bGui_SkipTimers.get(), flagsw);
		{
			refreshLayout();

			//ImGui::Dummy(ImVec2(0, 2));

			//--

			bool bOpen;
			ImGuiTreeNodeFlags _flagt;

			//if (ENABLE_TimersSkipRev && (MODE_SkipReverse || MODE_SkipTime)) 
			{
				bOpen = false;
				_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagt |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::TreeNodeEx("BPM CLOCK", _flagt))
				{
					refreshLayout();

					//widgetsManager.Add(bpmTimer, SurfingTypes::OFX_IM_STEPPER);
					//widgetsManager.Add(bpmTimer, SurfingTypes::OFX_IM_DEFAULT);
					////widgetsManager.Add(bpmDivider, SurfingTypes::OFX_IM_DEFAULT);

					ofxImGuiSurfing::AddParameter(bpmTimer);
					ofxImGuiSurfing::AddParameter(bpmDivider);

					if (ImGui::Button("HALF", ImVec2(_w50, _h / 2))) {
						bpmTimer /= 2.0f;
					}
					ImGui::SameLine();
					if (ImGui::Button("DOUBLE", ImVec2(_w50, _h / 2))) {
						bpmTimer *= 2.0f;
					}

					widgetsManager.Add(TRIG_Reset_Bpm, SurfingTypes::OFX_IM_BUTTON_SMALL);

					ImGui::TreePop();
				}
			}

			//--

			ImGui::Dummy(ImVec2(0, 10));

			// big enablers
			widgetsManager.Add(ENABLE_TimersSkipRev, SurfingTypes::OFX_IM_TOGGLE_BIG);
			if (ENABLE_TimersSkipRev)
			{
				ofxImGuiSurfing::AddBigToggle(MODE_SkipTime);
				ofxImGuiSurfing::AddBigToggle(MODE_SkipReverse);

				//widgetsManager.Add(MODE_SkipTime, SurfingTypes::OFX_IM_TOGGLE_BIG, true, 0);
				//widgetsManager.Add(MODE_SkipReverse, SurfingTypes::OFX_IM_TOGGLE_BIG, false, 0);
			}

			ImGui::Dummy(ImVec2(0, 2));

			//if (MODE_SkipTime)

			bOpen = true;
			_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagt |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx("A SKIP TIME", _flagt))
			{
				refreshLayout();

				widgetsManager.Add(TRIG_time_Skiper, SurfingTypes::OFX_IM_BUTTON_BIG, false, 1, 5);
				if (MODE_SkipTime && ENABLE_TimersSkipRev)
				{
					//widgetsManager.Add(divBeatSkipper, SurfingTypes::OFX_IM_DEFAULT);
					ofxImGuiSurfing::AddParameter(divBeatSkipper);

					// draw progress bar
					////widgetsManager.Add(timer_SkipTime, SurfingTypes::OFX_IM_DEFAULT);
					ofxImGuiSurfing::AddProgressBar(timer_SkipTime);
				}

				ImGui::TreePop();
			}

			//-

			ImGui::Dummy(ImVec2(0, 2));

			//if (MODE_SkipReverse)

			if (ImGui::TreeNodeEx("B SKIP REV", _flagt))
			{
				refreshLayout();

				widgetsManager.Add(TRIG_bReverseSkipper, SurfingTypes::OFX_IM_BUTTON_BIG, false, 1, 5);

				if (MODE_SkipReverse && ENABLE_TimersSkipRev)
				{
					//widgetsManager.Add(divBeatReverse, SurfingTypes::OFX_IM_DEFAULT);
					ofxImGuiSurfing::AddParameter(divBeatReverse);

					// draw progress bar
					////widgetsManager.Add(timer_SkipRev, SurfingTypes::OFX_IM_DEFAULT);
					ofxImGuiSurfing::AddProgressBar(timer_SkipRev);
				}

				ImGui::TreePop();
			}

			ImGui::Dummy(ImVec2(0, 5));
			widgetsManager.Add(TRIG_bResetEngine, SurfingTypes::OFX_IM_BUTTON_SMALL);

			//--

			//// skip player group
			//ImGuiTreeNodeFlags flagst;
			//flagst = ImGuiTreeNodeFlags_None;
			////flagst |= ImGuiTreeNodeFlags_DefaultOpen;
			//flagst |= ImGuiTreeNodeFlags_Framed;
			////ofxImGuiSurfing::AddGroup(params_Engine, flagst);
			//ofxImGuiSurfing::AddGroup(_param_Clock, flagst);

			ofxImGuiSurfing::AddToggleRoundedButton(_bAutoResize);
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiPanels()
{
	// panels sizes
	float xx = 10;
	float yy = 10;
	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT;

	//-

	std::string n;

	ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
	if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	//-

	n = "SURFING VIDEO";
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	//guiManager.beginWindow(n.c_str(), (bool*)&bGui_SurfingVideo.get(), flagsw);
	guiManager.beginWindow(n.c_str(), (bool*)&bGui.get(), flagsw);
	{
		refreshLayout();

		//-

		ImGui::Text("PANELS");

		// video
		widgetsManager.Add(bGui_SurfingVideo, SurfingTypes::OFX_IM_TOGGLE_SMALL);
		//widgetsManager.Add(bGui_SkipTimers, SurfingTypes::OFX_IM_TOGGLE_SMALL);

		//-

		// midi
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		widgetsManager.Add(mMidiParams.bGui, SurfingTypes::OFX_IM_TOGGLE_SMALL);
#endif
		//-

		// beat clock
#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
		widgetsManager.Add(beatClock.bGui, SurfingTypes::OFX_IM_TOGGLE_SMALL);
#endif
		//-

		// moods
#ifdef USE_ofxSurfingMoods 
		widgetsManager.Add(moodsSurfer.bGui, SurfingTypes::OFX_IM_TOGGLE_SMALL);
#endif
		//-

		// presets
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
		widgetsManager.Add(presets.bGui, SurfingTypes::OFX_IM_TOGGLE_SMALL);
#endif
	}
	guiManager.endWindow();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiControls()
{
	// panels sizes
	float xx = 10;
	float yy = 10;
	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT;

	//-

	std::string n;

	ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
	if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	//-

	// engine
	{
		n = bGui_SurfingVideo.getName();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		guiManager.beginWindow(n.c_str(), (bool*)&bGui_SurfingVideo.get(), flagsw);
		{
			refreshLayout();

			//ImGui::Dummy(ImVec2(0, 5));
			//ImGui::Separator();
			//ImGui::Dummy(ImVec2(0, 5));

			//--

			// file
			bool bOpen = false;
			ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagt |= ImGuiTreeNodeFlags_Framed;
			string n = videoName;
			//string n = "VIDEO FILE";

			if (ImGui::TreeNodeEx(n.c_str(), _flagt))
			{
				refreshLayout();

				if (ImGui::Button("OPEN FILE", ImVec2(_w100, _h / 2)))
				{
					setPathOpenDialog();
				}
				ImGui::Dummy(ImVec2(0, 1));
				//ImGui::Text(videoName.get().data());
				ImGui::Text(videoFilePath.get().data());
				ImGui::TreePop();
			}

			ImGui::Dummy(ImVec2(0, 2));

			// play
			widgetsManager.Add(PLAYING, SurfingTypes::OFX_IM_TOGGLE_BIG);
			widgetsManager.Add(MODE_EDIT, SurfingTypes::OFX_IM_TOGGLE_BIG);

			ImGui::Dummy(ImVec2(0, 10));

			// preset clicker
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
			presets.draw_ImGui_MiniClicker();
			ImGui::Dummy(ImVec2(0, 2));
#endif

			// mark clip start/end
			widgetsManager.Add(bSET_START, SurfingTypes::OFX_IM_BUTTON_BIG, true, 2);
			widgetsManager.Add(bSET_END, SurfingTypes::OFX_IM_BUTTON_BIG, false, 2);
			ImGui::Dummy(ImVec2(0, 2));

			ImGui::Text(("Time " + videoTIME.get()).data());

			// position
			widgetsManager.Add(POSITION, SurfingTypes::OFX_IM_SLIDER);
			// range
			ofxImGuiSurfing::AddRangeParam("CLIP", POSITION_Start, POSITION_End, "%.3f      %.3f", 1.0f);

			ImGui::Dummy(ImVec2(0, 5));

			// start/finish
			static bool bFineTune = false;
			ToggleRoundedButton("FineTune", &bFineTune, ImVec2(30, 20));
			if (bFineTune)
			{
				ImGui::Indent();

				refreshLayout();

				ImGui::Text(("Frame " + videoFRAME.get()).data());
				//ImGui::Text("Kick Frame");
				{
					ImGui::PushButtonRepeat(true);
					{
						if (ImGui::Button("-", ImVec2(_w50, _h / 2)))
						{
							bKickL = true;
						}
						ImGui::SameLine();
						if (ImGui::Button("+", ImVec2(_w50, _h / 2)))
						{
							bKickR = true;
						}
					}
					ImGui::PopButtonRepeat();
				}

				widgetsManager.Add(POSITION, SurfingTypes::OFX_IM_STEPPER);
				widgetsManager.Add(POSITION_Start, SurfingTypes::OFX_IM_STEPPER);
				widgetsManager.Add(POSITION_End, SurfingTypes::OFX_IM_STEPPER);

				ImGui::Unindent();
			}

			refreshLayout();

			//--

			ImGui::Dummy(ImVec2(0, 2));

			// speed
			//widgetsManager.Add(speed, SurfingTypes::OFX_IM_SLIDER);
			widgetsManager.Add(speedNorm, SurfingTypes::OFX_IM_SLIDER);

			//{
			//	// knob1
			//	ofxImGuiSurfing::AddKnob(speedNorm, 0.001, 30, 0.1);
			//	// knob2
			//	ImGui::SameLine();
			//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
			//	float width = 30;
			//	const ImU32 color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
			//	float v_min = 0;
			//	float v_max = 1;
			//	float v_step = 0.001f;
			//	static float p_value = 0.5f;
			//	ofxImGuiSurfing::KnobFloat(draw_list, width, color, "Value", &p_value, v_min, v_max, v_step);
			//	//static float _x = 100; 
			//	//static float _y = 100; 
			//	//ofxImGuiSurfing::Pad2D(draw_list, 100, 100, &_x, &_y);
			//}

			//-

			widgetsManager.Add(speed_Reset, SurfingTypes::OFX_IM_BUTTON_SMALL);

			ImGui::Dummy(ImVec2(0, 10));

			// loop
			widgetsManager.Add(MODE_LOOP, SurfingTypes::OFX_IM_TOGGLE_BIG, false, 1);
			widgetsManager.Add(loopedBack, SurfingTypes::OFX_IM_TOGGLE_SMALL, true, 2);
			widgetsManager.Add(reverseSpeed, SurfingTypes::OFX_IM_TOGGLE_SMALL, false, 2);

			ImGui::Dummy(ImVec2(0, 10));

			//-

			//ImGui::Dummy(ImVec2(0, 5));
			//ImGui::Separator();
			//ImGui::Dummy(ImVec2(0, 5));


			// skippers
			//widgetsManager.Add(bGui_SkipTimers, SurfingTypes::OFX_IM_TOGGLE_BIG);
			ofxImGuiSurfing::AddToggleRoundedButton(bGui_SkipTimers);

			ImGui::Dummy(ImVec2(0, 10));

			//--

			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
			if (guiManager.bExtra)
			{
				ImGui::Indent();
				ofxImGuiSurfing::AddToggleRoundedButton(ENABLE_Keys_Player);
				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);
				ofxImGuiSurfing::AddToggleRoundedButton(ENABLE_AutoHide);
				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);

				//widgetsManager.Add(guiManager.bExtra, SurfingTypes::OFX_IM_TOGGLE_SMALL);
				//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
				//guiManager.bAdvanced = guiManager.bExtra;

				// extra panel
				//if (guiManager.bExtra) 
				{
					//ImGui::Dummy(ImVec2(0, 5));
					guiManager.drawAdvancedSubPanel();
				}
				ImGui::Unindent();
			}
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui()
{
	if (!bGui) return;

	guiManager.begin();
	{
		draw_ImGuiPanels();
		if (bGui_SurfingVideo) draw_ImGuiControls();
		if (bGui_SkipTimers) draw_ImGuiSkipTimers();
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setPathOpenDialog()
{
	ofLogNotice(__FUNCTION__) << " SET PRESETS PATH";

	// Open the Open File Dialog
	std::string str = "Select vdieo file. Must be enconded in HAP codec!\n";
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

	// Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		//processOpenFileSelection(openFileResult);
		videoName = openFileResult.getName();
		videoFilePath = openFileResult.getPath();

		ofLogNotice(__FUNCTION__) << "video name: " << videoName;
		ofLogNotice(__FUNCTION__) << "video path: " << videoFilePath;
		loadMovie(videoFilePath);
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "User hit cancel";
	}
}

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_BeatTick() // callback to receive BeatTicks
{
	if (beatClock.BeatTick_TRIG)
	{
		ofLogNotice(__FUNCTION__) << "BeatTick ! #" << beatClock.Beat_current;

#ifdef USE_ofxSurfingMoods
		moodsSurfer.doBeatTick();
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_BeatBpm()
{
	ofLogNotice(__FUNCTION__) << "Bpm " << beatClock.BPM_Global;

#ifdef USE_ofxSurfingMoods
	moodsSurfer.setBpm(beatClock.BPM_Global);
#endif
}
#endif