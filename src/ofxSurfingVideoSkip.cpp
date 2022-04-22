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

	// settings folder
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

	bGui_VideoControlBar.set("Control Bar", false);//var used to hide/show player bar and gui also
	//bGui_VideoControlBar.addListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//--

	//// fbo settings
	//fboSettings.width = 1920;
	//fboSettings.height = 1080;
	//fboSettings.internalformat = GL_RGBA;
	//fboSettings.textureTarget = GL_TEXTURE_2D;
	////fboSettings.internalformat = GL_RGBA32F_ARB;
	////fboSettings.useDepth = true;
	////fboSettings.useStencil = true;
	////fboSettings.depthStencilAsTexture = true;

	//// fbo 
	////ofEnableArbTex();
	////ofDisableeArbTex();
	//fboPreview.allocate(fboSettings);
	//fboPreview.begin();
	//{
	//	ofClear(0, 255);
	//}
	//fboPreview.end();

	//--

	// skipper engine
	videoFilePath.set("videoFilePath", "NO FILE");
	videoName.set("FILE", "NO FILE");
	videoTIME.set("", ""); // current time position

	bGui_SkipTimers.set("SKIP TIMERS", true);

	bGui_Presets.set("SHOW PRESETS", true);

	bAutoHide.set("AutoHide Bar", true);

	bKickL.set("FRAME L", false);
	bKickR.set("FRAME R", false);

	bPlay.set("PLAY", false);
	bMODE_Edit.set("EDIT", false);
	//bMODE_Loop.set("CLAMP", false);
	bMODE_Loop.set("LOOP", false);

	speedNorm.set("SPEED", 0.0f, -1.0f, 1.0f);
#define SPEED_MIN 0.20f
#define SPEED_MAX 50.0f
	speed.set("SPEED_", 1.0f, SPEED_MIN, SPEED_MAX);
	bReset_Speed.set("RESET SPEED", false);

	bMODE_LoopedBack.set("LOOP BACK", false);
	bMODE_Reversed.set("REVERSE", false);
	bTRIG_ResetEngine.set("RESET ENGINE", false);
	bTRIG_Reset_Bpm.set("RESET BPM", false);
	bGui_Advanced.set("SHOW ADVANCED", false);

	POSITION_Start.set("START", 0.0f, 0.0f, 1.0f);
	POSITION_End.set("END", 1.0f, 0.0f, 1.0f);
	POSITION.set("POSITION", 0.0f, 0.0f, 1.0f);
	bSET_START.set("SET START", false);
	bSET_END.set("SET END", false);
	ENABLE_TimersGlobal.set("ENABLE TIMERS", false);
	bTRIG_SkipTime.set("A SKIP", false);
	bTRIG_SkipReverse.set("B REV", false);
	bMODE_SkipTime.set("A MODE SKIP", false);
	bMODE_SkipReverse.set("B MODE REV", false);

	bMODE_SkipLooped.set("SK Loop", true);
	bMODE_SkipPowered.set("SK Pow", false);
	skipPower.set("SK Power", 0.5f, 0.01f, 1.0f);

#ifndef USE_BPM_TIMER_MODE
	timePeriod_skipper.set("SKIP-TIME TIME", 1000, 1, 4000);
	timePeriod_reverser.set("SKIP-REVERSE TIME", 1000, 1, 4000);
#else

	// these are global.
	// will not be included into presets!
	bpmTimer.set("BPM", 120.0f, 40.f, 400.0f);
	bpmDivider.set("DIV", 2, 1, 8);

	timer_SkipTime.set("_timer1_", 0, 0, 1);
	timer_SkipRev.set("_timer2_", 0, 0, 1);

	divBeatSkipper.set("T SKIP", 4, 1, 8);
	divBeatReverse.set("T REV", 2, 1, 8);
#endif

	// beat
	bMODE_Beat.set("MODE BEAT", false);
	beatDuration.set("Duration", 4, 1, 8);
	beatRescale.set("Rescale", 1, 1, 10);
	bMODE_Lock.set("LOCK", false);

	//-

	last_TRIG_time = 0;
	last_TRIG_reverse = 0;
	bMODE_Edit.setSerializable(false);
	speed.setSerializable(false);
	bReset_Speed.setSerializable(false);
	bTRIG_ResetEngine.setSerializable(false);
	bTRIG_Reset_Bpm.setSerializable(false);
	timer_SkipTime.setSerializable(false);
	timer_SkipRev.setSerializable(false);
	bKickL.setSerializable(false);
	bKickR.setSerializable(false);

	// modes. not included into presets

#ifdef USE_ofxChannelFx
	ENABLE_Video_FX.set("ENABLE FX", false);
	SHOW_Video_FX.set("SHOW FX", false);
#endif

	//TODO:
	bGui_ControlPanel.set("SHOW CONTROL PANEL", true);
	bGui_ControlPanel_Header.set("SHOW HEADER", true);

	//#ifdef USE_ofxSurfingMoods
	//	SHOW_MoodMachine.set("SHOW MOOD-MACHINE", true);
	//#endif

	bGui_SurfingVideo.set("SKIP PLAYER", true);

	//ofParameter<std::string> label_FX{ "FX", "" };

	//--

	// video controls
	params_Engine.setName("SKIP PLAYER");

	params_Engine.add(bGui_SkipTimers);

	params_Engine.add(bPlay);
	params_Engine.add(bMODE_Edit);
	params_Engine.add(videoName);//NOTE: a longer string will resize the gui panel width!
	params_Engine.add(POSITION);
	//ofParameter<std::string> _labelName_{ "NAME"," " };
	//params_Engine.add(_labelName_);
	params_Engine.add(videoTIME);

	//-

	// for presets settings

	ofParameter<std::string> label_Engine{ "ENGINE", "" };
	params_Engine.add(label_Engine);

	params_Engine.add(speedNorm);
	params_Engine.add(speed);
	params_Engine.add(bReset_Speed);

	params_Engine.add(bMODE_SkipLooped);
	params_Engine.add(bMODE_SkipPowered);
	params_Engine.add(skipPower);

	params_Engine.add(bMODE_Loop);
	params_Engine.add(bSET_START);
	params_Engine.add(bSET_END);
	params_Engine.add(POSITION_Start);
	params_Engine.add(POSITION_End);

	params_Engine.add(bMODE_LoopedBack);
	params_Engine.add(bMODE_Reversed);

	params_Engine.add(bKickL);
	params_Engine.add(bKickR);

	//----

	// skip engine

	_param_SkipEngine.add(ENABLE_TimersGlobal);

	//-

	// spacers
	//ofParameter<std::string> labelSpacer1{ "_spacer1_", "" };
	//_param_SkipEngine.add(labelSpacer1);
	//ofParameter<std::string> labelSpacer2{ "_spacer2_", "" };
	//_param_SkipEngine.add(labelSpacer2);

	//-

	// skippers

	// time

	_param_SkipEngine.add(bMODE_SkipTime);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_skipper);
#else
	_param_SkipEngine.add(divBeatSkipper);
