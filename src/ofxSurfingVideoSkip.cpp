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

	// Name will be use to prefix file names at /group folder
	params_Preset.setName("CLIPS");

	params_Preset.add(position_In);
	params_Preset.add(position_Out);

	//params_Preset.add(bDoResetSpeed);
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

	//// Exclude
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
	// External connections
	// - clock
	// - osc  midi
	// 
	//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP

	beatClock.setup();
	beatClock.bGui.setName("CLOCK");

	// Callback to receive BeatTicks:
	listenerBeat = beatClock.BeatTick.newListener([&](bool&) {this->Changed_BeatTick(); });

	// Callback to receive BPM changes:
	//listenerBpm = beatClock.BPM_Global.newListener([&](float&) {this->Changed_BeatBpm(); });

#endif

	//--

	// External Control
	// to link to OSC/midi control

#ifdef USE_ofxRemoteParameters

	params_Remote.setName("VideoSkip REMOTE");

	params_Remote.add(bPlay);

	params_Remote.add(videoName);
	params_Remote.add(position);
	params_Remote.add(videoTIME);

	params_Remote.add(speedNorm);
	params_Remote.add(bDoResetSpeed);

	params_Remote.add(bMODE_LoopedBack);
	params_Remote.add(bMODE_Reversed);

	params_Remote.add(position);
	params_Remote.add(position_In);
	params_Remote.add(position_Out);

	//params_Remote.add(bGo_In);
	//params_Remote.add(bGo_Out);

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
	// to handle store/recall only, not for callbacks

	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bGui_SkipTimers);
	params_AppSettings.add(bPlay);
	params_AppSettings.add(videoName);
	params_AppSettings.add(videoFilePath);
	params_AppSettings.add(bKeys);
	params_AppSettings.add(bGui_VideoBarControl);
	params_AppSettings.add(bAutoHideVideoBar);
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

	//--

	bGui.set("SURFING VIDEO", true);
	bGui_SurfingVideo.set("VIDEO SKIP", true);
	bGui_SkipTimers.set("SKIP TIMERS", false);
	bGui_Previews.set("PREVIEWS", false);

	//--

	inScrub = false;

	bGui_VideoBarControl.set("Control Bar", false); // var used to hide/show player bar and gui also
	//bGui_VideoBarControl.addListener(this, &ofxSurfingVideoSkip::Changed_draw_Autohide);

	//--

	// Skipper Engine
	videoFilePath.set("videoFilePath", "NO FILE");
	videoName.set("FILE", "NO FILE");
	videoTIME.set("", ""); // current time position

	bAutoHideVideoBar.set("AutoHide Bar", true);

	bDoKickL.set("FRAME L", false);
	bDoKickR.set("FRAME R", false);

	bPlay.set("PLAY", false);
	bMODE_Edit.set("EDIT", false);
	bMODE_Loop.set("LOOP", true);

	speedNorm.set("SPEED", 0.0f, -1.0f, 1.0f);
	speed.set("SPEED_", 1.0f, SPEED_MIN, SPEED_MAX);
	bDoResetSpeed.set("RESET SPEED", false);

	bMODE_LoopedBack.set("LOOP BACK", false);
	bMODE_Reversed.set("REVERSE", false);
	bDoResetEngine.set("RESET ENGINE", false);
	bDoResetAll.set("RESET", false);
	bDoResetBpm.set("RESET BPM", false);

	//bGui_Advanced.set("SHOW ADVANCED", false);

	position_In.set("IN", 0.0f, 0.0f, 1.0f);
	position_Out.set("OUT", 1.0f, 0.0f, 1.0f);
	position.set("POS", 0.0f, 0.0f, 1.0f);

	bSet_In.set("SET IN", false);
	bSet_Out.set("SET OUT", false);
	bGo_In.set("GO IN", false);
	bGo_Out.set("GO OUT", false);

	bENABLE_TimersGlobal.set("ENABLE TIMERS", false);
	bDoSkipTime.set("A SKIP", false);
	bDoSkipReverse.set("B REV", false);
	bMODE_SkipTime.set("A MODE SKIP", false);
	bMODE_SkipReverse.set("B MODE REV", false);

	bMODE_SkipLooped.set("SK Loop", true);
	bMODE_SkipPowered.set("SK Pow", false);
	// How much near or far (0->1) from the current position jumps to.
	skipPower.set("SK Power", 0.5f, 0.01f, 1.0f);

#ifndef USE_BPM_TIMER_MODE
	timePeriod_skipper.set("SKIP-TIME TIME", 1000, 1, 4000);
	timePeriod_reverser.set("SKIP-REVERSE TIME", 1000, 1, 4000);
#else

	// These are global.
	// will not be included into presets!
	bpmTimer.set("BPM", 120.0f, 40.f, 400.0f);
	bpmDivider.set("DIV", 2, 1, 8);

	timer_SkipTime.set("_timer1_", 0, 0, 1);
	timer_SkipRev.set("_timer2_", 0, 0, 1);

	divBeatSkipper.set("T SKIP", 4, 1, 8);
	divBeatReverse.set("T REV", 2, 1, 8);
#endif

	// Beat
	bMODE_Beat.set("MODE BEAT", false);
	beatDuration.set("DURATION", 4, 1, 8);
	beatRescale.set("SCALE", 0, -8, 8);
	bMODE_Lock.set("LOCK", false);//?

	indexPreviewSource.set("Source Preview", 0, 0, previewSources.size() - 1);

	//-

	last_TRIG_time = 0;
	last_TRIG_reverse = 0;

	// Exclude
	bMODE_Loop.setSerializable(false);
	bMODE_Edit.setSerializable(false);
	speed.setSerializable(false);
	timer_SkipTime.setSerializable(false);
	timer_SkipRev.setSerializable(false);
	bDoResetSpeed.setSerializable(false);
	bDoResetEngine.setSerializable(false);
	bDoResetAll.setSerializable(false);
	bDoResetBpm.setSerializable(false);
	bDoKickL.setSerializable(false);
	bDoKickR.setSerializable(false);

	// Modes. Not included into presets

