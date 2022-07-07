#include "ofxSurfingVideoSkip.h"

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_PresetsStuff()
{
	//--

	// Load settings

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_Preset);

	//#ifdef USE_ofxSurfingFxChannel
	//		loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_ChannelFX);
	//#endif
#endif

	//--

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

	// Customize before add the group! that will call setup()
	presetsManager.setKeyFirstChar('a'); // customize keys to avoid collide when using multiple presets manager instances!
	//presetsManager.bGui.setName("SKIP PRESETS"); //-> Custom name
	//presetsManager.setEnableHandleSettingsFile(false); // disable saving settings to avoid collide with Gui Manager
	presetsManager.setPathGlobal(path_GLOBAL_Folder); // custom path
	presetsManager.setPathPresets(path_GLOBAL_Folder + "/" + "ofxSurfingVideoSkip" + "/" + "Presets");
	//presetsManager.setAmountButtonsPerRowClickers(3);
	presetsManager.setEnableKeySpace(false); // disable space key bc used by play toggle
	//presetsManager.setEnableKeysArrows(false); // disable arrows browse keys
	//presetsManager.bGui_InnerClicker = false;

	//-

	// -> The parameters to include on each preset!
	presetsManager.addGroup(params_Preset);

	//-

	// Link to not auto save when non editing!
	presetsManager.bEditMode.makeReferenceTo(bMODE_Edit);
	//presetsManager.bAutoSave.setName("EDIT");
	//bMODE_Edit.setName("EDIT");
	//bMODE_Edit.makeReferenceTo(presetsManager.bAutoSave);

#endif

	//--

	// We want to remote the preset controller vars.
	// Add after created presets manager object.