#endif
	_param_SkipEngine.add(timer_SkipTime);
	_param_SkipEngine.add(bTRIG_SkipTime);

	_param_SkipEngine.add(bMODE_SkipLooped);
	_param_SkipEngine.add(bMODE_SkipPowered);
	_param_SkipEngine.add(skipPower);

	//-

	// reverse

	_param_SkipEngine.add(bMODE_SkipReverse);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_reverser);
#else
	_param_SkipEngine.add(divBeatReverse);
#endif
	_param_SkipEngine.add(timer_SkipRev);
	_param_SkipEngine.add(bTRIG_SkipReverse);

	_param_SkipEngine.add(bTRIG_ResetEngine);

#ifdef USE_BPM_TIMER_MODE
	_param_Clock.add(bpmTimer);
	_param_Clock.add(bpmDivider);
#endif
	_param_Clock.add(bTRIG_Reset_Bpm);
	_param_SkipEngine.add(_param_Clock);

	//_param_SkipEngine.add(bGui_Advanced);

	params_Engine.add(_param_SkipEngine);

	//-

	ofAddListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	//this params are not stored. just from gui panels here

	//--

	// presetsManager 

	// params to save
	// name will be use to prefix file names at /group folder
	params_Preset.setName("Clips");
	//params_Preset.setName("ofxSurfingVideoSkip");
	params_Preset.add(POSITION_Start);
	params_Preset.add(POSITION_End);

	params_Preset.add(bReset_Speed);
	params_Preset.add(speedNorm);
	//params_Preset.add(speed);

	params_Preset.add(bMODE_LoopedBack);
	params_Preset.add(bMODE_Reversed);
	params_Preset.add(ENABLE_TimersGlobal);

#ifndef USE_BPM_TIMER_MODE
	params_Preset.add(timePeriod_skipper);
	params_Preset.add(timePeriod_reverser);

	params_Preset.add(bMODE_SkipTime);
	params_Preset.add(divBeatSkipper);
	params_Preset.add(bMODE_SkipReverse);
	params_Preset.add(divBeatReverse);
#else

	//// exclude
	//params_Preset.add(bpmTimer);
	//params_Preset.add(bpmDivider);

	params_Preset.add(bMODE_SkipTime);
	params_Preset.add(divBeatSkipper);
	params_Preset.add(bMODE_SkipReverse);
	params_Preset.add(divBeatReverse);
#endif

	params_Preset.add(bMODE_Beat);
	params_Preset.add(bMODE_Lock);
	params_Preset.add(beatDuration);
	params_Preset.add(beatRescale);

	// should add more params like direction/reverse/..

	//--

	// gui

	//----

	// main gui panel params

	params_Control.setName("SURFING VIDEO SKIP");

	//params_Control.add(bGui_ControlPanel);//TODO: same than SHOW_GUI ?

	params_Control.add(bGui_SurfingVideo);
	params_Control.add(bGui_SkipTimers);
	params_Control.add(bGui);
	//params_Control.add(bMODE_Edit);

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	params_Control.add(bGui_Presets);
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
	params_Control.add(bAutoHide);
	params_Control.add(bGui_ControlPanel_Header);

	params_Control.add(videoName);

	//--

	// channel fx
#ifdef USE_ofxChannelFx
	setup_ChannelFx();
#endif

	//--

#ifdef USE_ofxSurfingMoods
	surfingMoods.setup();
	surfingMoods.bGui.setName("MOODS");

	// customize
	//surfingMoods.setGui_Visible(SHOW_MoodMachine);
	//surfingMoods.setGui_AdvancedVertical_MODE(true);

	// listeners for inside class surfingMoods
	surfingMoods.RANGE_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	surfingMoods.TARGET_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	surfingMoods.PRESET_A_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	surfingMoods.PRESET_B_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	surfingMoods.PRESET_C_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);

	//surfingMoods.setTickMode(true);
#endif

	//----

	// app settings

	// main app parameters to store recall on exit and start app
	MODE_App.set("MODE_App", 0, 0, 1);

	//-

	// external control
	// to link to OSC/midi control
	params_ControlRemote.setName("Remote Control");
	params_ControlRemote.add(bPlay);
	params_ControlRemote.add(bMODE_Edit);
	params_ControlRemote.add(bKickL);
	params_ControlRemote.add(bKickR);
	params_ControlRemote.add(bSET_START);
	params_ControlRemote.add(bSET_END);
	params_ControlRemote.add(POSITION);

	// app settings to handle store/recall only
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bPlay);
	params_AppSettings.add(bMODE_Loop);
	params_AppSettings.add(videoName);
	params_AppSettings.add(videoFilePath);
	params_AppSettings.add(MODE_App);
	params_AppSettings.add(bGui_Presets);
	params_AppSettings.add(ENABLE_Keys_Player);
	params_AppSettings.add(bGui_SkipTimers);
	//params_AppSettings.add(bGui);

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	params_AppSettings.add(ENABLE_Keys_Presets);
#endif

#ifdef USE_ofxChannelFx
	params_AppSettings.add(ENABLE_Keys_Fx);
#endif

	params_AppSettings.add(bGui_ControlPanel);
	params_AppSettings.add(bGui_ControlPanel_Header);
	params_AppSettings.add(bAutoHide);

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

	// ofApp.cpp
	setup_ImGui();

	//--

	if (ENABLE_Keys_Player) addKeysListeners();

	//-

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	mMidiParams.connect();
	mMidiParams.add(params_ControlRemote);
	mMidiParams.add(params_Preset);
	//TODO:
	//mMidiParams.add(presets.getParametersSelectorToggles());
#endif

	//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.setup();
	beatClock.bGui.setName("CLOCK");

	// callback to receive BeatTicks
	listenerBeat = beatClock.BeatTick.newListener([&](bool&) {this->Changed_BeatTick(); });
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

	// load settings
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
		presetsManager.addGroup(params_Preset);
		presetsManager.bGui.setName("PRESETS");
		presetsManager.bGui_InnerClicker = false;
		presetsManager.setClickerAmount(3);
#endif

		// add after created object

#ifdef INCLUDE__OFX_SURFING_PRESET__MIDI__
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		mMidiParams.add(presets.getParametersSelectorToggles());
#endif
#endif

		//-

		loadGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

		//-

		// overwritte default settings

		if (!bMODE_SkipLooped && !bMODE_SkipPowered) bMODE_SkipPowered = true;

		//-

//		// workaround: bc presets is initiated before
//#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
//		{
//			std::string _path = ofToString("ofxSurfingPresets/" + videoName.get());
//			auto ss = ofSplitString(_path, ".");
//			if (ss.size() > 0) _path = ss[0];
//			presets.setPathPresets(_path);
//		}
//#endif
		//-
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
	player.setVolume(0.0f);

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
	//// fbo
	//fboPreview.begin();
	//ofClear(0, 255);
	//player.draw(0, 0, fboPreview.getWidth(), fboPreview.getHeight());
	//fboPreview.end();

	surfingPreview.begin();
	player.draw(0, 0);
	surfingPreview.end();

	//--

	// gui
	updateVideoPlayer();

	//--