#ifdef USE_ofxSurfingFxChannel
	channelFx.bGui.setName("FX CHANNEL");
#endif

	//--

	// Video controls

	params_Engine.setName("SKIP PLAYER");
	params_Engine.add(bGui_SkipTimers);
	params_Engine.add(bPlay);
	params_Engine.add(bMODE_Edit);
	params_Engine.add(videoName); // NOTE: a longer string will resize the GUI panel width!
	params_Engine.add(position);
	params_Engine.add(videoTIME);

	//--

	// For presets settings

	params_Engine.add(speedNorm);
	params_Engine.add(speed);
	params_Engine.add(bDoResetSpeed);

	params_Engine.add(bMODE_SkipLooped);
	params_Engine.add(bMODE_SkipPowered);
	params_Engine.add(skipPower);

	params_Engine.add(bSet_In);
	params_Engine.add(bSet_Out);
	params_Engine.add(bGo_In);
	params_Engine.add(bGo_Out);
	params_Engine.add(position_In);
	params_Engine.add(position_Out);

	params_Engine.add(bMODE_LoopedBack);
	params_Engine.add(bMODE_Reversed);

	params_Engine.add(bDoKickL);
	params_Engine.add(bDoKickR);

	//----

	// Skip Engine

	_param_SkipEngine.add(bENABLE_TimersGlobal);

	//--

	// Skippers

	// Time

	_param_SkipEngine.add(bMODE_SkipTime);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_skipper);
#else
	_param_SkipEngine.add(divBeatSkipper);
#endif
	_param_SkipEngine.add(timer_SkipTime);
	_param_SkipEngine.add(bDoSkipTime);

	_param_SkipEngine.add(bMODE_SkipLooped);
	_param_SkipEngine.add(bMODE_SkipPowered);
	_param_SkipEngine.add(skipPower);

	//-

	// Reverse

	_param_SkipEngine.add(bMODE_SkipReverse);
#ifndef USE_BPM_TIMER_MODE
	_param_SkipEngine.add(timePeriod_reverser);
#else
	_param_SkipEngine.add(divBeatReverse);
#endif
	_param_SkipEngine.add(timer_SkipRev);
	_param_SkipEngine.add(bDoSkipReverse);

	_param_SkipEngine.add(bDoResetEngine);
	_param_SkipEngine.add(bDoResetAll);

#ifdef USE_BPM_TIMER_MODE
	_param_Clock.add(bpmTimer);
	_param_Clock.add(bpmDivider);
#endif
	_param_Clock.add(bDoResetBpm);
	_param_SkipEngine.add(_param_Clock);

	params_Engine.add(_param_SkipEngine);

	//-

	ofAddListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	// these params are not stored. just from GUI panels here

	//--

	// Presets Manager 

	setup_Preset();

	//--

	// Gui

	//----

	// Main GUI panel params

	params_Control.setName("SURFING VIDEO SKIP");

	params_Control.add(bGui_SurfingVideo);
	params_Control.add(bGui_SkipTimers);
	params_Control.add(bGui);

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	params_Control.add(bGui_Presets);
	//#endif

	_param_Keys.add(bKeys);

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	_param_Keys.add(bKeys_Presets);
	//#endif
	//
	//#ifdef USE_ofxSurfingFxChannel
	//	_param_Keys.add(bKeys_Fx);
	//#endif

	//--

#ifdef USE_ofxSurfingFxChannel
	params_Control.add(_param_ChannelFx);
#endif

	//--

#ifdef USE_ofxSurfingMoods
	params_Control.add(_param_MoodMachine);
#endif
	params_Control.add(_param_Keys);

	//params_Control.add(bAutoHideVideoBar);//?
	//params_Control.add(videoName);

	//--

	// Channel Fx

#ifdef USE_ofxSurfingFxChannel
	setup_ChannelFx();
#endif

	//--

#ifdef USE_ofxSurfingMoods

	// Link Mood BPM to BeatBlock instance:
	moods.bpmSpeed.makeReferenceTo(beatClock.BPM_Global);
	bpmTimer.makeReferenceTo(beatClock.BPM_Global);

	// Link Mood PLAY to BeatBlock instance:
	moods.bPLAY.makeReferenceTo(beatClock.bPlay);

	moods.setEnableExternalClock(true); // Forced. Is not mandatory, can be modified using the GUI.

	moods.setup();

	//moods.bGui_Main.setName("MOODS");

	moods.setKeySpace(false);

	//--

	// Customize
	//moods.setGui_Visible(SHOW_MoodMachine);
	//moods.setGui_AdvancedVertical_MODE(true);

	// Listeners for inside class moods
	moods.RANGE_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	moods.TARGET_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	moods.PRESET_A_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	moods.PRESET_B_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	moods.PRESET_C_Selected.addListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);

	//moods.setTickMode(true);
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
	// video file path will (must) be loaded now
	startup();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::startup()
{
	bDISABLECALLBACKS = false;

	//-

	setup_PresetsStuff();

	//-

	// Overwrite default settings

	if (!bMODE_SkipLooped && !bMODE_SkipPowered) bMODE_SkipPowered = true;

	//----

	// HAP video player

	// A. Hard coded video file
	//videoFilePath="/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov";//default
	//videoFilePath="\movies\SampleHap.mov";
	//videoName="NightmoVES4";

	//// B. Compose the path name from the name stored in the xml
	//// BUG: videoName do not loads because the string param is used too in the other group param above..
	//// is not stored, like if goes out of the param group
	//auto myStrings = ofSplitString(videoFilePath, "/");//not working bc '\'
	//videoName = myStrings[myStrings.size() - 1];

	// split string path using boost
	// https://stackoverflow.com/questions/10099206/howto-parse-a-path-to-a-vector-using-c
	boost::filesystem::path p1(videoFilePath.get());
	//boost::filesystem::path p1("/usr/local/bin");
	////boost::filesystem::path p2("c:\\");
	//std::cout << p1.filename() << std::endl; // prints "bin"
	//std::cout << p1.parent_path() << std::endl; // prints "/usr/local"
	videoName = ofToString(p1.filename());

	//--

	// A. Load the path of the movie from xml settings
	loadMovie(videoFilePath);

	// B. Hard coded path file
	//loadMovie("movies/NightmoVES4.mov");
	//loadMovie("/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov");

	player.setLoopState(OF_LOOP_NORMAL);
	player.setVolume(0.0f);

	// Workflow
	// Skip black intro
	//player.setPosition(0.05);

	//--

	position = position_In;
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

	// Feed FxPro
	{
		fxPro.begin();
		{
			draw_Video();
		}
		fxPro.end(false);
	}

	//--

	// Feed Preview 

	surfingPreview.begin();
	{
		// unprocessed video
		if (indexPreviewSource == 0) player.draw(0, 0);

		// processed video 
		else if (indexPreviewSource == 1) fxPro.draw();
	}
	surfingPreview.end();

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

		// A new preset has been loaded.
		// Set position to preset, using the position start variable
		if (player.isLoaded())
		{
			position = position_In;
			player.setPosition(position);
		}
	}