#ifdef INCLUDE__OFX_SURFING_PRESET__MIDI__
#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	mMidiParams.add(presetsManager.getParametersSelectorToggles());
#endif
#endif

	//--

	// App Session Settings

	loadGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_Preset()
{
	// Prepare params to save

	// name will be use to prefix file names at /group folder
	params_Preset.setName("CLIPS");

	params_Preset.add(POSITION_Start);
	params_Preset.add(POSITION_End);

	//params_Preset.add(bReset_Speed);
	params_Preset.add(speedNorm);
	//params_Preset.add(speed);

	//params_Preset.add(bMODE_Loop);//exclude. allways loop mode

	params_Preset.add(bMODE_LoopedBack);
	params_Preset.add(bMODE_Reversed);
	params_Preset.add(bENABLE_TimersGlobal);

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
	params_Preset.add(beatDuration);
	params_Preset.add(beatRescale);
	//params_Preset.add(bMODE_Lock);


	// should add more params like direction/reverse/..
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_Remote()
{
	//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.setup();
	beatClock.bGui.setName("CLOCK");

	// callback to receive BeatTicks
	listenerBeat = beatClock.BeatTick.newListener([&](bool&) {this->Changed_BeatTick(); });
	listenerBpm = beatClock.BPM_Global.newListener([&](float&) {this->Changed_BeatBpm(); });
#endif

	//--

	// External Control
	// to link to OSC/midi control

#ifdef USE_ofxRemoteParameters
	params_Remote.setName("VideoSkip REMOTE");

	params_Remote.add(bPlay);

	params_Remote.add(videoName);
	params_Remote.add(POSITION);
	params_Remote.add(videoTIME);

	params_Remote.add(speedNorm);
	params_Remote.add(bReset_Speed);

	params_Remote.add(bMODE_LoopedBack);
	params_Remote.add(bMODE_Reversed);

	params_Remote.add(POSITION);
	params_Remote.add(POSITION_Start);
	params_Remote.add(POSITION_End);

	//params_Remote.add(bGo_START);
	//params_Remote.add(bGo_END);

	params_Remote.add(bENABLE_TimersGlobal);
	params_Remote.add(bMODE_SkipTime);
	params_Remote.add(divBeatSkipper);
	params_Remote.add(bMODE_SkipReverse);
	params_Remote.add(divBeatReverse);

	params_Remote.add(bMODE_SkipLooped);
	params_Remote.add(bMODE_SkipPowered);
	params_Remote.add(skipPower);

	params_Remote.add(bpmTimer);
	params_Remote.add(bpmDivider);
	params_Remote.add(bMODE_Edit);

	//TODO:
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	//params_Remote.add(presetsManager.getParametersSelectorToggles());
#endif

	remoteServer.setup(params_Remote);
#endif

	//--

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	mMidiParams.connect();
	mMidiParams.add(params_Remote);
	//mMidiParams.add(params_Preset);

	//TODO:
	//mMidiParams.add(presetsManager.getParametersSelectorToggles());
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_AppSettings()
{
	// App Settings

	// main app parameters to store recall on exit and start app
	//MODE_App.set("MODE_App", 0, 0, 1);

	//--

	// App Settings to handle store/recall only

	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bPlay);
	params_AppSettings.add(videoName);
	params_AppSettings.add(videoFilePath);
	params_AppSettings.add(bKeys);
	params_AppSettings.add(bAutoHide);

	//params_AppSettings.add(bGui);
	//params_AppSettings.add(bMODE_Loop);
	//params_AppSettings.add(MODE_App);

//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
//	params_AppSettings.add(bKeys_Presets);
//#endif
//
//#ifdef USE_ofxSurfingFxChannel
//	params_AppSettings.add(bKeys_Fx);
//#endif

	// Force preferred settings
	//surfingPreview.bGui = false;
	//bGui_VideoBarControl = true;

	//TODO: fix
	//// Handled by ImGui Manager!
	//params_AppSettings.add(bGui);
	//params_AppSettings.add(bGui_Presets);
	params_AppSettings.add(bGui_SkipTimers);
	//params_AppSettings.add(bGui_SurfingVideo);

	//params_AppSettings.add(guiManager.bAutoResize);
	//params_AppSettings.add(guiManager.bDebug);
	//params_AppSettings.add(guiManager.bExtra);

	//TODO: should be internaly handled..
	//params_AppSettings.add(guiManager.bGui);
	//params_AppSettings.add(guiManager.bGui_WindowsSpecials);
	//params_AppSettings.add(guiManager.bAutoSaveSettings);
	//params_AppSettings.add(guiManager.bMinimize);

	//params_AppSettings.add(bGui_VideoBarControl);
	//params_AppSettings.add(surfingPreview.bGui);
	//params_AppSettings.add(surfingPreview.bBigScreen);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup()
{
	bDISABLECALLBACKS = true;

	// Settings folder
	setPath_GlobalFolder("ofxSurfingVideoSkip");

	path_AppSettings = "ofxSurfingVideoSkip_App_Settings.xml";

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	path_Preset = "ofxSurfingVideoSkip_Engine_Settings.xml";
#ifdef USE_ofxSurfingFxChannel
	path_fileName_ChannelFX = "ofxSurfingVideoSkip_ChannelFX_Settings.xml";
#endif
#endif

	//-

	//window_W = ofGetWidth();
	//window_H = ofGetHeight();

	//--

	inScrub = false;

	bGui_VideoBarControl.set("Control Bar", false);//var used to hide/show player bar and gui also
	//bGui_VideoBarControl.addListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//--

	// skipper engine
	videoFilePath.set("videoFilePath", "NO FILE");
	videoName.set("FILE", "NO FILE");
	videoTIME.set("", ""); // current time position

	bGui_SkipTimers.set("SKIP TIMERS", true);
	//bGui_Presets.set("SHOW PRESETS", true);
	bAutoHide.set("AutoHide Bar", true);

	bKickL.set("FRAME L", false);
	bKickR.set("FRAME R", false);

	bPlay.set("PLAY", false);
	bMODE_Edit.set("EDIT", false);
	bMODE_Loop.set("LOOP", true);
	//bMODE_Loop.set("CLAMP", false);

	speedNorm.set("SPEED", 0.0f, -1.0f, 1.0f);
#define SPEED_MIN 0.20f
#define SPEED_MAX 50.0f
	speed.set("SPEED_", 1.0f, SPEED_MIN, SPEED_MAX);
	bReset_Speed.set("RESET SPEED", false);

	bMODE_LoopedBack.set("LOOPB", false);
	bMODE_Reversed.set("REVERSE", false);
	bTRIG_ResetEngine.set("RESET ENGINE", false);
	bTRIG_ResetAll.set("RESET", false);
	bTRIG_Reset_Bpm.set("RESET BPM", false);
	bGui_Advanced.set("SHOW ADVANCED", false);

	POSITION_Start.set("START", 0.0f, 0.0f, 1.0f);
	POSITION_End.set("END", 1.0f, 0.0f, 1.0f);
	POSITION.set("POSITION", 0.0f, 0.0f, 1.0f);
	bSET_START.set("SET START", false);
	bSET_END.set("SET END", false);
	bGo_START.set("GO START", false);
	bGo_END.set("GO END", false);
	bENABLE_TimersGlobal.set("ENABLE TIMERS", false);
	bTRIG_SkipTime.set("A SKIP", false);
	bTRIG_SkipReverse.set("B REV", false);
	bMODE_SkipTime.set("A MODE SKIP", false);
	bMODE_SkipReverse.set("B MODE REV", false);

	bMODE_SkipLooped.set("SK Loop", true);
	bMODE_SkipPowered.set("SK Pow", false);
	skipPower.set("SK Power", 0.5f, 0.01f, 1.0f);//how much near or far (0->1) from the current position jumps to

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
	beatDuration.set("DURATION", 4, 1, 8);
	beatRescale.set("RESCALE", 0, -8, 8);
	bMODE_Lock.set("LOCK", false);//?

	//-

	last_TRIG_time = 0;
	last_TRIG_reverse = 0;

	// exclude
	bMODE_Loop.setSerializable(false);
	bMODE_Edit.setSerializable(false);
	speed.setSerializable(false);
	bReset_Speed.setSerializable(false);
	bTRIG_ResetEngine.setSerializable(false);
	bTRIG_ResetAll.setSerializable(false);
	bTRIG_Reset_Bpm.setSerializable(false);
	timer_SkipTime.setSerializable(false);
	timer_SkipRev.setSerializable(false);
	bKickL.setSerializable(false);
	bKickR.setSerializable(false);

	// modes. not included into presets

#ifdef USE_ofxSurfingFxChannel
	channelFx.bGui.setName("FX CHANNEL");
#endif

	bGui_SurfingVideo.set("SKIP PLAYER", true);

	//--

	// video controls

	params_Engine.setName("SKIP PLAYER");
	params_Engine.add(bGui_SkipTimers);
	params_Engine.add(bPlay);
	params_Engine.add(bMODE_Edit);
	params_Engine.add(videoName);//NOTE: a longer string will resize the gui panel width!
	params_Engine.add(POSITION);
	params_Engine.add(videoTIME);

	//-

	// for presets settings

	params_Engine.add(speedNorm);
	params_Engine.add(speed);
	params_Engine.add(bReset_Speed);

	params_Engine.add(bMODE_SkipLooped);
	params_Engine.add(bMODE_SkipPowered);
	params_Engine.add(skipPower);

	//params_Engine.add(bMODE_Loop);
	params_Engine.add(bSET_START);
	params_Engine.add(bSET_END);
	params_Engine.add(bGo_START);
	params_Engine.add(bGo_END);
	params_Engine.add(POSITION_Start);
	params_Engine.add(POSITION_End);

	params_Engine.add(bMODE_LoopedBack);
	params_Engine.add(bMODE_Reversed);

	params_Engine.add(bKickL);
	params_Engine.add(bKickR);

	//----

	// Skip Engine

	_param_SkipEngine.add(bENABLE_TimersGlobal);

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
	_param_SkipEngine.add(bTRIG_ResetAll);

#ifdef USE_BPM_TIMER_MODE
	_param_Clock.add(bpmTimer);
	_param_Clock.add(bpmDivider);
#endif
	_param_Clock.add(bTRIG_Reset_Bpm);
	_param_SkipEngine.add(_param_Clock);

	params_Engine.add(_param_SkipEngine);

	//-

	ofAddListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	// these params are not stored. just from gui panels here

	//--

	// Presets Manager 

	setup_Preset();

	//--

	// Gui

	//----

	// Main gui panel params

	params_Control.setName("SURFING VIDEO SKIP");

	params_Control.add(bGui_SurfingVideo);
	params_Control.add(bGui_SkipTimers);
	params_Control.add(bGui);

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	params_Control.add(bGui_Presets);
	//#endif

		//-

	_param_Keys.add(bKeys);

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	_param_Keys.add(bKeys_Presets);
	//#endif
	//
	//#ifdef USE_ofxSurfingFxChannel
	//	_param_Keys.add(bKeys_Fx);
	//#endif

		//-

#ifdef USE_ofxSurfingFxChannel
	params_Control.add(_param_ChannelFx);
#endif

	//-

#ifdef USE_ofxSurfingMoods
	params_Control.add(_param_MoodMachine);
#endif
	params_Control.add(_param_Keys);

	//params_Control.add(bAutoHide);//?
	//params_Control.add(videoName);

	//--

	// Channel Fx

#ifdef USE_ofxSurfingFxChannel
	setup_ChannelFx();
#endif

	//--

#ifdef USE_ofxSurfingMoods
	surfingMoods.setup();
	surfingMoods.bGui.setName("MOODS");
	surfingMoods.setKeySpace(false);

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

	setup_AppSettings();

	//--

	// Callbacks

	// Gui
	listener_SHOW_gui = bGui.newListener([this](bool&)
		{
			this->Changed_bGui();
		});

	//// App Mode
	//listener_MODE_App = MODE_App.newListener([this](int &)
	//{
	//	this->Changed_MODE_App();
	//});

	//--

	// Presets Manager

#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	// for video mark/loops engine
	setup_PresetsManager();
#endif

	//--

	// Gui

	setup_ImGui();

	//--

	if (bKeys) addKeysListeners();

	setup_Remote();

	//----

	// Startup 
	startup(); // video file path will (must) be loaded now
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::startup()
{
	bDISABLECALLBACKS = false;

	//-

	setup_PresetsStuff();

	//-

	// Overwritte default settings

	if (!bMODE_SkipLooped && !bMODE_SkipPowered) bMODE_SkipPowered = true;

	//----

	// HAP video player

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
	bActive = b;

	setGuiVisible(b);
	setKeysEnable(b);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setKeysEnable(bool b)
{
	bKeys = b;
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.setEnableKeys(b);
#endif
}

#ifdef USE_ofxSurfingFxChannel
//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_ChannelFx()
{
	channelFx.setPath_GlobalFolder(path_GLOBAL_Folder + "/ofxChannelFx");//ofxSurfingVideoSkip/ofxChannelFx
	channelFx.setup();
	channelFx.setVisibleGui(true);
	channelFx.setKeysExtra(false);//disables space and arrows keys for browsing presets
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::update(ofEventArgs& args)
{
	//--

	// Draw

	// Fbo
	//fboPreview.begin();
	//ofClear(0, 255);
	//player.draw(0, 0, fboPreview.getWidth(), fboPreview.getHeight());
	//fboPreview.end();

	// Video unprocessed
	surfingPreview.begin();
	player.draw(0, 0);
	surfingPreview.end();


	//--

	// FxPro
	{
		fxPro.begin();
		{
			draw_Video();
		}
		fxPro.end(false);
	}

	//--

	// Player

	updateVideoPlayer();

	//--

	// Presets Manager

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

	// Simple callbacks 

	// 1. To detected that preset index changed
	if (presetsManager.isDoneLoad() || presetsManager.isRetrigged())
	{
		ofLogNotice(__FUNCTION__) << "Preset Loaded";

		// a new preset has been loaded.
		// set position to preset, using the position start variable
		if (player.isLoaded())
		{
			POSITION = POSITION_Start;
			player.setPosition(POSITION);
		}
	}
#endif

	//--

//	//TODO:
//	// sync max one time per frame. all the callbacks are "flagged to here".
//#ifdef USE_ofxRemoteParameters
//	if (bSyncRemote) {
//		bSyncRemote = false;
//		remoteServer.syncParameters();//hangs
//	}
//#endif

}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::calculateKick()
{
	// kick-drift
	// calculate desired step size
	// by left-right arrow keys

	// just try, bc if video is not loaded will need to do it again
	totalNumFrames = player.getTotalNumFrames();
	if (totalNumFrames == 0) {
		ofLogError(__FUNCTION__) << "totalNumFrames is 0. Could wait until video file is loaded...";
		return;
	}
	else
		ofLogNotice(__FUNCTION__) << "totalNumFrames: " << totalNumFrames;

	frameSizeNorm = 1.0f / (float)totalNumFrames;//decimals size of a frame
	ofLogNotice(__FUNCTION__) << "frameSizeNorm: " << frameSizeNorm;
	kickSizeFrame = frameSizeNorm * (float)numFramesToKick;
	ofLogNotice(__FUNCTION__) << "kickSizeFrame: " << kickSizeFrame;

	//float currDur = player.getDuration();//step proportional to video secs
	//currFrame = ofMap(POSITION, 0, 1, 0, totalNumFrames);//curr frame?
}

//TODO:
//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateConstraints()
{
	//if (!bMODE_Edit)
	{
		// mantain start must be before finish
		if (POSITION_Start == POSITION_End) {//set a minim loop duration
			POSITION_End += kickSizeFrame;
		}
		// if (POSITION_Start > POSITION_End) {//flip if inverted
		//	float temp = POSITION_End;
		//	POSITION_Start = POSITION_End;
		//	POSITION_End = temp;
		//}
		if (POSITION_Start > POSITION_End) {//force if inverted
			POSITION_Start = POSITION_End;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateVideoPlayer()
{
	POSITION.setWithoutEventNotifications(player.getPosition());
	//POSITION = player.getPosition();

	//--

	updateConstraints();

	////if (!bMODE_Edit)
	//{
	//	// mantain start must be before finish
	//	if (POSITION_Start == POSITION_End) {//set a minim loop duration
	//		POSITION_End += kickSizeFrame;
	//	}
	//	//else if (POSITION_Start > POSITION_End) {//flip if inverted
	//	//	float temp = POSITION_End;
	//	//	POSITION_Start = POSITION_End;
	//	//	POSITION_End = temp;
	//	//}
	//	else if (POSITION_Start > POSITION_End) {//force if inverted
	//		POSITION_Start = POSITION_End;
	//	}
	//}

	//--

	//TODO:
	// Do only once at startup!
	// if load video fails totalNumFrames will be 0, then we will retry every x seconds!
	if (totalNumFrames == 0 && ofGetFrameNum() % (60 * 5) == 0)
	{
		calculateKick();
	}

	//--

	// display current time in minute:seconds. max minutes is 99 to nice formatting
	positionSeconds = player.getDuration() * POSITION;

	// time label
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
	videoFRAME = ofToString(POSITION * totalNumFrames, 0);

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

	// not playing. player is stopped
	else
	{
		//if (!inScrub && !bENABLE_TimersGlobal && !player.isPaused())
		//{
			//// clamp cursor into loop range
			//if (bMODE_Loop && !bMODE_Edit)
			//{
			//	if (player.getPosition() >= POSITION_End)
			//	{
			//		POSITION = POSITION_End;
			//		player.setPosition(POSITION);
			//	}
			//	else if (player.getPosition() <= POSITION_Start)
			//	{
			//		POSITION = POSITION_Start;
			//		player.setPosition(POSITION);
			//	}
			//}
		//}
	}

	//--

	// auto hide gui system

	//if (ofGetElapsedTimeMillis() - lastMovement < time_autoHide)
	//{
	//	bGui_VideoControlBar_PRE = bGui_VideoBarControl;
	//	bGui_VideoBarControl = true;
	//}
	//else
	//{
	//	if (bAutoHide)
	//	{
	//		bGui_VideoBarControl = false;
	//		bGui_VideoControlBar_PRE = bGui_VideoBarControl;
	//	}
	//}

	//--

	//TODO
	// disable to avoid bug with clicks l/r on gui (ofxGuiExtended2)
	// hide mouse if changed
	if ((bGui_VideoBarControl != bGui_VideoControlBar_PRE) || bAutoHide)
	{
		ofRectangle window = ofGetWindowRect();
		if (!bGui_VideoBarControl && window.inside(ofGetMouseX(), ofGetMouseY()))
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
	//if(isPlaying && bPlay) 
	updateTimers();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateTimers()
{
	//--

	// Mode beat

	//TODO:
	// this could be wrong bc time and durations depends of play speed!
	// but we tryed to fix this using the speed as multiplier.

	if (bMODE_Beat)
	{
		int videoDur = (1.f / speed) * player.getDuration() * 1000;//video duration in ms
		int tDur;
		if (beatRescale == 0)
		{
			tDur = ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}
		else if (beatRescale > 0)//multiply
		{
			tDur = beatRescale * ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}
		else//divide
		{
			tDur = (1.0f / abs(beatRescale)) * ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}
		float tDurN = (float)tDur / (float)videoDur;//duration in normalized decimals 

		POSITION_End = POSITION_Start + tDurN;
	}

	//--

	// Timers

	// A Skip time

	if (bENABLE_TimersGlobal && bMODE_SkipTime && !inScrub && bPlay)
	{
		uint64_t t;
		int tmax;
		t = ofGetElapsedTimeMillis() - last_TRIG_time;

#ifndef USE_BPM_TIMER_MODE
		tmax = timePeriod_skipper;
#else
		tmax = divBeatSkipper.get() * ((60000 / bpmDivider.get()) / (bpmTimer.get()));
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

	// B Reverse

	if (bENABLE_TimersGlobal && bMODE_SkipReverse && !inScrub && bPlay)
	{
		uint64_t t;
		int tmax;
		t = ofGetElapsedTimeMillis() - last_TRIG_reverse;

#ifndef USE_BPM_TIMER_MODE
		tmax = timePeriod_reverser;
#else
		tmax = divBeatReverse.get() * ((60000 / bpmDivider.get()) / (bpmTimer.get()));
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
				skipPos = ofRandom(0.0, 1.0f);
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
	draw_VideoBarControl();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setGuiVisible(bool b)
{
	//bGui_SurfingVideo = b;//TODO: synced...

	//TODO:
	//ENABLE_GuiVisibleByAutoHide = b;
	if (b)
	{
		bGui_VideoControlBar_PRE = bGui_VideoBarControl;
		//bGui_VideoBarControl = true;
		lastMovement = ofGetElapsedTimeMillis();
	}

	// draw control bar
	//bGui_VideoBarControl = b;

	//-

//	// Presets Manager
//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
//	presetsManager.setVisible_PresetClicker(b);
//#endif

}

//--

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseMoved(ofMouseEventArgs& eventArgs)
{
	if (bActive)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& button = eventArgs.button;

		{
			lastMovement = ofGetElapsedTimeMillis();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseRefresh(int button, float position)
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
void ofxSurfingVideoSkip::mouseDragged(ofMouseEventArgs& eventArgs)
{
	if (bActive)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& button = eventArgs.button;

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

			mouseRefresh(button, _position);

			//--

			//// workflow
			////disable loop if cursor is out-of-loop
			//if (POSITION < POSITION_Start || POSITION > POSITION_End)
			//{
			//	if (bMODE_Loop && !bMODE_Edit) bMODE_Loop = false;
			//}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mousePressed(ofMouseEventArgs& eventArgs)
{
	if (bActive)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& button = eventArgs.button;

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

			mouseRefresh(button, _position);

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
void ofxSurfingVideoSkip::mouseReleased(ofMouseEventArgs& eventArgs)
{
	if (bActive)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& button = eventArgs.button;

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
	//TODO: resize fbo
	//window_W = _w;
	//window_H = _h;

	//player.set

	fxPro.windowResized(_w, _h);

}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
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

	//--

	fxPro.keyPressed(key);

	//----

	// ofxSurfingVideoSkip
	if (bKeys)
	{
		//----

		//TODO:
		//must add modifiers to avoid keys collapsing...
		//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);

		//if (bActive)
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

		//// Loop mode

		//else if (key == 'L' || key == 'l')
		//{
		//	bMODE_Loop = !bMODE_Loop;
		//}

		// Edit mode
		else if (key == 'E')
		{
			bMODE_Edit = !bMODE_Edit;

			//bMODE_Loop = !bMODE_Loop;
			////if (!bMODE_Loop && bENABLE_TimersGlobal)
			////{
			////	bENABLE_TimersGlobal = false;
			////}
		}

		//-

		// Go start/end

		//if (!bMODE_Edit)
		{
			// recall (go)

			if (key == 'I' && !mod_CONTROL)
			{
				bGo_START = true;
				//POSITION = POSITION_Start;
			}
			else if (key == 'O' && !mod_CONTROL)
			{
				bGo_END = true;
				//POSITION = POSITION_End;
			}

			// store (set)
			else if (key == 'I' && mod_CONTROL)
			{
				bSET_START = true;
			}
			else if (key == 'O' && mod_CONTROL)
			{
				bSET_END = true;
			}
		}

		//-

		//	//// user kick-drift frame-by-frame
		//	//else if (key == OF_KEY_LEFT && bMODE_Edit && !mod_CONTROL)
		//	//{
		//	//	bKickL = true;
		//	//	//POSITION -= kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_RIGHT && bMODE_Edit && !mod_CONTROL)
		//	//{
		//	//	bKickR = true;
		//	//	//POSITION += kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_LEFT && bMODE_Edit && mod_CONTROL)
		//	//{
		//	//	POSITION -= 10 * kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_RIGHT && bMODE_Edit && mod_CONTROL)
		//	//{
		//	//	POSITION += 10 * kickSizeFrame;
		//	//}
		//}

		// reset preset. basic settings only
		if (key == 'R' && !mod_CONTROL)
		{
			bTRIG_ResetEngine = true;
		}

		// reset all
		if (key == 'R' && mod_CONTROL)
		{
			bTRIG_ResetAll = true;
		}

		//-

		// Sound

		else if (key == OF_KEY_UP)
		{
			player.setVolume(player.getVolume() + 0.1);
		}
		else if (key == OF_KEY_DOWN)
		{
			player.setVolume(player.getVolume() - 0.1);
		}

		//-

		// Show/hide gui
		else if (key == 'G')
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

		//-
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
void ofxSurfingVideoSkip::Changed_DONE_save(bool& DONE_save)
{
	ofLogNotice(__FUNCTION__) << ofToString(DONE_save ? "TRUE" : "FALSE");
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_DONE_load(bool& DONE_load)
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
void ofxSurfingVideoSkip::Changed_Params(ofAbstractParameter& e) // patch change
{
	if (!bDISABLECALLBACKS)
	{
		std::string name = e.getName();

		if (name != "" &&
			name != bTRIG_SkipTime.getName() &&
			name != bTRIG_SkipReverse.getName() &&
			name != POSITION.getName() &&
			name != POSITION_Start.getName() &&
			name != POSITION_End.getName() &&
			name != bMODE_Reversed.getName() &&
			name != timer_SkipTime.getName() &&
			name != timer_SkipRev.getName()
			)
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (0) {}

		//--

		// Position

		else if (name == POSITION.getName())
		{
			player.setPosition(POSITION);

			// workflow

			//if (!bPlay && bMODE_Edit)
			if (bMODE_Edit)
			{
				POSITION_Start = POSITION;
			}

			//if (!bPlay && !bMODE_Edit) {
			//	bMODE_Edit = true;
			//}
		}

		else if (name == POSITION_Start.getName())
		{
			// constraint
			if (POSITION_Start > POSITION_End) {
				POSITION_End = POSITION_Start;
			}

			// workflow
			//if (!bPlay /*&& bMODE_Edit*/)
			if (bMODE_Edit)
			{
				POSITION = POSITION_Start;
			}
			if (!bPlay && !bMODE_Edit) {
				//bMODE_Edit = true;
			}
		}

		else if (name == POSITION_End.getName())
		{
			// constraint
			if (POSITION_Start > POSITION_End) {
				POSITION_Start = POSITION_End;
			}

			// workflow
			//if (!bPlay /*&& bMODE_Edit*/)
			//if (bMODE_Edit)//breaks
			if (!bPlay && bMODE_Edit)
			{
				bDISABLECALLBACKS = true;
				POSITION = POSITION_End;
				player.setPosition(POSITION);
				bDISABLECALLBACKS = false;
			}
			if (!bPlay && !bMODE_Edit) {
				//bMODE_Edit = true;
			}
		}

		//--

		else if (name == bPlay.getName())
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

				// workflow

				if (bMODE_Edit) bMODE_Edit = false;
				//if (!bMODE_Loop) bMODE_Loop = true;
			}
		}

		// edit
		else if (name == bMODE_Edit.getName())
		{
			if (bMODE_Edit)
			{
				// workflow
				if (bPlay) bPlay = false;
				//if (!bMODE_Loop) bMODE_Loop = true;

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

		//// loop
		//else if (name == bMODE_Loop.getName())
		//{
		//	//if (bMODE_Loop)
		//	//{
		//	//	player.setPosition(POSITION_Start);
		//	//}
		//}

		//--

		// go
		else if (name == bGo_START.getName() && bGo_START)
		{
			bGo_START = false;
			POSITION = POSITION_Start;
		}
		else if (name == bGo_END.getName() && bGo_END)
		{
			bGo_END = false;
			POSITION = POSITION_End;
		}

		// set
		else if (name == bSET_START.getName() && bSET_START)
		{
			bSET_START = false;
			POSITION_Start = player.getPosition();

			//if (bMODE_Edit)
			//{
			//	// workflow
			//	//enable loop
			//	if (!bMODE_Loop) bMODE_Loop = true;

			//	// workflow
			//	// if start/end flipped: set finish one second to the right
			//	if (POSITION_End < POSITION_Start)
			//	{
			//		float gap = frameSizeNorm * 60;//1sec at 60fps
			//		POSITION_End = POSITION_Start + gap;
			//	}
			//}
		}
		else if (name == bSET_END.getName() && bSET_END)
		{
			bSET_END = false;
			POSITION_End = player.getPosition();

			//// workflow
			////if star/end flipped: set finish one second to the right
			//if (POSITION_End < POSITION_Start)
			//{
			//	float gap = frameSizeNorm * 60;//1sec at 60fps
			//	POSITION_Start = POSITION_End - gap;
			//}
		}

		//-

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
			if (speedNorm >= 0 && speedNorm <= 1)
			{
				speed = ofMap(speedNorm, 0.f, 1.f, min, max, true);
			}
			else
			{
				min = speed.getMin();
				max = 1.0f;

				if (speedNorm >= -1 && speedNorm < 0) {
					speed = ofMap(speedNorm, -1.f, 0.f, min, max, true);
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

		else if (name == bTRIG_ResetAll.getName() && bTRIG_ResetAll)
		{
			bTRIG_ResetAll = false;

			bTRIG_ResetEngine = true;

			bMODE_Beat = true;
			beatRescale = 0;
			beatDuration = 2;
		}

		else if (name == bTRIG_ResetEngine.getName() && bTRIG_ResetEngine)
		{
			bTRIG_ResetEngine = false;

			bENABLE_TimersGlobal = false;
			bMODE_SkipTime = false;
			bMODE_SkipReverse = false;

			bMODE_LoopedBack = true;
			bMODE_Reversed = false;

			bReset_Speed = true;

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
			//	if (!bENABLE_TimersGlobal) bENABLE_TimersGlobal = true;
			//}
		}
		else if (name == bMODE_SkipReverse.getName())
		{
			//// workflow
			//if (bMODE_SkipReverse.get()) {
			//	if (!bENABLE_TimersGlobal) bENABLE_TimersGlobal = true;
			//}
		}

		else if (name == bGui_SurfingVideo.getName())
		{
			//bool b = bGui_SurfingVideo.get();
			//bGui_VideoBarControl = b;
		}

		//#ifdef USE_ofxSurfingMoods
		//		else if (name == SHOW_MoodMachine.getName())
		//		{
		//			surfingMoods.setGui_Visible(SHOW_MoodMachine);
		//		}
		//#endif

		//-

		// Presets Manager

//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
//		else if (name == "SHOW PRESETS")
//		{
//			presetsManager.setVisible_PresetClicker(bGui_Presets);
//		}
//#endif
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

		else if (name == bKeys.getName())
		{
			if (bKeys) addKeysListeners();
			else removeKeysListeners();

			//crashes
				//bDISABLECALLBACKS = true;
				//if (bKeys.get())
				//{
				//	bKeys_Presets = false;
				//	bKeys_Fx = false;
				//}
				//bDISABLECALLBACKS = false;
		}

		//		else if (name == bKeys_Presets.getName())
		//		{
		//			bDISABLECALLBACKS = true;
		//			if (bKeys_Presets.get())
		//			{
		//				bKeys = false;
		//				bKeys_Fx = false;
		//			}
		//			bDISABLECALLBACKS = false;
		//
		//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
		//			presetsManager.setEnableKeys(bKeys_Presets);
		//#endif
		//		}

		//else if (name == bKeys_Fx.getName())
		//{
		//	bDISABLECALLBACKS = true;
		//	if (bKeys_Fx.get())
		//	{
		//		bKeys_Presets = false;
		//		bKeys = false;
		//	}
		//	bDISABLECALLBACKS = false;
		//}

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

		//--

		//TODO:
#ifdef USE_ofxRemoteParameters
		bSyncRemote = true;
		//remoteServer.syncParameters();//hangs
#endif
}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_bGui()
{
	ofLogNotice(__FUNCTION__) << ofToString(bGui.get());

	// hide all
	if (!bGui)
	{
		setGuiVisible(false);
	}

	// show all
	else
	{
		setGuiVisible(true);

		//bGui_VideoControlBar_PRE = bGui_VideoBarControl;
		//bGui_VideoBarControl = true;
		//lastMovement = ofGetElapsedTimeMillis();
	}
}

////--------------------------------------------------------------
//void ofxSurfingVideoSkip::Changed_MODE_App()
//{
//	//ofLogNotice(__FUNCTION__) << "Changed_MODE_App:" << ofToString(MODE_App.get());
//
//	//lastMovement = ofGetElapsedTimeMillis();
//
//	//if (MODE_App == 1)
//	//{
//	//	//hide all player
//	//	setGuiVisible(false);
//	//}
//	//else if (MODE_App == 0)
//	//{
//	//	//show all player
//	//	if (bGui)
//	//	{
//	//		setGuiVisible(true);
//	//	}
//	//}
//}
//
////--------------------------------------------------------------
//void ofxSurfingVideoSkip::Changed_draw_Autohide(bool &b)
//{
//	//	if (bAutoHide)
//	//	{
//	//		if (bGui_VideoBarControl != bGui_VideoControlBar_PRE)
//	//		{
//	//			ofLogNotice(__FUNCTION__) << "Changed_draw_Autohide: " << ofToString(bGui_VideoBarControl ? "TRUE" : "FALSE");
//	//
//	//			if (MODE_App == 0)
//	//			{
//	//				if (!bGui && bGui_VideoBarControl)
//	//				{
//	//					bGui_VideoControlBar_PRE = bGui_VideoBarControl;
//	//					bGui_VideoBarControl = false;
//	//				}
//	//				setGuiVisible(bGui_VideoBarControl);
//	//
//	//#ifdef USE_ofxSurfingMoods
//	//				//surfingMoods.setGui_visible(bGui_VideoBarControl&&SHOW_MoodMachine);
//	//#endif
//	//			}
//	//			else if (MODE_App == 1)
//	//			{
//	//				if (!bGui && bGui_VideoBarControl)
//	//				{
//	//					bGui_VideoControlBar_PRE = bGui_VideoBarControl;
//	//					bGui_VideoBarControl = false;
//	//				}
//	//			}
//	//		}
//	//
//	//		//hide/show cursor
//	//		if (!bGui_VideoBarControl)
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
void ofxSurfingVideoSkip::Changed_Mood_RANGE(int& targetVal)
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
void ofxSurfingVideoSkip::Changed_Mood_TARGET(int& targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}
//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_A(int& targetVal)
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
void ofxSurfingVideoSkip::Changed_Mood_PRESET_B(int& targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}

//-------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_C(int& targetVal)
{
	ofLogNotice(__FUNCTION__) << targetVal;
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw(ofEventArgs& args)
{
	ofClear(surfingPreview.colorBg);
	//ofEnableDepthTest();

	//--

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	presetsManager.draw();
	//#endif

#ifdef USE_ofxSurfingFxChannel
	//if (ENABLE_Video_FX)
	if (channelFx.bENABLE_Fx)
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

	// FxPro
	fxPro.draw();

	//--

	if (bGui)
	{
		draw_Gui();

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
		mMidiParams.draw();
#endif
}

	//--

	surfingPreview.draw();
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
void ofxSurfingVideoSkip::draw_VideoBarControl()
{
	if (player.isLoaded())
	{
		int alphaBar = 128;
		float roundB = 5.0f;

		// Draw the position bar if appropriate
		if (bGui_VideoBarControl)
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

				// 3.2 Red line marks to start/end loop
				if (bMODE_Edit)
				{
					ofNoFill();
					ofSetLineWidth(2.0);
					ofSetColor(ofColor::red);
					ofDrawLine(pStart, yy + padding, pStart, yy + BarHeight - 1);
					ofDrawLine(pStart + pWidth, yy + padding, pStart + pWidth, yy + BarHeight - 1);

					float a = getFadeBlink(0.40, 0.70, 0.3);
					//float a = ofxSurfingHelpers::getFadeBlink(0.40, 0.70, 0.3);
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
void ofxSurfingVideoSkip::loadGroup(ofParameterGroup& g, std::string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::saveGroup(ofParameterGroup& g, string path)
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
#ifdef USE_ofxSurfingFxChannel

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
	//bGui_VideoBarControl.removeListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//-

	// Presets Manager
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
	//guiManager.setup();
	guiManager.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);

	// Customize default labels
	//guiManager.setLabelLayoutPanels("SURFING_VIDEO"); // global title

	//-

	// -> 1. Layouts Presets 
	// passing toggles

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

#ifdef USE_ofxSurfingFxChannel
	guiManager.addWindowSpecial(channelFx.bGui);
#endif

	guiManager.addWindowSpecial(fxPro.bGui);
	
	//-

	// -> 2. Extra Params to include into Layout Presets

	//// Preview
	////guiManager.addExtraParamToLayoutPresets(surfingPreview.getParameters());//group
	//guiManager.addExtraParamToLayoutPresets(surfingPreview.bGui);
	//guiManager.addExtraParamToLayoutPresets(surfingPreview.bBigScreen);
	////guiManager.addExtraParamToLayoutPresets(surfingPreview.bFullScreen);
	//guiManager.addExtraParamToLayoutPresets(bGui_VideoBarControl);

	//// Other
	//guiManager.addExtraParamToLayoutPresets(guiManager.bMinimize);
	//guiManager.addExtraParamToLayoutPresets(bGui_SkipTimers);

	////guiManager.addExtraParamToLayoutPresets(presetsManager.playerSurfer.bGui);

	//-

	// -> Initiates when adding proccess finished!
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

		//guiManager.beginWindow(n.c_str(), (bool*)&bGui_SkipTimers.get(), flagsw);
		//if (guiManager.beginWindow(n.c_str(), (bool*)&bGui_SkipTimers.get(), flagsw))

		if (guiManager.beginWindow(n.c_str(), bGui_SkipTimers))
		{
			// manual triggers
			guiManager.Add(bTRIG_SkipTime, OFX_IM_BUTTON_BIG, 2, true);
			guiManager.Add(bTRIG_SkipReverse, OFX_IM_BUTTON_BIG, 2, false);

			ofxImGuiSurfing::AddSpacingSeparated();
			//ImGui::Spacing();

			//--

			// Big enablers
			guiManager.Add(bENABLE_TimersGlobal, OFX_IM_TOGGLE_BIG_XXL_BORDER);

			ImGui::Spacing();
			//ImGui::Spacing();

			//--

			//if (bENABLE_TimersGlobal && (bMODE_SkipReverse || bMODE_SkipTime)) 
			if (bENABLE_TimersGlobal)
			{
				bool bOpen;
				ImGuiTreeNodeFlags _flagt;

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
					if (bENABLE_TimersGlobal)
					{
						bOpen = true;
						_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
						_flagt |= ImGuiTreeNodeFlags_Framed;
						if (ImGui::TreeNodeEx("A SKIP TIME", _flagt))
						{
							refreshLayout();

							//guiManager.Add(bTRIG_SkipTime, OFX_IM_BUTTON_BIG, 1, false);

							if (bMODE_SkipTime && bENABLE_TimersGlobal)
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
					if (bENABLE_TimersGlobal)
					{
						if (ImGui::TreeNodeEx("B SKIP REV", _flagt))
						{
							refreshLayout();

							//guiManager.Add(bTRIG_SkipReverse, OFX_IM_BUTTON_BIG, 1, false);

							if (bMODE_SkipReverse && bENABLE_TimersGlobal)
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

				//if (!bENABLE_TimersGlobal)
				//	ImGui::Spacing();

				guiManager.Add(bTRIG_ResetEngine, OFX_IM_BUTTON_SMALL);
			}

			//--

			ImGui::Spacing();
			ofxImGuiSurfing::AddToggleRoundedButton(_bAutoResize);

			guiManager.endWindow();
		}
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

	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

	//--

	// Engine

	// Skip player window
	{
		n = bGui_SurfingVideo.getName();

		//guiManager.beginWindow(n.c_str(), (bool*)&bGui_SurfingVideo.get(), flagsw);
		//if (guiManager.beginWindow(n.c_str(), (bool*)&bGui_SurfingVideo.get(), flagsw))

		if (guiManager.beginWindow(n.c_str(), bGui_SurfingVideo))
		{
			// minimize
			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
			ImGui::Spacing();

			//if (!guiManager.bMinimize)
			{
				//ImGui::Spacing();

				// Skip Panel
				guiManager.Add(bGui_SkipTimers, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				
				guiManager.Add(fxPro.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);

				//ImGui::Spacing();
			}

			guiManager.AddSpacingBigSeparated();
			//ImGui::Spacing();
			//ImGui::Spacing();

			//-

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
					guiManager.AddSpacing();
					ImGui::Text(videoFilePath.get().data());
					ImGui::TreePop();
				}

				guiManager.AddSpacingBigSeparated();
			}

			//--

			{
				ImGui::Spacing();

				//--

				// Play

				//guiManager.Add(bPlay, OFX_IM_TOGGLE_BIG_XXXL_BORDER_BLINK);
				////guiManager.Add(bPlay, OFX_IM_TOGGLE_BIG);
				{
					float ___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
					float ___w2 = ofxImGuiSurfing::getWidgetsWidth(2);
					float ___h = ofxImGuiSurfing::getWidgetsHeightUnit();
					ofxImGuiSurfing::AddBigToggleNamed(bPlay, ___w1, 3.0f * ___h, "PLAYING", "PLAY", true);
				}

				//-

				ImGui::Spacing();

				guiManager.Add(bMODE_LoopedBack, OFX_IM_TOGGLE_SMALL, 2, true);
				guiManager.Add(bMODE_Reversed, OFX_IM_TOGGLE_SMALL, 2, false);
				guiManager.AddSpacingBigSeparated();

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
				bool bReturn;

				{
					float __h = getWidgetsHeightRelative(); // relative to theme
					float _ww = ImGui::GetContentRegionAvail().x; // full window panel width

					if (bMODE_Loop)
						bReturn = (ofxImGuiSurfing::AddHSliderRanged2(p, ImVec2(_ww, __h), POSITION_Start, POSITION_End, true, true));
					else
						bReturn = (ofxImGuiSurfing::AddHSlider(p, ImVec2(_ww, __h), true, true));
				}

				if (bReturn)
				{
					POSITION = p;
				}
				IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(p);

				//ImGui::Spacing();

				//-

				//// Loop
				//guiManager.Add(bMODE_Loop, OFX_IM_TOGGLE_BIG_BORDER, 1, false);
				//ImGui::Spacing();

				guiManager.AddSpacingBigSeparated();

				//-

				// Range
				if (bMODE_Loop)
				{
					// Start/Finish
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Text("START END");
					guiManager.Add(POSITION_Start, OFX_IM_HSLIDER_MINI_NO_LABELS);

					if (!bMODE_Beat)
					{
						guiManager.Add(POSITION_End, OFX_IM_HSLIDER_MINI_NO_LABELS);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						guiManager.Add(POSITION_End, OFX_IM_HSLIDER_MINI_NO_LABELS);
						ImGui::PopItemFlag();
					}

					//if (!bMODE_Beat) ImGui::Text("START END");
					//else ImGui::Text("START");
					//guiManager.Add(POSITION_Start, OFX_IM_HSLIDER_MINI_NO_LABELS);
					//if (!bMODE_Beat) guiManager.Add(POSITION_End, OFX_IM_HSLIDER_MINI_NO_LABELS);
					//else {
					//	AddSpacingBig();
					//	AddSpacingBig();
					//}

					//ImGui::Spacing();
					guiManager.AddSpacingBigSeparated();
				}

				//-


				// Beat mode
				if (!guiManager.bMinimize)
					if (bMODE_Loop)
					{
						//AddSpacingSeparated();
						ImGui::Spacing();

						guiManager.Add(bMODE_Beat, OFX_IM_TOGGLE_BORDER);
						//guiManager.Add(bMODE_Beat, OFX_IM_TOGGLE_SMALL_BORDER, 2, true);
						//guiManager.Add(bMODE_Lock, OFX_IM_TOGGLE_SMALL_BORDER, 2, false);
						if (bMODE_Beat)
						{
							guiManager.Add(beatDuration, OFX_IM_SLIDER);
							guiManager.Add(beatRescale, OFX_IM_STEPPER);
							//ofxImGuiSurfing::AddParameter(beatDuration);//breaks layout
							//ofxImGuiSurfing::AddParameter(beatRescale);
						}

						//ImGui::Spacing();
						guiManager.AddSpacingBigSeparated();
					}

				//--

				// Preset Clicker

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
		
				guiManager.AddLabelBig("PRESETS");
				presetsManager.draw_ImGui_ClickerMinimal();
				guiManager.AddSpacingBigSeparated();

#endif
				//--

				if (!guiManager.bMinimize)
				{
					refreshLayout();

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
						//ImGui::Spacing();
						//guiManager.AddSpacingBigSeparated();

						// finetune
						static bool bFineTune = false;
						ToggleRoundedButton("FINETUNE", &bFineTune, ImVec2(30, 20));
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
										if (ImGui::Button("-", ImVec2(_w50, _h * 0.7)))//fix
										{
											bKickL = true;
										}
										ImGui::SameLine();
										if (ImGui::Button("+", ImVec2(_w50, _h * 0.7)))
										{
											bKickR = true;
										}
									}
									ImGui::PopButtonRepeat();
								}
								guiManager.AddSpacing();

								guiManager.Add(POSITION, OFX_IM_STEPPER);

								if (bMODE_Loop) {
									guiManager.Add(POSITION_Start, OFX_IM_STEPPER);

									if (!bMODE_Beat) guiManager.Add(POSITION_End, OFX_IM_STEPPER);
								}

								// Mark clip start/end
								//if (bMODE_Edit)
								if (bMODE_Loop)
								{
									ImGui::Spacing();

									if (bMODE_Beat)
									{
										guiManager.Add(bSET_START, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bGo_START, OFX_IM_BUTTON_SMALL, 2, false);
									}
									else
									{
										guiManager.Add(bSET_START, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bSET_END, OFX_IM_BUTTON_SMALL, 2, false);
										guiManager.Add(bGo_START, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bGo_END, OFX_IM_BUTTON_SMALL, 2, false);
									}

									ImGui::Spacing();
								}
							}
							ImGui::Unindent();
							refreshLayout();
						}
						//ImGui::Unindent();
					}
				}

				//-

				if (bMODE_Loop)
					if (!guiManager.bMinimize)
					{
						AddSpacingSeparated();
						ImGui::Spacing();

						//guiManager.Add(bMODE_Edit, OFX_IM_TOGGLE_BIG_BORDER_BLINK);
						guiManager.Add(bMODE_Edit, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);

						ImGui::Spacing();

						guiManager.Add(bTRIG_ResetAll, OFX_IM_BUTTON_SMALL);

						guiManager.AddSpacingBigSeparated();

					}

				//-
			}//loop

			//--

			// minimize

			//ImGui::Spacing();

			if (!guiManager.bMinimize)
			{
				static ofParameter<bool> bScreens{ "SCREENS", false };
				//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
				//guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.Add(bScreens, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

				//if (!guiManager.bMinimize)
				if (bScreens)
				{
					ImGui::Indent();
					refreshLayout();

					// Preview
					guiManager.Add(surfingPreview.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					guiManager.Add(surfingPreview.bBigScreen, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					if (surfingPreview.bBigScreen)
					{
						//guiManager.Add(surfingPreview.bInDocked, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						// Control Bar
						guiManager.Add(bGui_VideoBarControl, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					}

					ImGui::Unindent();
					refreshLayout();
				}

				guiManager.AddSpacingBigSeparated();
			}

			//-

			if (!guiManager.bMinimize)
			{
				ImGui::Spacing();
				refreshLayout();

				// extra
				guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
				if (guiManager.bExtra)
				{
					ImGui::Indent();
					{
						refreshLayout();

						ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
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
							mMidiParams.add(presetsManager.getParametersSelectorToggles());
						}
#endif
#endif
						//--

						// Extra panel
						guiManager.drawAdvanced(true, true);
					}
					ImGui::Unindent();
					refreshLayout();
				}
			}

			guiManager.endWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui()
{
	if (!bGui) return;

	//-

	guiManager.begin();
	{
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

		//----

		if (bGui_SurfingVideo) draw_ImGuiControls();

		if (bGui_SurfingVideo) draw_ImGuiPreview();

		if (bGui_SkipTimers && bGui_SurfingVideo) draw_ImGuiSkipTimers();

		//if (bGui_SkipTimers) draw_ImGuiSkipTimers();

		//--
	}
	guiManager.end();

	//--

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	presetsManager.draw();
#endif

#ifdef USE_ofxSurfingMoods
	surfingMoods.draw_ImGui();
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.draw();
#endif

#ifdef USE_ofxSurfingFxChannel
	channelFx.drawGui();
#endif

	// FxPro
	fxPro.drawGui();

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