//#ifdef USE_ofxSurfingMoods
//	surfingMoods.update();
//#endif

	//--

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	//if(0)
	if (presetsManager.isDoneLoad())
	{
		ofLogNotice(__FUNCTION__) << "Preset Loaded";

		if (player.isLoaded())
		{
			POSITION = POSITION_Start;
			player.setPosition(POSITION);

			//-

			//// workflow
			//if (!bMODE_Loop) bMODE_Loop = true;
		}
	}
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::calculateKick()
{
	// kick-drift
	// calculate desired step size
	// by left-right arrow keys

	// just try, bc if video is not loaded will need to do it again
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
void ofxSurfingVideoSkip::updateVideoPlayer()
{
	POSITION = player.getPosition();

	//--

	if (!bMODE_Edit)
	{
		// mantain start must be before finish
		if (POSITION_Start == POSITION_End) {
			POSITION_End += kickSizeFrame;
		}
		else if (POSITION_Start > POSITION_End) {
			float temp = POSITION_End;
			POSITION_Start = POSITION_End;
			POSITION_End = temp;
		}
	}

	//--

	//TODO:
	// Do only once!
	// if load video fails totalNumFrames will be 0, then we will retry every x secconds!
	if (totalNumFrames == 0 && ofGetFrameNum() % (60 * 5) == 0)
	{
		calculateKick();
	}

	//--

	// display current time in minute:seconds. max minutes is 99 to nice formatting
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

	// check if player is running
	bool isPlaying = player.isPlaying() && !player.isPaused();

	//TODO:
	//BUG: last/first frame get flicked..

	// playing. player running
	if (isPlaying)
	{
		//cout << "isPlaying: " << isPlaying << endl;

		// loop engine
		//TODO: 
		// workflow: added !bMODE_Edit to allow playing out of range loop
		// but requires to imrpove workflow when playing preset with EDIT MODE enabled

		if (bMODE_Loop)// && !bMODE_Edit)
		{
			if (player.getPosition() >= POSITION_End)
			{
				if (!bMODE_LoopedBack)
				{
					if (bMODE_Reversed) POSITION = POSITION_End;
					else POSITION = POSITION_Start;
				}
				else
				{
					POSITION = POSITION_End;
					bMODE_Reversed = !bMODE_Reversed;
				}

				player.setPosition(POSITION);
			}
			else if (player.getPosition() <= POSITION_Start)//? < only
			{
				if (!bMODE_LoopedBack)
				{
					if (bMODE_Reversed) POSITION = POSITION_End;
					else POSITION = POSITION_Start;
				}
				else
				{
					POSITION = POSITION_Start;
					bMODE_Reversed = !bMODE_Reversed;
				}

				player.setPosition(POSITION);
			}
		}
	}

	else // not playing. player stopped
	{
		if (!inScrub && !ENABLE_TimersGlobal && !player.isPaused())
		{
			//clamp cursor into loop
			if (bMODE_Loop && !bMODE_Edit)
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

	//if (ofGetElapsedTimeMillis() - lastMovement < time_autoHide)
	//{
	//	bGui_VideoControlBar_PRE = bGui_VideoControlBar;
	//	bGui_VideoControlBar = true;
	//}
	//else
	//{
	//	if (bAutoHide)
	//	{
	//		bGui_VideoControlBar = false;
	//		bGui_VideoControlBar_PRE = bGui_VideoControlBar;
	//	}
	//}

	//--

	//TODO
	// disable to avoid bug with clicks l/r on gui (ofxGuiExtended2)
	// hide mouse if changed
	if ((bGui_VideoControlBar != bGui_VideoControlBar_PRE) || bAutoHide)
	{
		ofRectangle window = ofGetWindowRect();
		if (!bGui_VideoControlBar && window.inside(ofGetMouseX(), ofGetMouseY()))
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
	//-

	// Mode beat

	//TODO:
	// this is wrong bc time and durations depends of play speed!
	if (bMODE_Beat)
	{
		int videoDur = player.getDuration() * 1000;//video duration in ms
		int tDur = beatRescale * ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		float tDurN = (float)tDur / (float)videoDur;//duration in normalized decimals 

		POSITION_End = POSITION_Start + tDurN;
	}

	//--

	// Timers

	if (ENABLE_TimersGlobal && bMODE_SkipTime && !inScrub && bPlay)
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
			bTRIG_SkipTime = true;

			//// workflow
			//if (bMODE_SkipReverse)
			//{
			//	last_TRIG_reverse = ofGetElapsedTimeMillis();
			//	bTRIG_SkipReverse = true;
			//}
		}

		timer_SkipTime = MAX(0, t / (float)tmax);
	}

	//-

	// Reverse

	if (ENABLE_TimersGlobal && bMODE_SkipReverse && !inScrub && bPlay)
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

			// trig jump skip
			bTRIG_SkipReverse = true;
		}

		timer_SkipRev = MAX(0, t / (float)tmax);
	}

	//--

	// A - B Trigers

	// Time trigger

	if (bTRIG_SkipTime == true)
	{
		bTRIG_SkipTime = false;

		float skipPos;

		if (bMODE_SkipLooped)
		{
			if (bMODE_Loop)
			{
				skipPos = ofRandom(POSITION_Start, POSITION_End);
			}
			else
			{
				skipPos = ofRandom(0.0, 1.0);
			}
		}
		else if (bMODE_SkipPowered)
		{
			float factor = 0.25f;
			float pow = factor * ofRandom(-skipPower, skipPower);
			skipPos = POSITION + pow;

			// clamp
			if (bMODE_Loop)
			{
				skipPos = ofClamp(skipPos, POSITION_Start, POSITION_End);
			}
			else
			{
				skipPos = ofClamp(skipPos, 0, 1);
			}
		}

		// apply
		player.setPosition(skipPos);
	}

	//--

	// Reverse Trigger

	if (bTRIG_SkipReverse == true)
	{
		bTRIG_SkipReverse = false;

		//if (bMODE_SkipReverse)
		{
			bMODE_Reversed = !bMODE_Reversed;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Gui()
{
	draw_ImGui();

	//#ifdef USE_ofxSurfingMoods
	//	surfingMoods.draw();
	//#endif

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
		bGui_VideoControlBar_PRE = bGui_VideoControlBar;
		//bGui_VideoControlBar = true;
		lastMovement = ofGetElapsedTimeMillis();
	}

	// draw control bar
	//bGui_VideoControlBar = b;

	//-

	// presets
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
//	surfingMoods.setGui_Visible(b && SHOW_MoodMachine);
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
	if (bMODE_Edit)
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
				if (bMODE_Loop && !bMODE_Edit) bMODE_Loop = false;
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
			bool wasPlaying = bPlay;

			ofLogVerbose(__FUNCTION__) << "mousePressed INSIDE: (" << x << "," << y << ")";

			inScrub = true;
			wasPaused = player.isPaused() || player.getIsMovieDone();

			// workflow
			//auto-stop
			if (bPlay)
			{
				bPlay = false;
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

			if (wasPlaying) bPlay = true;
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
			//bPlay = false;
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
//	surfingMoods.setPosition(gPos.x + gWidth + xPad, gPos.y);//right to the user panel
//
//	//float pw = 400;
//	//float px = window_W * 0.5f - pw * 0.5f;//center on screen
//	//surfingMoods.setPreviewPosition(px, 20, pw, 40);//custom preview position
//#endif

	//-

//#ifdef USE_ofxSurfingMoods
//	auto gGenWidth = surfingMoods.getGuiUserWidth();//right to the surfingMoods panel
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
//	auto gGenWidth = surfingMoods.getGuiUserWidth();//right to the surfingMoods panel
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

	// modifiers
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

	//guiManager.keyPressed(key);


	// ofxChannelFx
#ifdef USE_ofxChannelFx
	if (ENABLE_Keys_Fx)
	{
		channelFx.keyPressed(key);
	}
#endif

	//----

	// ofxSurfingVideoSkip
	if (ENABLE_Keys_Player)
	{
		//-

//		// main panels show/hide
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
//			surfingMoods.setPreviewToggleVisible();
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
//			bGui_VideoControlBar = !bGui_VideoControlBar;
//		}
//		else if (key == OF_KEY_F7)
//		{
//			bGui_ControlPanel = !bGui_ControlPanel;
//		}

		//----

		//TODO:
		//must add modifiers to avoid keys collapsing...
		//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);

		//if (ENABLE_Active)
		//if (MODE_App == 0)

		//-

		// transport

		// play-stop
		if (key == ' ')
		{
			setPlay(!bPlay);
		}

		else if (key == OF_KEY_RETURN)
		{
#ifdef USE_ofxSurfingMoods
			bool b = !surfingMoods.isPlaying();
			setPlay_MoodMachine(b);
			setPlay(b);
#endif
		}

		//-

		// loop mode
		else if (key == 'L' || key == 'l')
		{
			bMODE_Loop = !bMODE_Loop;
		}

		// edit mode
		else if (key == 'E' || key == 'e')
		{
			bMODE_Edit = !bMODE_Edit;

			//bMODE_Loop = !bMODE_Loop;
			////if (!bMODE_Loop && ENABLE_TimersGlobal)
			////{
			////	ENABLE_TimersGlobal = false;
			////}
		}

		// workflow
		// auto-enable edit mode if it's disabled
		if ((key == 'i' || key == 'o') && !bMODE_Edit)
		{
			bMODE_Edit = true;
		}

		// edit mode
		if (bMODE_Edit)
		{
			if (false) {}

			// store (set)
			else if (key == 'i')
			{
				bSET_START = true;
			}
			else if (key == 'o')
			{
				bSET_END = true;
			}

			// recall (go)
			else if (key == 'I')
			{
				POSITION = POSITION_Start;
			}
			else if (key == 'O')
			{
				POSITION = POSITION_End;
			}

			//// user kick-drift frame-by-frame
			//else if (key == OF_KEY_LEFT && bMODE_Edit && !mod_CONTROL)
			//{
			//	bKickL = true;
			//	//POSITION -= kickSizeFrame;
			//}
			//else if (key == OF_KEY_RIGHT && bMODE_Edit && !mod_CONTROL)
			//{
			//	bKickR = true;
			//	//POSITION += kickSizeFrame;
			//}
			//else if (key == OF_KEY_LEFT && bMODE_Edit && mod_CONTROL)
			//{
			//	POSITION -= 10 * kickSizeFrame;
			//}
			//else if (key == OF_KEY_RIGHT && bMODE_Edit && mod_CONTROL)
			//{
			//	POSITION += 10 * kickSizeFrame;
			//}

			// reset preset. basic settings only
			else if (key == 'r' || key == 'R')
			{
				bTRIG_ResetEngine = true;
			}
		}

		//-

		//// helpers triggers
		//else if (key == 's')
		//{
		//	bTRIG_SkipTime = true;
		//}
		//else if (key == 'S')
		//{
		//	ENABLE_TimersGlobal = !ENABLE_TimersGlobal;
		//}

		// autohide
		//else if (key == 'a' && !ENABLE_AutoHide_external)
		//{
		//	bAutoHide = !bAutoHide;
		//	if (bAutoHide)
		//	{
		//		//bGui_VideoControlBar=true;
		//		lastMovement = ofGetElapsedTimeMillis();
		//	}
		//}

		//-

		//-

		// sound
		else if (key == OF_KEY_UP)
		{
			player.setVolume(player.getVolume() + 0.1);
		}
		else if (key == OF_KEY_DOWN)
		{
			player.setVolume(player.getVolume() - 0.1);
		}

		//-

		// show/hide gui
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
void ofxSurfingVideoSkip::dragEvent(ofDragInfo dragInfo) // drag video to load another one
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
		//if (!bMODE_Loop) bMODE_Loop = true;
	}
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Params(ofAbstractParameter &e) // patch change
{
	if (!DISABLE_CALLBACKS)
	{
		std::string name = e.getName();

		if (name != "" &&
			name != bTRIG_SkipTime.getName() &&
			name != bTRIG_SkipReverse.getName() &&
			name != POSITION.getName() &&
			name != bMODE_Reversed.getName() &&
			name != timer_SkipTime.getName() &&
			name != timer_SkipRev.getName()
			)
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (name == bPlay.getName())
		{
			if (!bPlay) // stop 
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
				if (!bMODE_Loop)
				{
					bMODE_Loop = true;
				}

				//if (bMODE_Edit)
				//{
				//	bMODE_Edit = false;
				//}
			}
		}
		else if (name == bMODE_Edit.getName())
		{
			if (bMODE_Edit)
			{
				// workflow
				if (!bMODE_Loop) bMODE_Loop = true;

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
		else if (name == bMODE_Loop.getName())
		{
			if (bMODE_Loop)
			{
				player.setPosition(POSITION_Start);
			}
		}
		else if (name == bSET_START.getName() && bSET_START)
		{
			bSET_START = false;
			POSITION_Start = player.getPosition();

			if (bMODE_Edit)
			{
				// workflow
				//enable loop
				if (!bMODE_Loop) bMODE_Loop = true;

				// workflow
				// if start/end flipped: set finish one second to the right
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
			if (!bPlay /*&& bMODE_Edit*/)
			{
				POSITION = POSITION_Start;
			}
		}
		else if (name == POSITION_End.getName())
		{
			if (!bPlay /*&& bMODE_Edit*/)
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
			player.setSpeed((bMODE_Reversed ? (-1.0f) : (1.0f)) * speed);
		}
		else if (name == bReset_Speed.getName() && bReset_Speed)
		{
			bReset_Speed = false;
			speedNorm = 0.0f;
			//speed = 1.0f;
		}

		//-

		else if (name == bTRIG_ResetEngine.getName() && bTRIG_ResetEngine)
		{
			bTRIG_ResetEngine = false;

			ENABLE_TimersGlobal = false;
			bMODE_SkipTime = false;
			bMODE_SkipReverse = false;

			bMODE_LoopedBack = false;
			bMODE_Reversed = false;

			bReset_Speed = true;
			//speed = 1.0f;

			divBeatSkipper = 4;
			divBeatReverse = 2;

			bMODE_SkipLooped = true;
			skipPower = 0.5f;

		}
		else if (name == bTRIG_Reset_Bpm.getName())
		{
			bTRIG_Reset_Bpm = false;

			bpmTimer = 120.f;
			bpmDivider = 2;
		}

		else if (name == bMODE_Reversed.getName())
		{
			player.setSpeed((bMODE_Reversed ? (-1.0f) : (1.0f)) * speed);

			//if (bMODE_Reversed)
			//{
			//}
		}
		else if (name == bMODE_SkipTime.getName())
		{
			//// workflow
			//if (bMODE_SkipTime.get())
			//{
			//	if (!ENABLE_TimersGlobal) ENABLE_TimersGlobal = true;
			//}
		}
		else if (name == bMODE_SkipReverse.getName())
		{
			//// workflow
			//if (bMODE_SkipReverse.get()) {
			//	if (!ENABLE_TimersGlobal) ENABLE_TimersGlobal = true;
			//}
		}

		else if (name == bGui_SurfingVideo.getName())
		{
			bool b = bGui_SurfingVideo.get();

			//bGui_VideoControlBar = b;
		}

		//#ifdef USE_ofxSurfingMoods
		//		else if (name == SHOW_MoodMachine.getName())
		//		{
		//			surfingMoods.setGui_Visible(SHOW_MoodMachine);
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
			presetsManager.setVisible_PresetClicker(bGui_Presets);
		}
#endif
		//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
		else if (name == videoName.getName())
		{
			std::string _path = ofToString("ofxSurfingPresets/" + videoName.get());
			auto ss = ofSplitString(_path, ".");
			if (ss.size() > 0) _path = ss[0];
			ofStringReplace(_path, "\"", "");
			presetsManager.setPathPresets(_path);
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

		// advanced
		else if (name == bGui_Advanced.getName())
		{

		}

		//-

		// skip mode
		else if (name == bMODE_SkipLooped.getName())
		{
			bMODE_SkipPowered = !bMODE_SkipLooped.get();
		}
		else if (name == bMODE_SkipPowered.getName())
		{
			bMODE_SkipLooped = !bMODE_SkipPowered.get();
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

		//bGui_VideoControlBar_PRE = bGui_VideoControlBar;
		//bGui_VideoControlBar = true;
		//lastMovement = ofGetElapsedTimeMillis();
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
//	//	if (bAutoHide)
//	//	{
//	//		if (bGui_VideoControlBar != bGui_VideoControlBar_PRE)
//	//		{
//	//			ofLogNotice(__FUNCTION__) << "Changed_draw_Autohide: " << ofToString(bGui_VideoControlBar ? "TRUE" : "FALSE");
//	//
//	//			if (MODE_App == 0)
//	//			{
//	//				if (!bGui && bGui_VideoControlBar)
//	//				{
//	//					bGui_VideoControlBar_PRE = bGui_VideoControlBar;
//	//					bGui_VideoControlBar = false;
//	//				}
//	//				setGuiVisible(bGui_VideoControlBar);
//	//
//	//#ifdef USE_ofxSurfingMoods
//	//				//surfingMoods.setGui_visible(bGui_VideoControlBar&&SHOW_MoodMachine);
//	//#endif
//	//			}
//	//			else if (MODE_App == 1)
//	//			{
//	//				if (!bGui && bGui_VideoControlBar)
//	//				{
//	//					bGui_VideoControlBar_PRE = bGui_VideoControlBar;
//	//					bGui_VideoControlBar = false;
//	//				}
//	//			}
//	//		}
//	//
//	//		//hide/show cursor
//	//		if (!bGui_VideoControlBar)
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

//surfingMoods
//listeners for inside surfingMoods

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
	presetsManager.load(targetVal);
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
	ofClear(surfingPreview.colorBg);

	//--

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

		// draw processed
		channelFx.draw();
	}

	// raw clean
	else
#endif
	{
		ofEnableArbTex();
		draw_Video();
	}

	//--

	if (bGui)
	{
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		mMidiParams.draw();
#endif
		draw_Gui();
	}

	//--

	surfingPreview.draw();

	////ofSetColor(ofColor(ofColor::red, 128));
	////ofDrawRectangle(rectDraggable);
	//rectDraggable.draw();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Video()
{
	if (surfingPreview.bBigScreen)
	{
		if (player.isLoaded())
		{
			ofPushStyle();
			{
				// Get player rect
				ofRectangle r(0, 0, player.getWidth(), player.getHeight());

				//-

				// Re scale rect
				if (surfingPreview.bFullScreen)
				{
					r.scaleTo(ofGetWindowRect(), surfingPreview.scaleMode); // Full view
				}
				else
				{
					// Draggable viewport
					if (surfingPreview.bInDocked)
					{
						r.scaleTo(guiManager.getRectangleCentralDocking(), surfingPreview.scaleMode);
						surfingPreview.updateRectDraggable(r);
					}
					else // Inner central docking view rect
					{
						r.scaleTo(surfingPreview.rectDraggable, surfingPreview.scaleMode);
					}
				}

				//--

				// Draw video frame
				ofSetColor(255, 255, 255, 255);

				player.draw(r.x, r.y, r.width, r.height);
			}
			ofPopStyle();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_VideoControls()
{
	if (player.isLoaded())
	{
		int alphaBar = 128;
		float roundB = 5.0f;

		// Draw the position bar if appropriate
		if (bGui_VideoControlBar)
		{
			ofPushStyle();

			//-

			// 1. Border rect only. full video timeline
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

			//// 2. Filled rectangle from time 0 to current time position
			////ofDrawRectangle(barCurTime);
			//ofDrawRectRounded(barCurTime, roundB);

			int yy = ofGetWindowHeight() - BarInset - BarHeight;

			// 3. Loop clip
			// Don't draw loop bar if loop not enable
			if (bMODE_Loop)
			{
				// 3. Markers loop rectangle: from start to end
				ofSetColor(ofColor(255), 192); // lighter grey
				//ofSetColor(ofColor(64), 192); // darker grey
				int pStart, pWidth;
				int padding = 2;
				pStart = BarInset + barFull.width * POSITION_Start;
				pWidth = (BarInset + barFull.width * POSITION_End) - pStart;
				ofRectangle barLoop = ofRectangle(pStart, yy + padding, pWidth, BarHeight - padding * 2);
				ofDrawRectangle(barLoop);

				// 3.2 Red line markes to loop
				if (bMODE_Edit)
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

			// 4. Red line for current video player time
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
		ofLogError(__FUNCTION__) << "VIDEO NOT FOUND '" << _path << "' z!";

		_path = "movies/SampleHap.mov";
		ofLogWarning(__FUNCTION__) << "...TRYING TO LOAD A BACKUP MOVIE: '" << _path << "' !";

		b = player.load(_path);
		if (!b) ofLogError(__FUNCTION__) << "BAD ERROR!";
	}
	bLoaded = b;

	// workflow
	if (!bPlay) bPlay = true;

	// to fix startup bug..
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	{
		std::string _path = ofToString("ofxSurfingPresets/" + videoName.get());
		auto ss = ofSplitString(_path, ".");
		if (ss.size() > 0) _path = ss[0];
		ofStringReplace(_path, "\"", "");
		presetsManager.setPathPresets(_path);
	}
#endif

	if (!bLoaded) {
		// TODO: + fix presets path empty on startup
		// skipper engine set without name
		// bc settings could store last file path but maybe could be moved,
		// so last path is wrong! That happens when moving the video file...
		videoFilePath.set("NO FILE");
		videoName.set("NO FILE");
		videoTIME.set(""); // current time position
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

//--------------------------------------------------------------
void ofxSurfingVideoSkip::exit()
{
	// save app settings
	saveGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

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

	// disable for the momment
	//ofRemoveListener(params_Control_VideoFX.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_params_VideoFX);

	//// auto hide callback
	//bGui_VideoControlBar.removeListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//-

	// presetsManager
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.DONE_save.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_save);
	presetsManager.DONE_load.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_load);
	presetsManager.exit();
#endif

	//-

#ifdef USE_ofxSurfingMoods
	// listener for inside class surfingMoods
	surfingMoods.RANGE_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	surfingMoods.TARGET_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	surfingMoods.PRESET_A_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	surfingMoods.PRESET_B_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	surfingMoods.PRESET_C_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);
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
	// TODO: minimal. allow disable all layouting..
#define MODE_MINIMAL_IM_GUI

//#ifdef MODE_MINIMAL_IM_GUI
//	guiManager.setup(IM_GUI_MODE_INSTANTIATED);
	//guiManager.setup(IM_GUI_MODE_INSTANTIATED_SINGLE);
//#endif

	//// -> Required to allow custom docking layout. 
	//// instead of the default centralized.
	//guiManager.setAutoSaveSettings(true);
	//guiManager.setImGuiDocking(true);
	//guiManager.setImGuiDockingModeCentered(true);
	//guiManager.setImGuiAutodraw(true);
	//guiManager.setSharedMode(true);

	guiManager.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);

	//-

//#ifndef MODE_MINIMAL_IM_GUI
	// Customize default labels
	guiManager.setLabelLayoutPanels("SURFING_VIDEO"); // global title
	//guiManager.setLabelLayoutMainWindow("Layout Manager");
	//guiManager.bGui_LayoutsManager.setName("Layout Manager");
//#endif

	//guiManager.setup();//?

	//-

	// -> Layouts presets
	// this bool toggles will control the show of the added window
	// and will be added too to layout presets engine
	guiManager.addWindowSpecial(bGui_SurfingVideo);

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	guiManager.addWindowSpecial(presetsManager.bGui);
#endif

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	guiManager.addWindowSpecial(mMidiParams.bGui);
#endif

#ifdef USE_ofxSurfingMoods 
	guiManager.addWindowSpecial(surfingMoods.bGui);
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	guiManager.addWindowSpecial(beatClock.bGui);
#endif

	//-

	// -> Extra params to include into layout presets
//#ifndef MODE_MINIMAL_IM_GUI
	//guiManager.addParameterToLayoutPresets(rectDraggable.getParameter());

	guiManager.addParameterToLayoutPresets(surfingPreview.getParameters());//?group
	//guiManager.addParameterToLayoutPresets(surfingPreview.bGui);
//#endif

	//-

	// -> Initiates when adding finished
	guiManager.startup();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiSkipTimers()
{
	// panels sizes
	float xx = 10;
	float yy = 10;
	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT;

	static ofParameter<bool> _bAutoResize{ "Auto Resize", true };

	//-

	std::string n;

	//-

	// windows
	{
		n = "SKIP TIMERS";
		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (_bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		guiManager.beginWindow(n.c_str(), (bool*)&bGui_SkipTimers.get(), flagsw);
		{
			bool bOpen;
			ImGuiTreeNodeFlags _flagt;

			refreshLayout();

			// manual triggers
			guiManager.Add(bTRIG_SkipTime, OFX_IM_BUTTON_BIG, 2, true);
			guiManager.Add(bTRIG_SkipReverse, OFX_IM_BUTTON_BIG, 2, false);

			ofxImGuiSurfing::AddSpacingSeparated();
			//ImGui::Spacing();

			//--

			// Big enablers
			guiManager.Add(ENABLE_TimersGlobal, OFX_IM_TOGGLE_BIG_XXL_BORDER);

			ImGui::Spacing();
			//ImGui::Spacing();

			//--

			//if (ENABLE_TimersGlobal && (bMODE_SkipReverse || bMODE_SkipTime)) 
			if (ENABLE_TimersGlobal)
			{
				bOpen = false;
				_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagt |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::TreeNodeEx("BPM CLOCK", _flagt))
				{
					refreshLayout();

					guiManager.Add(bpmTimer, OFX_IM_DEFAULT);
					guiManager.Add(bpmDivider, OFX_IM_DEFAULT);

					if (ImGui::Button("HALF", ImVec2(_w50, _h / 2))) {
						bpmTimer /= 2.0f;
					}
					ImGui::SameLine();

					if (ImGui::Button("DOUBLE", ImVec2(_w50, _h / 2))) {
						bpmTimer *= 2.0f;
					}

					// reset
					guiManager.Add(bTRIG_Reset_Bpm, OFX_IM_BUTTON_SMALL);

					ImGui::TreePop();
				}

				//ImGui::Spacing();
				ofxImGuiSurfing::AddSpacingSeparated();

				//--

				//ofxImGuiSurfing::AddBigToggle(bMODE_SkipTime);
				//ofxImGuiSurfing::AddBigToggle(bMODE_SkipReverse);
				guiManager.Add(bMODE_SkipTime, OFX_IM_TOGGLE_BIG_XXL_BORDER);

				if (bMODE_SkipTime)
					if (ENABLE_TimersGlobal)
					{
						bOpen = true;
						_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
						_flagt |= ImGuiTreeNodeFlags_Framed;
						if (ImGui::TreeNodeEx("A SKIP TIME", _flagt))
						{
							refreshLayout();

							//guiManager.Add(bTRIG_SkipTime, OFX_IM_BUTTON_BIG, 1, false);

							if (bMODE_SkipTime && ENABLE_TimersGlobal)
							{
								//guiManager.Add(divBeatSkipper, OFX_IM_DEFAULT);
								ofxImGuiSurfing::AddParameter(divBeatSkipper);

								// draw progress bar
								////guiManager.Add(timer_SkipTime, OFX_IM_DEFAULT);
								//ofxImGuiSurfing::AddProgressBar(timer_SkipTime, true);
								guiManager.Add(timer_SkipTime, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							guiManager.Add(bMODE_SkipLooped, OFX_IM_TOGGLE_SMALL, 2, true);
							guiManager.Add(bMODE_SkipPowered, OFX_IM_TOGGLE_SMALL, 2, false);
							if (bMODE_SkipPowered) {
								guiManager.Add(skipPower);
							}

							ImGui::TreePop();
						}
						refreshLayout();
					}

				//-

				ofxImGuiSurfing::AddSpacingSeparated();

				guiManager.Add(bMODE_SkipReverse, OFX_IM_TOGGLE_BIG_XXL_BORDER);
				//guiManager.Add(bMODE_SkipTime, OFX_IM_TOGGLE_BIG, true, 0);
				//guiManager.Add(bMODE_SkipReverse, OFX_IM_TOGGLE_BIG, false, 0);

				if (bMODE_SkipReverse)
					if (ENABLE_TimersGlobal)
					{
						if (ImGui::TreeNodeEx("B SKIP REV", _flagt))
						{
							refreshLayout();

							//guiManager.Add(bTRIG_SkipReverse, OFX_IM_BUTTON_BIG, 1, false);

							if (bMODE_SkipReverse && ENABLE_TimersGlobal)
							{
								//guiManager.Add(divBeatReverse, OFX_IM_DEFAULT);
								ofxImGuiSurfing::AddParameter(divBeatReverse);

								// draw progress bar
								////guiManager.Add(timer_SkipRev, OFX_IM_DEFAULT);
								//ofxImGuiSurfing::AddProgressBar(timer_SkipRev, true);
								guiManager.Add(timer_SkipRev, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							ImGui::TreePop();
						}
						refreshLayout();
					}

				ofxImGuiSurfing::AddSpacingSeparated();

				//if (!ENABLE_TimersGlobal)
				//	ImGui::Spacing();

				guiManager.Add(bTRIG_ResetEngine, OFX_IM_BUTTON_SMALL);
			}

			//--

			ImGui::Spacing();
			ofxImGuiSurfing::AddToggleRoundedButton(_bAutoResize);
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiPreview()
{
	surfingPreview.draw_ImGuiPreview();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiControls()
{
	std::string n;

	ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
	if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT * 2;

	ImGuiCond flagsCond = ImGuiCond_None;
	flagsCond |= ImGuiCond_Appearing;
	//flagsCond |= ImGuiCond_FirstUseEver; 

	ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
	//ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

	//--

	// Engine

	// Skip player window
	{
		n = bGui_SurfingVideo.getName();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		guiManager.beginWindow(n.c_str(), (bool*)&bGui_SurfingVideo.get(), flagsw);
		{
			refreshLayout();

			// minimize
			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
			ImGui::Spacing();

			// video tittle
			if (!guiManager.bMinimize)
			{
				// File
				bool bOpen = false;
				ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagt |= ImGuiTreeNodeFlags_Framed;
				std::string n = videoName;

				//if (!guiManager.bMinimize)
				if (ImGui::TreeNodeEx(n.c_str(), _flagt))
				{
					refreshLayout();

					if (ImGui::Button("OPEN FILE", ImVec2(_w100, _h / 2)))
					{
						doOpenDialogToSetPath();
					}

					ImGui::Text(videoFilePath.get().data());
					ImGui::TreePop();
				}
			}

			//--

			{
				refreshLayout();

				ImGui::Spacing();

				//-

				// Play

				//guiManager.Add(bPlay, OFX_IM_TOGGLE_BIG_XXXL_BORDER_BLINK);
				////guiManager.Add(bPlay, OFX_IM_TOGGLE_BIG);
				{
					float _w1 = ofxImGuiSurfing::getWidgetsWidth(1);
					float _w2 = ofxImGuiSurfing::getWidgetsWidth(2);
					float _h = ofxImGuiSurfing::getWidgetsHeightUnit();
					ofxImGuiSurfing::AddBigToggleNamed(bPlay, _w1, 3.0f * _h, "PLAYING", "PLAY", true);
				}

				//-

				ImGui::Spacing();

				guiManager.Add(bMODE_LoopedBack, OFX_IM_TOGGLE_SMALL, 2, true);
				guiManager.Add(bMODE_Reversed, OFX_IM_TOGGLE_SMALL, 2, false);

				//-

				// Speed

				//guiManager.Add(speedNorm, OFX_IM_KNOB, 4);
				//TODO:			
				guiManager.Add(speedNorm, OFX_IM_SLIDER);
				//guiManager.Add(speed, OFX_IM_SLIDER);
				guiManager.Add(bReset_Speed, OFX_IM_BUTTON_SMALL);

				//ImGui::Spacing();
				AddSpacingSeparated();
				ImGui::Spacing();
				ImGui::Spacing();

				//-

				ImGui::Text(("Time " + videoTIME.get()).data());
				ImGui::Spacing();

				//-

				// Position bigger

				//guiManager.Add(POSITION, OFX_IM_HSLIDER_NO_LABELS);

				ofParameter<float> p = POSITION.cast<float>();
				auto tmpRef = p.get();
				std::string name = p.getName();

				float __h = getWidgetsHeightRelative(); // relative to theme
				float _ww = ImGui::GetContentRegionAvail().x; // full window panel width

				bool bReturn;
				if (bMODE_Loop)
					bReturn = (ofxImGuiSurfing::AddHSliderRanged2(p, ImVec2(_ww, __h), POSITION_Start, POSITION_End, true, true));
				else
					bReturn = (ofxImGuiSurfing::AddHSlider(p, ImVec2(_ww, __h), true, true));

				if (bReturn)
				{
					POSITION = p;
				}
				IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(p);

				ImGui::Spacing();

				//-

				// Loop
				guiManager.Add(bMODE_Loop, OFX_IM_TOGGLE_BIG, 1, false);

				//-

				// Range
				if (bMODE_Loop)
				{
					// Start/Finish
					ImGui::Spacing();
					ImGui::Text("START END");
					guiManager.Add(POSITION_Start, OFX_IM_HSLIDER_MINI_NO_LABELS);
					guiManager.Add(POSITION_End, OFX_IM_HSLIDER_MINI_NO_LABELS);
					ImGui::Spacing();
				}

				//-

				// Beat mode
				if (!guiManager.bMinimize)
					if (bMODE_Loop)
					{
						ImGui::Spacing();
						guiManager.Add(bMODE_Beat, OFX_IM_TOGGLE_SMALL_BORDER, 2, true);
						guiManager.Add(bMODE_Lock, OFX_IM_TOGGLE_SMALL_BORDER, 2, false);
						if (bMODE_Beat) {
							ofxImGuiSurfing::AddParameter(beatDuration);
							ofxImGuiSurfing::AddParameter(beatRescale);
						}
						ImGui::Spacing();
					}

				//--

				//if (bMODE_Loop)
				{
					// Preset Clicker
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
					presetsManager.draw_ImGui_MiniClicker();
#endif
					refreshLayout();

					if (!guiManager.bMinimize)
						//if (bMODE_Edit)
					{
						// TODO:
						// disable range bc is too small and duplicated from the above controls

						/*
						refreshLayout();

						// Position
						guiManager.Add(POSITION, OFX_IM_SLIDER);

						// Range
						if (bMODE_Loop) {
							ofxImGuiSurfing::AddRangeParam("LOOP", POSITION_Start, POSITION_End, "%.3f      %.3f", 1.0f, ImVec2(-1, -1), true);
						}

						ImGui::Spacing();
						*/

						//--

						if (!guiManager.bMinimize)
						{
							ImGui::Spacing();

							// finetune
							static bool bFineTune = false;
							ToggleRoundedButton("FineTune", &bFineTune, ImVec2(30, 20));
							ImGui::Spacing();

							if (bFineTune)
							{
								ImGui::Indent();
								{
									refreshLayout();

									ImGui::Text(("Frame " + videoFRAME.get()).data());
									ImGui::Spacing();
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

									guiManager.Add(POSITION, OFX_IM_STEPPER);

									if (bMODE_Loop) {
										guiManager.Add(POSITION_Start, OFX_IM_STEPPER);
										guiManager.Add(POSITION_End, OFX_IM_STEPPER);
									}

									// Mark clip start/end
									//if (bMODE_Edit)
									if (bMODE_Loop)
									{
										guiManager.Add(bSET_START, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bSET_END, OFX_IM_BUTTON_SMALL, 2, false);
										ImGui::Spacing();
									}
								}
								ImGui::Unindent();
							}

							//ImGui::Unindent();
						}
					}
				}

				//-

				if (!guiManager.bMinimize)
				{
					AddSpacingSeparated();
					ImGui::Spacing();

					guiManager.Add(bMODE_Edit, OFX_IM_TOGGLE_BIG_BORDER_BLINK);

					//ImGui::Indent();
				}

				//-

				ImGui::Spacing();
				AddSpacingSeparated();

				//guiManager.Add(bMODE_Edit, OFX_IM_TOGGLE_BIG_BORDER);

				//refreshLayout();

				//--

				// Loop
				//guiManager.Add(bMODE_Loop, OFX_IM_TOGGLE_BIG, 1, false);
				//guiManager.Add(bMODE_LoopedBack, OFX_IM_TOGGLE_SMALL, 2, true);
				//guiManager.Add(bMODE_Reversed, OFX_IM_TOGGLE_SMALL, 2, false);

				//--

				// Skippers
				//guiManager.Add(bGui_SkipTimers, OFX_IM_TOGGLE_BIG);
				//ofxImGuiSurfing::AddToggleRoundedButton(bGui_SkipTimers);

			}//loop

			//--

			// minimize
			//refreshLayout();
			{
				ImGui::Spacing();

				//if (!guiManager.bMinimize)
				{
					ImGui::Spacing();

					// Skip Panel
					guiManager.Add(bGui_SkipTimers, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					ImGui::Spacing();
				}

				//-

				ImGui::Spacing();

				if (!guiManager.bMinimize)
				{
					static ofParameter<bool> bScreens{ "Screens", false };
					//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
					//guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					guiManager.Add(bScreens, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					//if (!guiManager.bMinimize)
					if (bScreens)
					{
						ImGui::Indent();

						// Preview
						guiManager.Add(surfingPreview.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
						guiManager.Add(surfingPreview.bBigScreen, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
						if (surfingPreview.bBigScreen)
						{
							//guiManager.Add(surfingPreview.bInDocked, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							// Control Bar
							guiManager.Add(bGui_VideoControlBar, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						}

						ImGui::Unindent();
					}
				}
			}

			//-

			if (!guiManager.bMinimize)
			{
				ImGui::Spacing();

				// extra
				guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
				if (guiManager.bExtra)
				{
					ImGui::Indent();
					{
						refreshLayout();

						ofxImGuiSurfing::AddToggleRoundedButton(ENABLE_Keys_Player);
						ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);
						ofxImGuiSurfing::AddToggleRoundedButton(bAutoHide);
						//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);

						//guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_SMALL);
						//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
						//guiManager.bAdvanced = guiManager.bExtra;

						//--

						//TODO:

#ifdef INCLUDE__OFX_SURFING_PRESET__MIDI__
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
						static bool bPopulateMidiToggles = false;
						if (ofxImGuiSurfing::ToggleRoundedButton("Populate Midi", &bPopulateMidiToggles))
						{
							mMidiParams.add(presets.getParametersSelectorToggles());
						}
#endif
#endif
						//--

						// Extra panel
						guiManager.drawAdvanced(true, true);
			}
					ImGui::Unindent();
		}
	}
}

		//ImGui::PopItemWidth();
}
	guiManager.endWindow();
}
//}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui()
{
	if (!bGui) return;

	//-

	guiManager.begin();
	{
		if (bGui_SurfingVideo) draw_ImGuiControls();

		if (bGui_SurfingVideo) draw_ImGuiPreview();

		if (bGui_SkipTimers && bGui_SurfingVideo) draw_ImGuiSkipTimers();

		//if (bGui_SkipTimers) draw_ImGuiSkipTimers();

		//--

		guiManager.beginDocking();
		{
			//dockingPopulate(); // -> initialize and "bypass" layout presets system if required

			//--

			//if (bDockingReset)
			//{
			//	bDockingReset = false;
			//	dockingReset();
			//}

			//--

			//if (guiManager.bGui_Menu) draw_ImGuiMenu();
		}
		guiManager.endDocking();
	}
	guiManager.end();

	//-

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	presetsManager.draw();
#endif

#ifdef USE_ofxSurfingMoods
	surfingMoods.draw_ImGui();
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.draw();
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::doOpenDialogToSetPath()
{
	ofLogNotice(__FUNCTION__) << "Set presets path";

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
	//if (beatClock.BeatTick)
	{
		ofLogNotice(__FUNCTION__) << "Beat #" << beatClock.getBeat();

#ifdef USE_ofxSurfingMoods
		surfingMoods.doBeatTick();
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_BeatBpm()
{
	ofLogNotice(__FUNCTION__) << "Bpm " << beatClock.getBpm();

	bpmTimer.set(beatClock.getBpm());

#ifdef USE_ofxSurfingMoods
	surfingMoods.setBpm(beatClock.getBpm());
#endif
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::dockingReset() // not works on runtime..?
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//ImGuiID dockspace_id = ImGui::GetID("DockSpace");

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
	ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

	// split the dockspace into 2 nodes --
	// DockBuilderSplitNode takes in the following args in the following order
	//   window ID to split, direction, fraction (between 0 and 1),
	// the final two setting let's us choose which id we want (which ever one we DON'T set as NULL,
	// will be returned by the function)
	// out_id_at_dir is the id of the node in the direction we specified earlier,
	// out_id_at_opposite_dir is in the opposite direction
	auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
	auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
	auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
	auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
	//auto dock_id_left2 = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.2f, nullptr, &dock_id_left);
	//auto dock_id_down2 = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.15f, nullptr, &dock_id_down);

	// we now dock our windows into the docking node we made above

	//ImGui::DockBuilderDockWindow("Window 1", dock_id_top);
	//ImGui::DockBuilderDockWindow("Window 2", dock_id_right);
	//ImGui::DockBuilderDockWindow("Window 3", dock_id_left);
	//ImGui::DockBuilderDockWindow("Window 4", dock_id_down);

	//ImGui::DockBuilderDockWindow("Window 0", dock_id_right);
	//ImGui::DockBuilderDockWindow("Main Window", dock_id_left);
	//ImGui::DockBuilderDockWindow("Layouts Presets", dock_id_left);
	//ImGui::DockBuilderDockWindow("Panels", dock_id_top);

	ImGui::DockBuilderFinish(dockspace_id);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiMenu()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool* p_open = NULL;

	//-

	// menu bar

	// This is not operative. just for testing menus!

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ofxImGuiSurfing::AddTooltipHelp(
			"This is not operative here. Just for testing menus!" "\n\n"
			"When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
			"- Drag from window title bar or their tab to dock/undock." "\n"
			"- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
			"- Hold SHIFT to disable docking." "\n"
			"This demo app has nothing to do with it!" "\n\n"
			"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
			"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
			"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
		);

		ImGui::EndMenuBar();
	}
}