#endif

}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::calculateKick()
{
	// Kick-drift
	// Calculate desired step size
	// by left-right arrow keys

	// Just try, bc if video is not loaded will need to do it again
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
	//currFrame = ofMap(position, 0, 1, 0, totalNumFrames);//curr frame?
}

//TODO:
//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateConstraints()
{
	//if (!bMODE_Edit)
	{
		// maintain start must be before finish

		// set a minim loop duration
		if (position_In == position_Out) {
			position_Out += kickSizeFrame;
		}

		// flip if inverted
		// if (position_In > position_Out) {
		//	float temp = position_Out;
		//	position_In = position_Out;
		//	position_Out = temp;
		//}

		// force if inverted
		if (position_In > position_Out) {
			position_In = position_Out;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateVideoPlayer()
{
	position.setWithoutEventNotifications(player.getPosition());
	//position = player.getPosition();

	//--

	updateConstraints();

	////if (!bMODE_Edit)
	//{
	//	// mantain start must be before finish
	//	if (position_In == position_Out) {//set a minim loop duration
	//		position_Out += kickSizeFrame;
	//	}
	//	//else if (position_In > position_Out) {//flip if inverted
	//	//	float temp = position_Out;
	//	//	position_In = position_Out;
	//	//	position_Out = temp;
	//	//}
	//	else if (position_In > position_Out) {//force if inverted
	//		position_In = position_Out;
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

	// Display current time in minute:seconds. 
	// max minutes is 99 to nice formatting
	positionSeconds = player.getDuration() * position;

	// Time label
	int currMin, currSec;
	std::string strMin, strSec;
	currMin = positionSeconds / 60;
	currSec = ((int)(positionSeconds)) % 60;
	strMin = (currMin < 10) ? "0" : "";
	strSec = (currSec < 10) ? "0" : "";
	strMin += ofToString(currMin);
	strSec += ofToString(currSec);
	videoTIME = /*"Time " + */strMin + ":" + strSec;//std::string

	// Frame
	videoFRAME = ofToString(position * totalNumFrames, 0);

	//--

	// Check if player is running
	bool isPlaying = player.isPlaying() && !player.isPaused();

	//TODO:
	//BUG: last/first frame get flicked..

	// Playing. 
	// player running
	if (isPlaying)
	{
		//TODO: 
		// Loop Engine
		// workflow: added !bMODE_Edit to allow playing out of range loop
		// but requires to imrpove workflow when playing preset with EDIT MODE enabled

		if (bMODE_Loop)
		{
			if (player.getPosition() >= position_Out)
			{
				if (!bMODE_LoopedBack)
				{
					if (bMODE_Reversed) position = position_Out;
					else position = position_In;
				}
				else
				{
					position = position_Out;
					bMODE_Reversed = !bMODE_Reversed;
				}

				player.setPosition(position);
			}

			else if (player.getPosition() <= position_In) // ? < only
			{
				if (!bMODE_LoopedBack)
				{
					if (bMODE_Reversed) position = position_Out;
					else position = position_In;
				}
				else
				{
					position = position_In;
					bMODE_Reversed = !bMODE_Reversed;
				}

				player.setPosition(position);
			}
		}
	}

	// Not playing. player is stopped
	else
	{
		//if (!inScrub && !bENABLE_TimersGlobal && !player.isPaused())
		//{
			//// clamp cursor into loop range
			//if (bMODE_Loop && !bMODE_Edit)
			//{
			//	if (player.getPosition() >= position_Out)
			//	{
			//		position = position_Out;
			//		player.setPosition(position);
			//	}
			//	else if (player.getPosition() <= position_In)
			//	{
			//		position = position_In;
			//		player.setPosition(position);
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
	//	if (bAutoHideVideoBar)
	//	{
	//		bGui_VideoBarControl = false;
	//		bGui_VideoControlBar_PRE = bGui_VideoBarControl;
	//	}
	//}

	//--

	//TODO
	// Disable to avoid bug with clicks l/r on gui (ofxGuiExtended2)
	// Hide mouse if changed
	if ((bGui_VideoBarControl != bGui_VideoControlBar_PRE) || bAutoHideVideoBar)
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

	// Skipper Engine:
	//if(isPlaying && bPlay) 
	updateTimers();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateTimers()
{
	//--

	// Mode beat

	//TODO:
	// This could be wrong bc time and durations depends of play speed!
	// But we tried to fix this using the speed as multiplier.

	if (bMODE_Beat)
	{
		int videoDur = (1.f / speed) * player.getDuration() * 1000;//video duration in ms
		int tDur;
		if (beatRescale == 0)
		{
			tDur = ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}
		else if (beatRescale > 0) // multiply
		{
			tDur = beatRescale * ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}
		else // divide // < 0
		{
			tDur = (1.0f / abs(beatRescale)) * ((int)((60000.f / (float)bpmTimer)) * beatDuration);//duration in ms. rescaled
		}

		float tDurN = (float)tDur / (float)videoDur; // duration in normalized decimals 

		position_Out = position_In + tDurN;
	}

	//----

	// Timers

	// A Skip Time

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
			bDoSkipTime = true;

			//// workflow
			//if (bMODE_SkipReverse)
			//{
			//	last_TRIG_reverse = ofGetElapsedTimeMillis();
			//	bDoSkipReverse = true;
			//}
		}

		timer_SkipTime = MAX(0, t / (float)tmax);
	}

	//--

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
			bDoSkipReverse = true;
		}

		timer_SkipRev = MAX(0, t / (float)tmax);
	}

	//--

	// A - B Triggers

	// Time Trigger

	if (bDoSkipTime == true)
	{
		bDoSkipTime = false;

		float skipPos;

		if (bMODE_SkipLooped)
		{
			if (bMODE_Loop)
			{
				skipPos = ofRandom(position_In, position_Out);
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
			skipPos = position + pow;

			// clamp
			if (bMODE_Loop)
			{
				skipPos = ofClamp(skipPos, position_In, position_Out);
			}
			else
			{
				skipPos = ofClamp(skipPos, 0, 1);
			}
		}

		// Apply
		player.setPosition(skipPos);
	}

	//--

	// Reverse Trigger

	if (bDoSkipReverse == true)
	{
		bDoSkipReverse = false;

		//if (bMODE_SkipReverse)
		{
			bMODE_Reversed = !bMODE_Reversed;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Gui()
{
	if (!bGui) return;

	draw_ImGui();

	// bar controller 
	draw_VideoBarControl();

	//--

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	mMidiParams.draw();
#endif

	//--

	//TODO:remove
	surfingPreview.draw_Rectangle();
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
			// workflow A
			position_In = position;
			if (position_Out < position_In) position_Out = position_In;

			// workflow B
			//if (position < position_Out) position_In = position;
			//else if (position > position_Out) 
			//{
			//	float POSITION_Start_PRE = position_In;
			//	float POSITION_End_PRE = position_Out;
			//	position_Out = position;
			//	position_In = POSITION_Start_PRE;
			//}
		}
		else if (button == 2)
		{
			position_Out = position;

			if (position_Out < position_In) position_In = position_Out;

		}
	}
	else
	{
		position = position;
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
			//if (position < position_In || position > position_Out)
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
			// auto stop
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
	fxPro.windowResized(_w, _h);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	// Modifiers
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

		// Transport

		// Play / Stop

		if (key == ' ')
		{
			setPlay(!bPlay);
		}

		else if (key == OF_KEY_RETURN)
		{
#ifdef USE_ofxSurfingMoods
			bool b = !moods.isPlaying();
			setPlay_MoodMachine(b);
			setPlay(b);
#endif
		}

		//--

		// Edit mode
		else if (key == 'E')
		{
			bMODE_Edit = !bMODE_Edit;
		}

		//--

		// Go In / Out

		//if (!bMODE_Edit)
		{
			// Recall (go)

			if (key == 'I' && !mod_CONTROL)
			{
				bGo_In = true;
				//position = position_In;
			}
			else if (key == 'O' && !mod_CONTROL)
			{
				bGo_Out = true;
				//position = position_Out;
			}

			// Store (set)
			else if (key == 'I' && mod_CONTROL)
			{
				bSet_In = true;
			}
			else if (key == 'O' && mod_CONTROL)
			{
				bSet_Out = true;
			}
		}

		//-

		//	//// user kick-drift frame-by-frame
		//	//else if (key == OF_KEY_LEFT && bMODE_Edit && !mod_CONTROL)
		//	//{
		//	//	bDoKickL = true;
		//	//	//position -= kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_RIGHT && bMODE_Edit && !mod_CONTROL)
		//	//{
		//	//	bDoKickR = true;
		//	//	//position += kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_LEFT && bMODE_Edit && mod_CONTROL)
		//	//{
		//	//	position -= 10 * kickSizeFrame;
		//	//}
		//	//else if (key == OF_KEY_RIGHT && bMODE_Edit && mod_CONTROL)
		//	//{
		//	//	position += 10 * kickSizeFrame;
		//	//}
		//}

		// Reset Preset. Basic settings only
		if (key == 'R' && !mod_CONTROL)
		{
			bDoResetEngine = true;
		}

		// Reset All
		if (key == 'R' && mod_CONTROL)
		{
			bDoResetAll = true;
		}

		//--

		// Sound

		else if (key == OF_KEY_UP)
		{
			player.setVolume(player.getVolume() + 0.1);
		}
		else if (key == OF_KEY_DOWN)
		{
			player.setVolume(player.getVolume() - 0.1);
		}

		//--

		// Show / hide GUI

		else if (key == 'G')
		{
			bGui = !bGui; // independent to auto hide state

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
		position = position_In;
		player.setPosition(position);

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
			name != bDoSkipTime.getName() &&
			name != bDoSkipReverse.getName() &&
			name != position.getName() &&
			name != position_In.getName() &&
			name != position_Out.getName() &&
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

		else if (name == position.getName())
		{
			player.setPosition(position);

			// workflow

			//if (!bPlay && bMODE_Edit)
			if (bMODE_Edit)
			{
				position_In = position;
			}

			//if (!bPlay && !bMODE_Edit) {
			//	bMODE_Edit = true;
			//}
		}

		else if (name == position_In.getName())
		{
			// constraint
			if (position_In > position_Out)
			{
				position_Out = position_In;
			}

			// workflow
			//if (!bPlay /*&& bMODE_Edit*/)
			if (bMODE_Edit)
			{
				position = position_In;
			}
			if (!bPlay && !bMODE_Edit)
			{
				//bMODE_Edit = true;
			}
		}

		else if (name == position_Out.getName())
		{
			// constraint
			if (position_In > position_Out) {
				position_In = position_Out;
			}

			// workflow
			//if (!bPlay /*&& bMODE_Edit*/)
			//if (bMODE_Edit)//breaks
			if (!bPlay && bMODE_Edit)
			{
				bDISABLECALLBACKS = true;
				position = position_Out;
				player.setPosition(position);
				bDISABLECALLBACKS = false;
			}
			if (!bPlay && !bMODE_Edit)
			{
				//bMODE_Edit = true;
			}
		}

		//--

		// play

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

		//--

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
		//	//	player.setPosition(position_In);
		//	//}
		//}

		//--

		// go

		else if (name == bGo_In.getName() && bGo_In)
		{
			bGo_In = false;
			position = position_In;
		}
		else if (name == bGo_Out.getName() && bGo_Out)
		{
			bGo_Out = false;
			position = position_Out;
		}

		//--

		// set

		else if (name == bSet_In.getName() && bSet_In)
		{
			bSet_In = false;
			position_In = player.getPosition();

			//if (bMODE_Edit)
			//{
			//	// workflow
			//	//enable loop
			//	if (!bMODE_Loop) bMODE_Loop = true;

			//	// workflow
			//	// if start/end flipped: set finish one second to the right
			//	if (position_Out < position_In)
			//	{
			//		float gap = frameSizeNorm * 60;//1sec at 60fps
			//		position_Out = position_In + gap;
			//	}
			//}
		}
		else if (name == bSet_Out.getName() && bSet_Out)
		{
			bSet_Out = false;
			position_Out = player.getPosition();

			//// workflow
			////if star/end flipped: set finish one second to the right
			//if (position_Out < position_In)
			//{
			//	float gap = frameSizeNorm * 60;//1sec at 60fps
			//	position_In = position_Out - gap;
			//}
		}

		//--

		// kick frame

		else if (name == bDoKickL.getName() && bDoKickL)
		{
			bDoKickL = false;
			position -= kickSizeFrame;
		}
		else if (name == bDoKickR.getName() && bDoKickR)
		{
			bDoKickR = false;
			position += kickSizeFrame;
		}

		//--

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

		else if (name == bDoResetSpeed.getName() && bDoResetSpeed)
		{
			bDoResetSpeed = false;
			speedNorm = 0.0f;
			//speed = 1.0f;
		}

		//-

		else if (name == bDoResetAll.getName() && bDoResetAll)
		{
			bDoResetAll = false;

			bDoResetEngine = true;

			bMODE_Beat = true;
			beatRescale = 0;
			beatDuration = 2;
		}

		else if (name == bDoResetEngine.getName() && bDoResetEngine)
		{
			bDoResetEngine = false;

			bENABLE_TimersGlobal = false;
			bMODE_SkipTime = false;
			bMODE_SkipReverse = false;

			bMODE_LoopedBack = false;
			bMODE_Reversed = false;

			bDoResetSpeed = true;

			divBeatSkipper = 4;
			divBeatReverse = 2;

			bMODE_SkipLooped = true;
			skipPower = 0.5f;

		}
		else if (name == bDoResetBpm.getName())
		{
			bDoResetBpm = false;

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
		//			moods.setGui_Visible(SHOW_MoodMachine);
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
// 
		//--

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

		//--

		// Keys

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

		//// Advanced
		//else if (name == bGui_Advanced.getName())
		//{
		//}

		//--

		// Skip mode
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
	}
}

#ifdef USE_ofxSurfingMoods

// moods
// listeners for inside moods

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
void ofxSurfingVideoSkip::Changed_Mood_PRESET_B(int& i)
{
	ofLogNotice(__FUNCTION__) << i;

	fxPro.presetsManager.load(i);
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
	//ofClear(surfingPreview.colorBg);

	//ofEnableDepthTest();

	//--

	//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	//	presetsManager.draw();
	//#endif

	//--

	/*
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
	*/

	//--

	// FxPro

	// We can choose the behavior.. 
	if (surfingPreview.bGui_PreviewBig) fxPro.draw();

	//----

	// Gui

	if (bGui)
	{
		draw_Gui();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_Video()
{
	if (!player.isLoaded()) return;

	// We can choose the behavior.. 
	//if (surfingPreview.bGui_PreviewBig)
	{
		ofPushStyle();
		{
			// Get player rect
			ofRectangle r(0, 0, player.getWidth(), player.getHeight());

			//--

			// Re scale player rectangle to surfingPreview view port

			// Full Screen mode

			if (surfingPreview.bFullScreen)
			{
				r.scaleTo(ofGetWindowRect(), surfingPreview.scaleMode); // Full view
			}
			else
			{
				// A. Draggable view port

				if (surfingPreview.bInDocked)
				{
					r.scaleTo(guiManager.getRectangleCentralDocking(), surfingPreview.scaleMode);
					surfingPreview.updateRectDraggable(r);
				}

				//--

				// B. Inner central docking view rectangle

				else
				{
					r.scaleTo(surfingPreview.rectDraggable, surfingPreview.scaleMode);
				}
			}

			//--

			// Draw video frame
			{
				ofSetColor(255, 255, 255, 255);
				player.draw(r.x, r.y, r.width, r.height);
			}
		}
		ofPopStyle();
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
				pStart = BarInset + barFull.width * position_In;
				pWidth = (BarInset + barFull.width * position_Out) - pStart;
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
void ofxSurfingVideoSkip::exit()
{
	// Save app settings
	saveGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

	// Channel fx
#ifdef USE_ofxSurfingFxChannel

//#ifndef USE_ofxPresetsManager__VIDEO_SKIP
//	saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_ChannelFX);
//#endif

	// ofxChannelFx
	channelFx.exit();
#endif

	//-

	// Callbacks
	ofRemoveListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);

	// disable for the moment
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
	// Listener for inside class moods
	moods.RANGE_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	moods.TARGET_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	moods.PRESET_A_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	moods.PRESET_B_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	moods.PRESET_C_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setPath_GlobalFolder(std::string folder)
{
	ofLogNotice(__FUNCTION__) << folder;
	path_GLOBAL_Folder = folder;
	ofxSurfingHelpers::CheckFolder(folder);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_ImGui()
{
	//guiManager.setup();
	guiManager.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);

	// Customize default labels
	//guiManager.setLabelLayoutPanels("SURFING_VIDEO"); // global title

	//--

	// -> 1. Layouts Presets 
	// passing toggles

	// This bool toggles will control the show of the added window
	// and will be added too to layout presets engine

	guiManager.addWindowSpecial(bGui_SurfingVideo);

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	guiManager.addWindowSpecial(presetsManager.bGui);
#endif

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	guiManager.addWindowSpecial(mMidiParams.bGui);
#endif

#ifdef USE_ofxSurfingMoods 
	guiManager.addWindowSpecial(moods.bGui_Main);
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	guiManager.addWindowSpecial(beatClock.bGui);
#endif

#ifdef USE_ofxSurfingFxChannel
	guiManager.addWindowSpecial(channelFx.bGui);
#endif

	guiManager.addWindowSpecial(fxPro.bGui);

	//--

	// -> 2. Extra Params to include into Layout Presets

	//// Preview
	////guiManager.addExtraParamToLayoutPresets(surfingPreview.getParameters());//group
	//guiManager.addExtraParamToLayoutPresets(surfingPreview.bGui);
	//guiManager.addExtraParamToLayoutPresets(surfingPreview.bGui_PreviewBig);
	////guiManager.addExtraParamToLayoutPresets(surfingPreview.bFullScreen);
	//guiManager.addExtraParamToLayoutPresets(bGui_VideoBarControl);

	//// Other
	//guiManager.addExtraParamToLayoutPresets(guiManager.bMinimize);
	//guiManager.addExtraParamToLayoutPresets(bGui_SkipTimers);

	////guiManager.addExtraParamToLayoutPresets(presetsManager.playerSurfer.bGui);

	//--

	// -> Initiates when adding proccess finished!
	guiManager.startup();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiSkipTimers()
{
	if (bGui_SkipTimers)
	{
		if (bGui_SurfingVideo) guiManager.setNextWindowAfterWindowNamed(bGui_SurfingVideo.getName());

		if (guiManager.beginWindow(bGui_SkipTimers))
		{
			float ___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
			float ___w2 = ofxImGuiSurfing::getWidgetsWidth(2);
			float ___h = ofxImGuiSurfing::getWidgetsHeightUnit();

			// Manual triggers
			guiManager.Add(bDoSkipTime, OFX_IM_BUTTON_BIG, 2, true);
			guiManager.Add(bDoSkipReverse, OFX_IM_BUTTON_BIG, 2, false);

			guiManager.AddSpacingSeparated();

			//--

			// Big enablers
			guiManager.Add(bENABLE_TimersGlobal, OFX_IM_TOGGLE_BIG_XXL_BORDER);

			guiManager.AddSpacing();

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
					guiManager.refreshLayout();
					___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
					___w2 = ofxImGuiSurfing::getWidgetsWidth(2);

					guiManager.Add(bpmTimer, OFX_IM_DEFAULT);
					guiManager.Add(bpmDivider, OFX_IM_DEFAULT);

					if (ImGui::Button("HALF", ImVec2(___w2, ___h)))
					{
						bpmTimer /= 2.0f;
					}

					guiManager.SameLine();

					if (ImGui::Button("DOUBLE", ImVec2(___w2, ___h)))
					{
						bpmTimer *= 2.0f;
					}

					// Reset
					guiManager.Add(bDoResetBpm, OFX_IM_BUTTON_SMALL);

					ImGui::TreePop();
				}

				guiManager.AddSpacingSeparated();

				//--

				guiManager.Add(bMODE_SkipTime, OFX_IM_TOGGLE_BIG_XXL_BORDER);

				if (bMODE_SkipTime)
					if (bENABLE_TimersGlobal)
					{
						bOpen = true;
						_flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
						_flagt |= ImGuiTreeNodeFlags_Framed;

						if (ImGui::TreeNodeEx("A SKIP TIME", _flagt))
						{
							guiManager.refreshLayout();

							if (bMODE_SkipTime && bENABLE_TimersGlobal)
							{
								guiManager.Add(divBeatSkipper, OFX_IM_DEFAULT);

								// Draw progress bar
								guiManager.Add(timer_SkipTime, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							guiManager.Add(bMODE_SkipLooped, OFX_IM_TOGGLE_SMALL, 2, true);
							guiManager.Add(bMODE_SkipPowered, OFX_IM_TOGGLE_SMALL, 2, false);

							if (bMODE_SkipPowered)
							{
								guiManager.Add(skipPower);
							}

							ImGui::TreePop();
						}

						guiManager.refreshLayout();
					}

				//--

				guiManager.AddSpacingSeparated();

				guiManager.Add(bMODE_SkipReverse, OFX_IM_TOGGLE_BIG_XXL_BORDER);

				if (bMODE_SkipReverse)
					if (bENABLE_TimersGlobal)
					{
						if (ImGui::TreeNodeEx("B SKIP REV", _flagt))
						{
							guiManager.refreshLayout();

							if (bMODE_SkipReverse && bENABLE_TimersGlobal)
							{
								guiManager.Add(divBeatReverse, OFX_IM_DEFAULT);

								// draw progress bar
								guiManager.Add(timer_SkipRev, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							ImGui::TreePop();
						}
						guiManager.refreshLayout();
					}

				guiManager.AddSpacingSeparated();

				guiManager.Add(bDoResetEngine, OFX_IM_BUTTON_SMALL);
			}

			guiManager.endWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiPreview()
{
	// Append the source name to the window header label.
	string n = "";
	if (indexPreviewSource < previewSources.size()) n = previewSources[indexPreviewSource];

	surfingPreview.draw_ImGuiPreview(guiManager.bMinimize, n);

	// if minimized will draw only the floating preview, not the extra window
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGuiControls()
{
	//--

	// Engine

	// Skip player window

	if (bGui_SurfingVideo)
	{
		IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

		if (guiManager.beginWindow(bGui_SurfingVideo))
		{
			float ___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
			float ___w2 = ofxImGuiSurfing::getWidgetsWidth(2);
			float ___h = ofxImGuiSurfing::getWidgetsHeightUnit();

			// Minimize
			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);

			guiManager.AddSpacingSeparated();

			//--

			// Video Title / File

			if (!guiManager.bMinimize)
			{
				// File
				bool bOpen = false;
				ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagt |= ImGuiTreeNodeFlags_Framed;
				std::string n = videoName;

				if (ImGui::TreeNodeEx(n.c_str(), _flagt))
				{
					___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
					___w2 = ofxImGuiSurfing::getWidgetsWidth(2);

					if (ImGui::Button("OPEN FILE", ImVec2(___w1, ___h)))
					{
						doOpenDialogToSetPath();
					}
					guiManager.AddSpacing();
					ImGui::TextWrapped(videoFilePath.get().data());

					ImGui::TreePop();
				}

				___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
				___w2 = ofxImGuiSurfing::getWidgetsWidth(2);

				guiManager.AddSpacingSeparated();
			}

			//--

			// Preview Screens

			guiManager.Add(bGui_Previews, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
			guiManager.AddSpacing();

			// minimized
			if (guiManager.bMinimize)
				if (bGui_Previews)
				{
					guiManager.Indent();

					// Preview Big
					guiManager.Add(surfingPreview.bGui_PreviewBig, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Preview Float
					guiManager.Add(surfingPreview.bGui_PreviewFloat, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Source Selector
					if (surfingPreview.bGui_PreviewFloat)
						guiManager.AddCombo(indexPreviewSource, previewSources);

					guiManager.Unindent();
				}

			// expanded
			if (!guiManager.bMinimize)
			{
				if (bGui_Previews)
				{
					guiManager.Indent();

					// Source Selector
					guiManager.AddCombo(indexPreviewSource, previewSources);

					// Preview Big
					guiManager.Add(surfingPreview.bGui_PreviewBig, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					if (surfingPreview.bGui_PreviewBig)
					{
						guiManager.Indent();

						// Control Bar
						guiManager.Add(bGui_VideoBarControl, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						if (bGui_VideoBarControl) guiManager.Add(bAutoHideVideoBar, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

						guiManager.Unindent();
					}

					// Preview Float
					guiManager.Add(surfingPreview.bGui_PreviewFloat, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					guiManager.Add(surfingPreview.bGui_Extra, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					guiManager.Unindent();
				}

				//guiManager.AddSpacingSeparated();
			}

			//--

			// Skip Panel

			guiManager.AddSpacingSeparated();

			guiManager.Add(bGui_SkipTimers, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

			guiManager.AddSpacingSeparated();

			//--

			{
				// Play

				___w1 = ofxImGuiSurfing::getWidgetsWidth(1);
				___w2 = ofxImGuiSurfing::getWidgetsWidth(2);

				ofxImGuiSurfing::AddBigToggleNamed(bPlay, ___w1, 3.0f * ___h, "PLAYING", "PLAY", true);

				guiManager.AddSpacingSeparated();

				//--

				// Preset Clicker

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

				//guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_ROUNDED);

				if (!presetsManager.bGui)
				{
					//guiManager.AddLabelBig("PRESETS", false, true);

					bool bHeader = true;
					bool bMinimal = guiManager.bMinimize;
					bool bShowMinimize = true;
					bool bNoExtras = false;

					presetsManager.draw_ImGui_ClickerSimple(bHeader, bMinimal, bShowMinimize, bNoExtras);

					guiManager.AddSpacingSeparated();
				}
#endif
				//--

				// Time 

				string s = ("Time " + videoTIME.get()).data();
				guiManager.AddLabelBig(s, false, true);
				guiManager.AddSpacing();

				//--

				// Position Bigger

				ofParameter<float> p = position.cast<float>();
				auto tmpRef = p.get();
				std::string name = p.getName();
				bool bReturn = false;

				if (bMODE_Loop)
				{
					bReturn = (ofxImGuiSurfing::AddHSliderRanged2(p, ImVec2(___w1, ___h), position_In, position_Out, true, true));
				}
				else
				{
					bReturn = (ofxImGuiSurfing::AddHSlider(p, ImVec2(___w1, ___h), true, true));
				}
				IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(p);

				if (bReturn)
				{
					position = p;
				}

				//--

				guiManager.AddSpacingSeparated();

				//--

				// Range

				if (bMODE_Loop)
				{
					// In / Out 
					guiManager.AddLabelBig("In | Out", false, true);
					guiManager.Add(position_In, OFX_IM_HSLIDER_MINI_NO_LABELS);

					if (!bMODE_Beat)
					{
						guiManager.Add(position_Out, OFX_IM_HSLIDER_MINI_NO_LABELS);
					}
					else
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						guiManager.Add(position_Out, OFX_IM_HSLIDER_MINI_NO_LABELS);
						ImGui::PopItemFlag();
					}

					guiManager.AddSpacingSeparated();
				}

				//--

				// Beat mode

				if (bMODE_Loop)
				{
					guiManager.Add(bMODE_Beat, OFX_IM_TOGGLE_BORDER);

					guiManager.AddSpacing();

					if (bMODE_Beat)
					{
						guiManager.Add(beatDuration, OFX_IM_SLIDER);
						guiManager.Add(beatRescale, OFX_IM_STEPPER);
					}

					guiManager.AddSpacingSeparated();
				}

				//--

				// Loop / Reverse

				guiManager.Add(bMODE_LoopedBack, OFX_IM_TOGGLE_SMALL, 2, true);
				guiManager.Add(bMODE_Reversed, OFX_IM_TOGGLE_SMALL, 2, false);
				guiManager.AddSpacingSeparated();

				//--

				// Speed

				guiManager.Add(speedNorm, OFX_IM_SLIDER);
				guiManager.Add(bDoResetSpeed, OFX_IM_BUTTON_SMALL);
				guiManager.AddSpacingSeparated();

				//--

				{
					guiManager.refreshLayout();

					// FineTune
					{
						static ofParameter<bool> bFineTune{ "Fine Tune", false };
						guiManager.Add(bFineTune, OFX_IM_TOGGLE_ROUNDED);

						guiManager.AddSpacing();

						if (bFineTune)
						{
							guiManager.Indent();
							{
								___w1 = guiManager.getWidgetsWidth(1);
								___w2 = guiManager.getWidgetsWidth(2);

								ImGui::Text(("Frame " + videoFRAME.get()).data());

								guiManager.AddSpacing();
								{
									ImGui::PushButtonRepeat(true);
									{
										if (ImGui::Button("-", ImVec2(___w2, 1.5f * ___h)))
										{
											bDoKickL = true;
										}

										ImGui::SameLine();

										if (ImGui::Button("+", ImVec2(___w2, 1.5f * ___h)))
										{
											bDoKickR = true;
										}
									}
									ImGui::PopButtonRepeat();
								}
								guiManager.AddSpacing();

								guiManager.Add(position, OFX_IM_STEPPER);

								if (bMODE_Loop)
								{
									guiManager.Add(position_In, OFX_IM_STEPPER);

									if (!bMODE_Beat) guiManager.Add(position_Out, OFX_IM_STEPPER);
								}

								//--

								// Mark clip In / Out

								//if (bMODE_Edit)

								if (bMODE_Loop)
								{
									guiManager.AddSpacing();

									if (bMODE_Beat)
									{
										guiManager.Add(bSet_In, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bGo_In, OFX_IM_BUTTON_SMALL, 2, false);
									}
									else
									{
										guiManager.Add(bSet_In, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bSet_Out, OFX_IM_BUTTON_SMALL, 2, false);
										guiManager.Add(bGo_In, OFX_IM_BUTTON_SMALL, 2, true);
										guiManager.Add(bGo_Out, OFX_IM_BUTTON_SMALL, 2, false);
									}
								}
							}
							guiManager.Unindent();
						}
					}
				}
			}

			//--

			if (bMODE_Loop)
			{
				guiManager.AddSpacingSeparated();

				if (!guiManager.bMinimize)
				{
					guiManager.Add(bMODE_Edit, OFX_IM_TOGGLE_BIG_BORDER_BLINK);
					guiManager.AddSpacing();
				}

				guiManager.Add(bDoResetAll, OFX_IM_BUTTON_SMALL);
			}

			//--

			/*
			if (!guiManager.bMinimize)
			{
						guiManager.AddSpacingSeparated();
				guiManager.refreshLayout();

				// Extra

				guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED);
				if (guiManager.bExtra)
				{
					guiManager.Indent();
					{
						guiManager.refreshLayout();

						guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED);
						guiManager.Add(guiManager.bAutoResize, OFX_IM_TOGGLE_BUTTON_ROUNDED);

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
					}
					guiManager.Unindent();
				}
			}
			*/

			guiManager.endWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui()
{
	if (!bGui) return;

	//--

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

		if (bGui_SurfingVideo)
		{
			// Controls
			draw_ImGuiControls();

			// Preview
			draw_ImGuiPreview();
		}

		if (bGui_SkipTimers && bGui_SurfingVideo) draw_ImGuiSkipTimers();

		//if (bGui_SkipTimers) draw_ImGuiSkipTimers();

		//--
	}
	guiManager.end();

	//--

	// Presets Manager
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	presetsManager.draw();
#endif

	// Moods
#ifdef USE_ofxSurfingMoods
	moods.drawGui();
#endif

	// BeatClock
#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	beatClock.draw();
#endif

	// FX Channel
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

//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_BeatTick() // callback to receive BeatTicks
{
	ofLogNotice(__FUNCTION__) << "Beat #" << beatClock.getBeat();

#ifdef USE_ofxSurfingMoods
	moods.doBeatTick();
#endif

}

////--------------------------------------------------------------
//void ofxSurfingVideoSkip::Changed_BeatBpm()
//{
//	ofLogNotice(__FUNCTION__) << "BPM " << beatClock.getBpm();
//
//	//bpmTimer.set(beatClock.getBpm());
//
////#ifdef USE_ofxSurfingMoods
////	moods.setBpm(beatClock.getBpm());
////#endif
//
//}

#endif

//--

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