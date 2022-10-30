#include "ofxSurfingVideoSkip.h"

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_PresetsStuff()
{
	//--

	// Load settings

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	ofxSurfingHelpers::loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_Preset);
#endif

	//--

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

	// Customize before add the group! that will call setup()

	presetsManager.setFliped(true);

	presetsManager.setKeyFirstChar('0');
	//presetsManager.setKeyFirstChar('a'); 
	// customize keys to avoid collide when using multiple presets manager instances!

	//presetsManager.bGui.setName("SKIP PRESETS"); //-> Custom name

	presetsManager.setPathGlobal(path_GLOBAL_Folder); // custom path
	presetsManager.setPathPresets(path_GLOBAL_Folder + "/" + "ofxSurfingVideoSkip" + "/" + "Presets");
	presetsManager.setEnableKeySpace(false); // disable space key bc used by play toggle

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

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);
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
	//params_Preset.add(bpm);
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
	//TODO:
   // External connections
   // - clock
   // - osc / midi

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

	params_Remote.add(bpm);
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

	//----

	// OSC

#ifdef USE_ofxSurfingOsc
	setup_Osc();
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
	params_AppSettings.add(bGui_BarControl);
	params_AppSettings.add(bAutoHide_BarControl);
	params_AppSettings.add(bTheme);
	params_AppSettings.add(surfingPreview.params);
	params_AppSettings.add(bGui_Main);
	params_AppSettings.add(bMODE_Edit);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup()
{
	bDISABLE_CALLBACKS = true;

	//--

	// Settings folder
	setPath_GlobalFolder("ofxSurfingVideoSkip");

	path_AppSettings = "ofxSurfingVideoSkip_App_Settings.xml";

#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	path_Preset = "ofxSurfingVideoSkip_Engine_Settings.xml";
#ifdef USE_ofxSurfingFxChannel
	path_fileName_ChannelFX = "ofxSurfingVideoSkip_ChannelFX_Settings.xml";
#endif
#endif		

	// Font big
	{
		if (font.isLoaded()) return;
		//fontSize = sz;
		string _path = FONT_FILES_PATH; // assets folder
		string f = FONT_FILE_BIG;
		//string f = "JetBrainsMono-Bold.ttf";
		_path += f;
		bool b = font.load(_path, fontSize);
		if (!b) font.load(OF_TTF_MONO, fontSize);
	}

	//--

	bLinkAllFiles.set("Link Files", true);

	bGui.set("SURFING VIDEO", true);
	bGui_Main.set("VIDEO SKIP", true);
	bGui_SkipTimers.set("SKIP TIMERS", false);
	bGui_Previews.set("MONITOR", false);
	//bGui_Previews.set("PREVIEWS", false);

	//--

	inScrub = false;

	bGui_BarControl.set("Control Bar", true); // var used to hide/show player bar and gui also
	bGui_BarControl.addListener(this, &ofxSurfingVideoSkip::Changed_VideoBarControl);

	//--

	// Skipper Engine
	videoFilePath.set("videoFilePath", "NO FILE");
	videoName.set("FILE", "NO FILE");
	videoTIME.set("", ""); // current time position

	bAutoHide_BarControl.set("AutoHide Bar", true);

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

	bSet_In.set("IN SET ", false);
	bSet_Out.set("OUT SET", false);
	bGo_In.set("IN GO  ", false);
	bGo_Out.set("OUT GO ", false);

	bENABLE_TimersGlobal.set("ENABLE TIMERS", false);
	bDoSkipTime.set("A SKIP", false);
	bDoSkipReverse.set("B REV", false);
	bMODE_SkipTime.set("A MODE SKIP", false);
	bMODE_SkipReverse.set("B MODE REV", false);

	bMODE_SkipLooped.set("SK Loop", true);
	bMODE_SkipPowered.set("SK Pow", false);
	// How much near or far (0->1) from the current position jumps to.
	skipPower.set("SK Power", 0.5f, 0.0f, 1.0f);

#ifndef USE_BPM_TIMER_MODE
	timePeriod_skipper.set("SKIP-TIME TIME", 1000, 1, 4000);
	timePeriod_reverser.set("SKIP-REVERSE TIME", 1000, 1, 4000);
#else

	// These are global.
	// will not be included into presets!
	bpm.set("BPM", 120.0f, 40.f, 400.0f);
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

#ifndef USE_MINIMAL_ofxSurfingVideoSkip
#if defined(USE_ofxSurfingFxChannel) || defined(USE_ofxSurfingFxPro) 
	indexPreviewSource.set("Source Preview", 0, 0, previewSources.size() - 1);
#else
	indexPreviewSource.set("Source Preview", 0, 0, 0);
#endif
#endif

	//--

	last_TRIG_time = 0;
	last_TRIG_reverse = 0;

	// Exclude
	bMODE_Loop.setSerializable(false);//forced true
	//bMODE_Edit.setSerializable(false);
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
	params_Engine.add(player.volume);
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

	//--

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
	_param_Clock.add(bpm);
	_param_Clock.add(bpmDivider);
#endif
	_param_Clock.add(bDoResetBpm);
	_param_SkipEngine.add(_param_Clock);

	params_Engine.add(_param_SkipEngine);

	//--

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

	params_Control.add(bGui_Main);
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

	//params_Control.add(bAutoHide_BarControl);//?
	//params_Control.add(videoName);

	params_Control.add(bTheme);

	//--

	// Channel Fx

#ifdef USE_ofxSurfingFxChannel
	setup_ChannelFx();
#endif

	//--

#ifdef USE_ofxSurfingMoods

	moods.bpmSpeed.makeReferenceTo(bpm);

	// Link Mood BPM to BeatBlock instance:
#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	moods.bpmSpeed.makeReferenceTo(beatClock.BPM_Global);
	bpm.makeReferenceTo(beatClock.BPM_Global);

	// Link Mood PLAY to BeatBlock instance:
	moods.bPLAY.makeReferenceTo(beatClock.bPlay);
#endif

	moods.setEnableExternalClock(true); // Forced. Is not mandatory, can be modified using the GUI.

	moods.setup();

	//moods.bGui.setName("MOODS");

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
	listener_bGui = bGui.newListener([this](bool&)
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

	// Audio

	player.setup();

	//--

	// Osc

	setup_Remote();

	//--

#ifdef USE_ofxNDI

	ndi.setup();
	ndi.setMode_ImGui();

	//ndi.setGuiInternalVisible(true);//debug

#endif

	//--

	// Gui

	setup_ImGui();

	//--

	if (bKeys) addKeysListeners();

	//----

	// Startup 
	// video file path will (must) be loaded now
	startup();
}

#ifdef USE_ofxSurfingOsc
//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_Osc()
{
	// To use as reference the local ui / imgui manager!
	oscHelper.setUiPtr(&ui);

	// An internal and hardcoded example 
	// of a custom template with only some params enabled and customized.
	oscHelper.setCustomTemplate(true);

	oscHelper.setup(ofxSurfingOsc::Slave);

	//--

	// Link params
	///*
	{
		/*
		oscHelper.linkValue(bpm);

		oscHelper.linkBang(bang1);
		oscHelper.linkBang(bang2);
		oscHelper.linkBang(bang3);
		oscHelper.linkBang(bang4);
		oscHelper.linkToggle(toggle1);
		oscHelper.linkToggle(toggle2);
		oscHelper.linkToggle(toggle3);
		oscHelper.linkToggle(toggle4);
		oscHelper.linkValue(value2);
		oscHelper.linkValue(value3);
		oscHelper.linkValue(value4);
		oscHelper.linkNumber(number1);
		oscHelper.linkNumber(number2);
		oscHelper.linkNumber(number3);
		oscHelper.linkNumber(number4);
		*/
	}
	//*/

	//--

	/*
	//ofxSubscribeOsc(PORT, "/bpm", bpm);

	ofxSubscribeOsc(PORT, "/bpm", [this](const float& val) {
		ofLogNotice("ofxSurfingVideoSkip") << "bpm" << val;
		bpm.set(val);
		presetsManager.playerSurfer.durationBpm.set(val);
		});

	//ofxSubscribeOsc(PORT, "/bpm", [this]() {
	//	ofLogNotice("ofxSurfingVideoSkip") << "bpm" << ;
	//	presetsManager.load(7);
	//	});

	ofxSubscribeOsc(PORT, "/beat", [this]() {
		ofLogNotice("ofxSurfingVideoSkip") << "beat!";

		presetsManager.doLoadNext();
		});

	//ofxSubscribeOsc(PORT, "/bang0", [this]() {
	//	ofLogNotice("ofxSurfingVideoSkip") << "bang 0";

	//	presetsManager.load(7);
	//	});

	ofxSubscribeOsc(PORT, "/bang1", [this]() {
		ofLogNotice("ofxSurfingVideoSkip") << "bang 1";

		presetsManager.load(8);
		});
	*/

	//--

	// Local callbacks for target changes
	ofAddListener(oscHelper.params_Targets.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Targets);
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::startup()
{
	bDISABLE_CALLBACKS = false;

	//-

	setup_PresetsStuff();

	//-

	// Overwrite default settings

	if (!bMODE_SkipLooped && !bMODE_SkipPowered) bMODE_SkipPowered = true;

	//--

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP

	pathSubs = "subs/Huxley.srt";
	//pathSubs = "subs/Alphaville.srt";
	//pathSubs = "subs/spanish.srt";

	subs.setDuration(player.getDuration());//link durations
	subs.setDisableGuiInternal(true);
	subs.setup(pathSubs);
	subs.setUiPtr(&ui);

#endif

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
	//player.setVolume(volumeVideo);
	//player.setVolume(0.0f);

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
	//TODO:
	if (bError && ofGetFrameNum() == 60 * 4) {
		doOpenDialogToSetPath();
	}

	//--

	// Load thumbs to images
	if (commandThread.isDone())
	{
		ofLogNotice("ofxSurfingVideoSkip") << "Thumbs Generated";
		loadThumbs();
	}

	//--

	player.update();

	//--

	// Feed FxPro

#ifdef USE_ofxSurfingFxPro
	{
		fxPro.begin();
		{
			draw_Video();
		}
		fxPro.end(false);
	}
#endif

	//--

	// Feed Preview 

	surfingPreview.begin();
	{
		// unprocessed video
		if (indexPreviewSource == 0)
		{
			//player.draw(0, 0);

			//get original video at original size. fails if not full hd!
			//ofRectangle r(0, 0, player.getWidth(), player.getHeight());

			//ofRectangle r = surfingPreview.getRectangle();//fit in window. fail

			//TODO:
			ofRectangle r(0, 0, 1920, 1080);//force to fix if video source is smaller...

			//-

			//player.draw(r);

			//--

			// Fx Processed
#ifdef USE_ofxSurfingFxChannel
			if (channelFx.bEnable_Fx)
			{
				channelFx.begin();
				{
					ofSetColor(255, 255, 255, 255);
					player.draw(r);
				}
				channelFx.end();
				channelFx.draw(); // draw processed
			}

			// Raw clean
			else
#endif
			{
				//ofEnableArbTex();
				ofSetColor(255, 255, 255, 255);
				player.draw(r);
			}

			//--

			// subs

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
			subs.drawRaw();
#endif
		}

		//--

#ifdef USE_ofxSurfingFxPro
		// processed video
		else if (indexPreviewSource == 1) fxPro.draw();
#else
		else if (indexPreviewSource == 1) draw_Video();
#endif
	}
	surfingPreview.end();

	//--

	// Player

	updateVideoPlayer();

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
	subs.updatePosition(player.getPosition());
#endif

	//--

	// Presets Manager

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

	// Simple callbacks 
	// 1. To detected that preset index changed
	if (presetsManager.isDoneLoad() || presetsManager.isRetrigged())
	{
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "Preset Loaded";

		// A new preset has been loaded.
		// Set position to preset, using the position start variable
		if (player.isLoaded())
		{
			position = position_In;
			player.setPosition(position);
		}
	}

#endif

	//--

	// NDI
#ifdef USE_ofxNDI

	ndi.begin_NDI_OUT();
	{
		//drawBg();

		//--

		// FxPro

		// We can choose the behavior.. 
#ifdef USE_ofxSurfingFxPro
		//if (surfingPreview.bGui_PreviewBig) fxPro.draw();
		fxPro.draw();
#else
		//if (surfingPreview.bGui_PreviewBig) draw_Video();
		draw_Video();
#endif
		//--

		ndi.drawSignals();
	}
	ndi.end_NDI_OUT();

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
		ofLogError("ofxSurfingVideoSkip") << (__FUNCTION__) << "totalNumFrames is 0. Could wait until video file is loaded...";
		return;
	}
	else
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "totalNumFrames: " << totalNumFrames;

	frameSizeNorm = 1.0f / (float)totalNumFrames;//decimals size of a frame
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "frameSizeNorm: " << frameSizeNorm;
	kickSizeFrame = frameSizeNorm * (float)numFramesToKick;
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "kickSizeFrame: " << kickSizeFrame;

	//float currDur = player.getDuration();//step proportional to video secs
	//currFrame = ofMap(position, 0, 1, 0, totalNumFrames);//curr frame?
}

//TODO:
//--------------------------------------------------------------
void ofxSurfingVideoSkip::updateConstraints()
{
	//return;

	//if (!bMODE_Edit)
	{
		// maintain start must be before finish

		// flip if inverted
		if (position_In > position_Out)
		{
			float preIn = position_In;
			position_In.setWithoutEventNotifications(position_Out);
			position_Out.setWithoutEventNotifications(preIn);
		}

		//// set a minim loop duration
		//if (position_In == position_Out)
		//{
		//	position_Out += kickSizeFrame;
		//}

		/*
		// set a minim loop duration
		if (position_In == position_Out)
		{
			position_Out += kickSizeFrame;
		}
		*/

		// flip if inverted
		// if (position_In > position_Out) 
		// {
		//	float temp = position_Out;
		//	position_In = position_Out;
		//	position_Out = temp;
		//}

		//// force if inverted
		//if (position_In > position_Out) 
		//{
		//	position_In = position_Out;
		//}
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
	//	// maintain start must be before finish
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
		// WIP
		// fix

		// Loop Engine
		// workflow: added !bMODE_Edit to allow playing out of range loop
		// but requires to improve workflow when playing preset with EDIT MODE enabled

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

	// Auto hide GUI system
	if (bAutoHide_BarControl)
		if (ofGetElapsedTimeMillis() - lastMovement < time_autoHide)
		{
			if (!bGui_BarControl.get()) bGui_BarControl = true;
		}
		else
		{
			//if (bAutoHide_BarControl)
			{
				if (bGui_BarControl.get()) bGui_BarControl = false;
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
	//----

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
			tDur = ((int)((60000.f / (float)bpm)) * beatDuration);//duration in ms. rescaled
		}
		else if (beatRescale > 0) // multiply
		{
			tDur = (beatRescale + 1) * ((int)((60000.f / (float)bpm)) * beatDuration);//duration in ms. rescaled
		}
		else // divide // < 0
		{
			tDur = (1.0f / abs(beatRescale - 1)) * ((int)((60000.f / (float)bpm)) * beatDuration);//duration in ms. rescaled
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
		tmax = divBeatSkipper.get() * ((60000 / bpmDivider.get()) / (bpm.get()));
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
		tmax = divBeatReverse.get() * ((60000 / bpmDivider.get()) / (bpm.get()));
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
			//float factor = 0.45f;
			float factor = 0.25f;

			float _skipPower = MAX(skipPower, 0.1f);

			float pow = factor * ofRandom(-_skipPower, _skipPower);
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

//#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
//	subs.drawGui();
//#endif

	//--

	//TODO: remove
	//surfingPreview.draw_Rectangle();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setGuiVisible(bool b)
{
	//bGui_Main = b;//TODO: synced...

	//TODO:
	//ENABLE_GuiVisibleByAutoHide = b;
	if (b)
	{
		//bGui_VideoControlBar_PRE = bGui_BarControl;
		//bGui_BarControl = true;
		lastMovement = ofGetElapsedTimeMillis();
	}

	// draw control bar
	//bGui_BarControl = b;

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
void ofxSurfingVideoSkip::mouseRefreshPressed(int button, float position)
{
	if (bMODE_Edit)
	{
		if (button == 0)//left
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
		else if (button == 1)//center
		{
		}
		else if (button == 2)//right
		{
			if (!bMODE_Beat)
			{
				position_Out = position;

				if (position_Out < position_In) position_In = position_Out;
			}
			/*
			else
			{
				scrubDragStart = position;
				cout << "scrubDragStart:" << scrubDragStart << endl;

			}
			*/
		}
	}
	else
	{
		position = position;
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseScrolled(ofMouseEventArgs& eventArgs)
{
	if (bActive && bGui_BarControl)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& sy = eventArgs.scrollY;

		ofRectangle bar = getBarRectangle();

		if (!bar.inside(x, y)) return;

		cout << "scroll: " << sy << endl;

		if (bMODE_Edit)
		{
			if (!mod_CONTROL)
			{
				if (bMODE_Beat)
				{
					if (sy < 0) {
						beatDuration--;
						beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
					}
					else if (sy > 0) {
						beatDuration++;
						beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
					}
				}
			}
			else
			{
				float step = mod_ALT ? 0.001f : 0.020f;

				if (sy < 0) {
					position_In -= step;
					position_Out -= step;
				}
				else if (sy > 0) {
					position_In += step;
					position_Out += step;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseDragged(ofMouseEventArgs& eventArgs)
{
	if (bActive && bGui_BarControl)
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

			mouseRefreshPressed(button, _position);

			//--

			//// workflow
			////disable loop if cursor is out-of-loop
			//if (position < position_In || position > position_Out)
			//{
			//	if (bMODE_Loop && !bMODE_Edit) bMODE_Loop = false;
			//}

			//--

			//TODO: requires more work..
			//we use mouse scroll by now.
			/*
			ofRectangle bar = getBarRectangle();

			if (bar.inside(x, y))
			{
				if (bMODE_Edit)
				{
					if (button == 2)//right
					{
						if (bMODE_Beat)
						{
							// calculate
							float _position = (x - BarInset) / getBarRectangle().width;
							_position = ofClamp(_position, 0, 1);

							scrubDragDelta = _position - scrubDragStart;
							cout << "out           :" << _position << endl;
							cout << "scrubDragDelta:" << scrubDragDelta << endl;
							cout << endl;

							float amnt = abs(scrubDragDelta);
							float thresh = 0.1;

							if (amnt > thresh)
							{
								if (scrubDragDelta < 0) {
									beatDuration--;
									beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
								}
								else if (scrubDragDelta > 0) {
									beatDuration++;
									beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
								}
							}
						}
					}
				}
			}
			*/
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mousePressed(ofMouseEventArgs& eventArgs)
{
	if (bActive && bGui_BarControl)
	{
		const int& x = eventArgs.x;
		const int& y = eventArgs.y;
		const int& button = eventArgs.button;

		ofRectangle bar = getBarRectangle();

		if (bar.inside(x, y))
		{
			bool wasPlaying = bPlay;

			ofLogVerbose("ofxSurfingVideoSkip") << (__FUNCTION__) << "mousePressed INSIDE: (" << x << "," << y << ")";

			inScrub = true;
			wasPaused = player.isPaused() || player.isMovieDone();

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
			//float _position = static_cast<float>(x - BarInset) / getBarRectangle().width;
			float _position = (x - BarInset) / getBarRectangle().width;
			_position = ofClamp(_position, 0, 1);

			player.setPosition(_position);

			//--

			mouseRefreshPressed(button, _position);

			//--

			if (button == 2)//right
			{
				if (!bMODE_Beat)
				{
				}
				else
				{
					scrubDragStart = _position;
					cout << "scrubDragStart in  :" << scrubDragStart << endl;
				}
			}

			//--

			if (wasPlaying) bPlay = true;
		}
		else
		{
			//ofLogNotice("ofxSurfingVideoSkip")<<(__FUNCTION__) << "OUTSIDE";
		}

		lastMovement = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::mouseReleased(ofMouseEventArgs& eventArgs)
{
	if (bActive && bGui_BarControl)
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

			/*
			ofRectangle bar = getBarRectangle();

			if (bar.inside(x, y))
			{
				if (bMODE_Edit)
				{
					if (button == 2)//right
					{
						if (!bMODE_Beat)
						{

						}
						else
						{
							// calculate
							float _position = (x - BarInset) / getBarRectangle().width;
							_position = ofClamp(_position, 0, 1);

							scrubDragDelta = _position - scrubDragStart;
							cout << "out           :" << _position << endl;
							cout << "scrubDragDelta:" << scrubDragDelta << endl;
							cout << endl;

							if (scrubDragDelta < 0) {
								beatDuration--;
								beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
							}
							else if (scrubDragDelta > 0) {
								beatDuration++;
								beatDuration = ofClamp(beatDuration, beatDuration.getMin(), beatDuration.getMax());
							}
							}
						}
				}
			}
			*/
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::windowResized(int _w, int _h)
{
#ifdef USE_ofxSurfingFxPro
	fxPro.windowResized(_w, _h);
#endif

#ifdef USE_ofxNDI

	ndi.windowResized(_w, _h);

#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	// Modifiers
	mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (false)
	{
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//--

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
	subs.keyPressed(key);
#endif

	//--

#ifdef USE_ofxSurfingFxPro
	fxPro.keyPressed(key);
#endif

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

#ifdef USE_ofxSurfingMoods
		else if (key == OF_KEY_RETURN)
		{
			bool b = !moods.isPlaying();
			setPlay_MoodMachine(b);
			setPlay(b);
		}
#endif

		//--

		// Edit mode
		else if (key == 'E' || key == 'e')
		{
			bMODE_Edit = !bMODE_Edit;
		}

		//--

		// Beat mode
		else if (key == 'B' || key == 'b')
		{
			bMODE_Beat = !bMODE_Beat;
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
			player.setVolumenUp();
		}
		else if (key == OF_KEY_DOWN)
		{
			player.setVolumenDown();
		}

		//--

		// Show / hide GUI

		else if (key == 'G')
		{
			bGui = !bGui; // independent to auto hide state
		}

		//--
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::keyReleased(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	// Modifiers
	mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

#ifdef USE_ofxSurfingFxPro
	fxPro.keyReleased(key);
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::dragEvent(ofDragInfo dragInfo) // drag video to load another one
{
	vector<std::string> fileList = dragInfo.files;
	videoName = ofToString(ofFilePath::getFileName(fileList[0]));
	videoFilePath = ofFilePath::getAbsolutePath(fileList[0]);

	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "video name: " << videoName;
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "video path: " << videoFilePath;
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
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << ofToString(DONE_save ? "TRUE" : "FALSE");
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_DONE_load(bool& DONE_load)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << ofToString(DONE_load ? "TRUE" : "FALSE");

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
	if (!bDISABLE_CALLBACKS)
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
			ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << name << " : " << e;
		}

		if (0) {}

		//--

		// Position

		else if (name == position.getName())
		{
			player.setPosition(position);

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
			subs.setPosition(position);
#endif

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
			/*
			// constraint
			if (position_In > position_Out)
			{
				position_Out = position_In;
			}
			*/

			/*
			// workflow
			//if (!bPlay)
			if (bMODE_Edit)
			{
				position = position_In;
			}
			*/

			/*
			if (!bPlay && !bMODE_Edit)
			{
				//bMODE_Edit = true;
			}
			*/
		}

		else if (name == position_Out.getName())
		{
			// constraint
			if (position_In > position_Out)
			{
				position_In = position_Out;
			}

			// workflow
			//if (!bPlay /*&& bMODE_Edit*/)
			//if (bMODE_Edit)//breaks
			if (!bPlay && bMODE_Edit)
			{
				bDISABLE_CALLBACKS = true;
				position = position_Out;
				player.setPosition(position);
				bDISABLE_CALLBACKS = false;
			}

			/*
			if (!bPlay && !bMODE_Edit)
			{
				//bMODE_Edit = true;
			}
			*/
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

				//// workflow
				//if (bMODE_Edit) bMODE_Edit = false;
				////if (!bMODE_Loop) bMODE_Loop = true;
			}
		}

		//--

		// edit

		else if (name == bMODE_Edit.getName())
		{
			if (bMODE_Edit)
			{
				//// workflow
				//if (bPlay) bPlay = false;
				////if (!bMODE_Loop) bMODE_Loop = true;

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

			//full
			bMODE_Beat = false;
			position_In = 0;
			position_Out = 1;

			// a beat
			//bMODE_Beat = true;
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

			bpm = 120.f;
			bpmDivider = 2;
		}

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
		else if (name == bpm.getName())
		{
			refreshTimerPlayForced();
		}
		else if (name == bpmDivider.getName())
		{
			refreshTimerPlayForced();
		}
#endif

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

		else if (name == bGui_Main.getName())
		{
			//bool b = bGui_Main.get();
			//bGui_BarControl = b;
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
				//bDISABLE_CALLBACKS = true;
				//if (bKeys.get())
				//{
				//	bKeys_Presets = false;
				//	bKeys_Fx = false;
				//}
				//bDISABLE_CALLBACKS = false;
		}

		//		else if (name == bKeys_Presets.getName())
		//		{
		//			bDISABLE_CALLBACKS = true;
		//			if (bKeys_Presets.get())
		//			{
		//				bKeys = false;
		//				bKeys_Fx = false;
		//			}
		//			bDISABLE_CALLBACKS = false;
		//
		//#ifdef USE_ofxPresetsManager__VIDEO_SKIP
		//			presetsManager.setEnableKeys(bKeys_Presets);
		//#endif
		//		}

		//else if (name == bKeys_Fx.getName())
		//{
		//	bDISABLE_CALLBACKS = true;
		//	if (bKeys_Fx.get())
		//	{
		//		bKeys_Presets = false;
		//		bKeys = false;
		//	}
		//	bDISABLE_CALLBACKS = false;
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

		else if (name == bTheme.getName())
		{
			bFboReady = false;
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
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << ofToString(bGui.get());

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

// Moods
// listeners for inside moods

//-------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_RANGE(int& targetVal)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << targetVal;

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
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << targetVal;
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_A(int& targetVal)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << targetVal;

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
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << i;

#ifdef USE_ofxSurfingFxPro
	fxPro.presetsManager.load(i);
#endif
}

//-------------------------------------------------
void ofxSurfingVideoSkip::Changed_Mood_PRESET_C(int& targetVal)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << targetVal;
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

	// Channel Fx

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

#ifdef USE_ofxSurfingFxPro
	// We can choose the behavior.. 
	if (surfingPreview.bGui_PreviewBig) fxPro.draw();
#else
	if (surfingPreview.bGui_PreviewBig) draw_Video();
#endif

	//----

	// Gui

	if (bGui)
	{
		draw_Gui();
	}

	//--

	// NDI

#ifdef USE_ofxNDI

	ndi.draw();

#endif

	// NDI

#ifdef USE_ofxNDI

	ndi.drawGui();

#endif
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

			// 1. BIG
			// Full Screen mode

			if (surfingPreview.bFullScreen)
			{
				r.scaleTo(ofGetWindowRect(), surfingPreview.scaleMode); // Full view
			}

			//--

			// 2. Floating Preview

			else
			{
				// A. Draggable view port

				if (surfingPreview.bInDocked)
				{
					r.scaleTo(ui.getRectangleCentralDocking(), surfingPreview.scaleMode);
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
			//{
			//	ofSetColor(255, 255, 255, 255);
			//	player.draw(r.x, r.y, r.width, r.height);
			//}

			//--

			// Fx Processed
#ifdef USE_ofxSurfingFxChannel
			if (channelFx.bEnable_Fx)
			{
				channelFx.begin();
				{
					ofSetColor(255, 255, 255, 255);
					player.draw(r.x, r.y, r.width, r.height);
				}
				channelFx.end();
				channelFx.draw(); // draw processed
			}

			// Raw clean
			else
#endif
			{
				//ofEnableArbTex();
				ofSetColor(255, 255, 255, 255);
				player.draw(r.x, r.y, r.width, r.height);
			}

			//--

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
			subs.draw(r);
#endif
			//--
		}
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_VideoBarControl()
{
	if (player.isLoaded())
	{
		// Draw the position bar if appropriate
		if (bGui_BarControl)
		{
			ofPushStyle();
			{
				int yy = ofGetWindowHeight() - BarInset - BarHeight;

				ofRectangle barFull = getBarRectangle();

				ofRectangle barLoopPre = getBarRectangle();
				barLoopPre.width = barFull.width * position_In;

				ofRectangle barLoopPost = getBarRectangle();
				barLoopPost.translateX(barLoopPre.width + barFull.width * (position_Out - position_In));
				barLoopPost.width = barFull.width * (1 - position_Out);

				//clamp
				barLoopPost.x = ofClamp(barLoopPost.x, BarInset, BarInset + barFull.width);
				barLoopPost.width = ofClamp(barLoopPost.width, 0, barLoopPost.x + barFull.width);

				//// 2. Filled rectangle from time 0 to current time position
				////ofDrawRectangle(barLoopPre);
				//ofDrawRectRounded(barLoopPre, BarRounded);

				//--

				// 5. Draw thumbs
				//TODO: make a cached fbo
				static ofFbo fbo;
				//static bool bFboReady = false;
				if (!bFboReady && imgThumbs.size() > 0)
				{
					bFboReady = true;
					fbo.allocate(barFull.getWidth(), barFull.getHeight());
					fbo.begin();
					ofClear(0, 255);

					{
						float w = imgThumbs[0].getWidth();
						float h = imgThumbs[0].getHeight();
						float x = 0;
						float y = 0;

						int _amount = getBarRectangle().getWidth() / w;
						float wEmpty = getBarRectangle().getWidth() - (_amount * w);
						float thumbGap = wEmpty / _amount;

						//for (int i = 0; i < (int)dirThumbs.size(); i++)
						for (int i = 0; i < (int)dirThumbs.size() - 1; i++)//TODO: workaround fix to ignore last extra thumb!
						{
							// frames border
							if (1)
							{
								ofSetLineWidth(6);//thumbs separator
								if (bTheme) ofSetColor(ofColor::white, 48); // light
								else ofSetColor(ofColor::black, 200); // dark
								ofNoFill();
								ofRectangle r(x, y, w, h);
								ofDrawRectangle(r);
								//ofDrawRectRounded(r, 2);
							}

							// thumb
							ofSetColor(ofColor::white, 255);
							ofFill();
							imgThumbs[i].draw(x, y);
							x += w;

							if (bFit)
							{
								if (i == (int)dirThumbs.size() - 2) break;//last

								// video aspect ratio
								float _w = player.getWidth();
								float _h = player.getHeight();
								float _ratio = _w / _h;

								// thumb size
								float h = BarHeight;
								float w = BarHeight * _ratio;

								//x += 10;
								x += thumbGap;
							}
						}
					}
					fbo.end();
				}
				if (bFboReady && imgThumbs.size() > 0)
				{
					float x = getBarRectangle().getTopLeft().x;
					float y = getBarRectangle().getTopLeft().y;
					fbo.draw(x, y);
				}

				//--

				// 1. Border rect only.
				// full video timeline
				ofNoFill();
				ofSetLineWidth(3.0f);
				if (bTheme) ofSetColor(ofColor::white, 48); // light
				else ofSetColor(ofColor::black, 255); // dark
				//ofSetColor(ofColor::white, BarAlpha);

				ofDrawRectangle(barFull);
				//ofDrawRectRounded(barFull, BarRounded);

				//--

				// 3. Loop clip
				// 
				// Don't draw loop bar if loop not enable
				if (bMODE_Loop)
				{
					//TODO; make inverted

					// 3. Markers loop rectangle: 
					// 
					// from loop start to loop end
					int padding = 0;
					int pStart, pWidth;

					pStart = BarInset + barFull.width * position_In;
					pWidth = (BarInset + barFull.width * position_Out) - pStart;

					pStart = ofClamp(pStart, BarInset, BarInset + barFull.width);
					pWidth = ofClamp(pWidth, 0, (BarInset + barFull.width) - pStart);

					ofRectangle barLoop = ofRectangle(pStart, yy + padding, pWidth, BarHeight - padding * 2);

					/*
					ofFill();
					// Mode zone
					*/

					/*
					ofSetColor(ofColor::white, BarAlpha);
					ofSetColor(ofColor(255), 192); // lighter grey
					//ofSetColor(ofColor(64), 192); // darker grey
					ofDrawRectangle(barLoop);
					*/

					// Mode inverted
					/*
					ofRectangle barNoLoopPre = ofRectangle(barFull.getTopLeft().x, yy + padding,
						pStart, BarHeight - padding * 2);

					ofRectangle barNoLoopPost = ofRectangle(barFull.getTopLeft().x + pWidth, yy + padding,
						pStart, BarHeight - padding * 2);
					*/

					// add opacity
					ofFill();
					//ofSetColor(ofColor(0), 150); // dark
					ofSetColor(ofColor(0), 125); // dark
					ofDrawRectangle(barLoopPre);
					ofDrawRectangle(barLoopPost);

					//--

					// 3.2 Red line marks to start/end loop

					//if (bMODE_Edit)
					{
						ofColor c;

						// filled 
						float a2 = ofMap(player.getPosition(), position_In, position_Out, 1.00, 0.00, true);
						//float a = ofMap(player.getPosition(), position_In, position_Out, 0.40, 0.00, true);
						float a = ofxSurfingHelpers::getFadeBlink(0.02, 0.40, 0.5);
						//float a = ofxSurfingHelpers::getFadeBlink(0.40, 0.70, 0.3);
						//a *= a2;
						//if (!bPlay) a = a2 = 1;

						/*
						if (!bPlay) c = ofColor(ofColor::red, 255 * a * a2);
						else c = ofColor(ofColor::red, 255 * 0.02 * a2);
						*/

						// fill
						if (bMODE_Edit)
						{
							c = ofColor(ofColor::red, 255 * a * a2);
							ofSetColor(c);
							ofFill();
							ofDrawRectangle(barLoop);
						}

						// lines
						// in/out border lines
						ofNoFill();
						ofSetLineWidth(2.0);

						//ofSetColor(ofColor::red);
						if (bMODE_Edit) c = ofColor(ofColor::red, 255 * ofMap(a * a2, 0., 1., 0.4, 1.0, true));
						else c = ofColor(ofColor::red, 255 * 0.4);

						// lines
						ofSetColor(c);
						ofDrawLine(pStart, yy + padding, pStart, yy + BarHeight - 1);
						ofDrawLine(pStart + pWidth, yy + padding, pStart + pWidth, yy + BarHeight - 1);

						// rectangle
						ofSetColor(ofColor(ofColor::red, 255 * 0.4));
						ofRectangle r(pStart, yy + padding, pWidth, BarHeight - 1);
						ofDrawRectangle(r);
					}
				}

				//-

				// 4. Red line for current video player time
				ofNoFill();
				ofSetColor(ofColor::red);
				ofSetLineWidth(4.0);
				//float posTime = barLoopPre.width + BarInset;
				float posTime = BarInset + barFull.width * player.getPosition();
				posTime = ofClamp(posTime, BarInset, BarInset + barFull.width);
				int padding = 1;
				ofDrawLine(posTime, yy - padding, posTime, yy + BarHeight + padding);
			}
			ofPopStyle();
		}
	}

	// Error. No loaded video!
	else
	{
		bool bBlink = true;
		float a = 0.5f;
		const ofColor c_ = ofColor::white;
		ofColor c;
		if (bBlink)
		{
			float v = ofxSurfingHelpers::getFadeBlink();
			a = ofClamp(v, BLINK_MIN, BLINK_MAX);
			c = ofColor(c_.r, c_.g, c_.b, c_.a * a);
		}

		if (player.isError())
		{
			bError = true;
			float w = ofxSurfingHelpers::getWidthBBtextBoxed(font, "MUST PICK A VIDEO FILE!");
			ofxSurfingHelpers::drawTextBoxed(font, "MUST PICK A VIDEO FILE!", ofGetWidth() / 2 - w / 2, 40, c);
			//player.getError() 
		}
		else
		{
			bError = false;
			ofxSurfingHelpers::drawTextBoxed(font, "LOADING VIDEO FILE...", 20, 40, c);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::loadThumbs()
{
	//dirThumbs.listDir("../");
	dirThumbs.listDir("/thumbs/");
	dirThumbs.allowExt("jpg");
	dirThumbs.sort();

	//allocate the vector to have as many ofImages as files
	if (dirThumbs.size()) {
		imgThumbs.assign(dirThumbs.size(), ofImage());
	}

	for (int i = 0; i < (int)dirThumbs.size(); i++) {
		imgThumbs[i].load(dirThumbs.getPath(i));
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::loadMovie(std::string _path)
{
	bool b = player.load(_path, bLinkAllFiles);

	if (b)
	{
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "LOADED VIDEO '" << _path << "'";

		calculateKick(); // just try bc if video is not loaded will need to do again
		lastMovement = ofGetElapsedTimeMillis();
	}
	else
	{
		// try a default alternative file

		ofLogError("ofxSurfingVideoSkip") << (__FUNCTION__) << "VIDEO NOT FOUND '" << _path << "' z!";

		_path = "movies/SampleHap.mov";
		ofLogWarning("ofxSurfingVideoSkip") << (__FUNCTION__) << "...TRYING TO LOAD A BACKUP MOVIE: '" << _path << "' !";

		b = player.load(_path, bLinkAllFiles);

		if (!b) ofLogError("ofxSurfingVideoSkip") << (__FUNCTION__) << "BAD ERROR!";
	}
	bLoaded = b;

	//--

	// To fix startup bug..
#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	{
		std::string _path = ofToString("ofxSurfingPresets/" + videoName.get());
		auto ss = ofSplitString(_path, ".");
		if (ss.size() > 0) _path = ss[0];
		ofStringReplace(_path, "\"", "");
		presetsManager.setPathPresets(_path);
	}
#endif

	if (!bLoaded)
	{
		// TODO: + fix presets path empty on startup
		// skipper engine set without name
		// bc settings could store last file path but maybe could be moved,
		// so last path is wrong! That happens when moving the video file...
		videoFilePath.set("NO FILE");
		videoName.set("NO FILE");
		videoTIME.set(""); // current time position
	}

	//--

	//TODO; 
#ifdef AUTO_GENERATE_THUMBS_ON_LOADING
	if (bLoaded)
	{
		doGenerateThumbs();
	}
#else 
	// Load thumbs to images
	loadThumbs();
	bFboReady = false;
#endif

	//----

	//TODO:
	//add dialog loader
	// Audio
	//player.loadAudio("Z:\\_DATA\\VIDEO\\HUXLEY2\\HUXLEY2.wav");

	//--

	if (bLinkAllFiles) 
	{
#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
		pathSubs = player.getPathSrt();
		subs.setDuration(player.getDuration());//link durations
		subs.load(pathSubs);
#endif
	}

	//--

	// workflow
	// autostart play
	if (!bPlay) bPlay = true;
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
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_GLOBAL_Folder + "/" + path_AppSettings);

	//	// Channel fx
	//#ifdef USE_ofxSurfingFxChannel
	////#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	////	saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_ChannelFX);
	////#endif
	//#endif

		//-

		// Callbacks
	ofRemoveListener(params_Engine.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Params);

	// disable for the moment
	//ofRemoveListener(params_Control_VideoFX.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_params_VideoFX);

	// auto hide callback
	bGui_BarControl.removeListener(this, &ofxSurfingVideoSkip::Changed_VideoBarControl);

	//-

	// Presets Manager
#ifdef USE_ofxPresetsManager__VIDEO_SKIP
	presetsManager.DONE_save.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_save);
	presetsManager.DONE_load.removeListener(this, &ofxSurfingVideoSkip::Changed_DONE_load);
	presetsManager.exit();
#endif

	// Save states when not using presets!
#ifndef USE_ofxPresetsManager__VIDEO_SKIP
	ofxSurfingHelpers::saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_Preset);

#endif
	// 
		//--

#ifdef USE_ofxSurfingMoods
	// Listener for inside class moods
	moods.RANGE_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_RANGE);
	moods.TARGET_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_TARGET);
	moods.PRESET_A_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_A);
	moods.PRESET_B_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_B);
	moods.PRESET_C_Selected.removeListener(this, &ofxSurfingVideoSkip::Changed_Mood_PRESET_C);
#endif

	//--

#ifdef USE_ofxSurfingOsc 
	ofRemoveListener(oscHelper.params_Targets.parameterChangedE(), this, &ofxSurfingVideoSkip::Changed_Targets);
#endif

	//player.exit();
}

#ifdef USE_ofxSurfingOsc 
//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_Targets(ofAbstractParameter& e)
{
	///*

	//if (oscHelper.bBypass) return;

	string _name = e.getName();

	if (0) ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << _name << " : " << e;

	//--

	// BOOLS / BANGS 

	if (e.type() == typeid(ofParameter<bool>).name())
	{
		ofParameter<bool> p = e.cast<bool>();
		if (p.get())//get true only
		{
			if (_name == "BANG_" + ofToString(0))
			{
				ofLogNotice("ofxSurfingVideoSkip") << "Bang 0 >";
				//presetsManager.doLoadNext();
#ifdef USE_ofxSurfingMoods
				moods.doBeatTick();
#endif
				return;
			}

			if (_name == "BANG_" + ofToString(1))
			{
				ofLogNotice("ofxSurfingVideoSkip") << "Bang 1 > ";
				//presetsManager.doLoadNext();
				//presetsManager.load(7);
#ifdef USE_ofxSurfingMoods
				moods.doBeatTick();
#endif
				return;
			}

			if (_name == "BANG_" + ofToString(2))
			{
				ofLogNotice("ofxSurfingVideoSkip") << "Bang 2 > ";
				//presetsManager.doLoadNext();
				//presetsManager.load(7);
#ifdef USE_ofxSurfingMoods
				moods.doBeatTick();
#endif
				return;
			}
		}
	}

	//--

	// FLOATS

	else if (e.type() == typeid(ofParameter<float>).name())
	{
		ofParameter<float> p = e.cast<float>();

		//ofLogNotice("ofxSurfingVideoSkip") << ofToString(p, 2);

		if (_name == "VALUE_" + ofToString(0))
		{
			//ofLogNotice("ofxSurfingVideoSkip") << "Value 0 > " << ofToString(p, 2);
			return;
		}

		if (_name == "VALUE_" + ofToString(1))
		{
			//ofLogNotice("ofxSurfingVideoSkip") << "Value 1 > " << ofToString(p, 2);
#ifdef USE_ofxSurfingMoods
			//moods.controlManual = 1 - p; // flip
			moods.controlManual = p;
#endif
			return;
		}

		if (_name == "VALUE_" + ofToString(2)) // bpm
		{
			ofLogNotice("ofxSurfingVideoSkip") << "Value 2 > BPM " << ofToString(p, 2);
			bpm.set(p);
			return;
		}
	}

	//*/
}
#endif

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setPath_GlobalFolder(std::string folder)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << folder;
	path_GLOBAL_Folder = folder;
	ofxSurfingHelpers::CheckFolder(folder);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::setup_ImGui()
{
	//ui.setup();
	ui.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);

	// Customize default labels
	//ui.setLabelLayoutPanels("SURFING_VIDEO"); // global title

	//--

	// -> 1. Layouts Presets 
	// passing toggles

	// This bool toggles will control the show of the added window
	// and will be added too to layout presets engine

	ui.addWindowSpecial(bGui_Main);

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP
	ui.addWindowSpecial(presetsManager.bGui);
#endif

#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
	ui.addWindowSpecial(mMidiParams.bGui);
#endif

#ifdef USE_ofxSurfingMoods 
	ui.addWindowSpecial(moods.bGui);
#endif

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP
	ui.addWindowSpecial(beatClock.bGui);
#endif

#ifdef USE_ofxSurfingFxChannel
	ui.addWindowSpecial(channelFx.bGui);
#endif

#ifdef USE_ofxSurfingFxPro
	ui.addWindowSpecial(fxPro.bGui);
#endif

#ifdef USE_ofxSurfingOsc
	ui.addWindowSpecial(oscHelper.bGui);
	//ui.addWindowSpecial(oscHelper.bGui_Targets);
#endif

#ifdef USE_ofxNDI
	ui.addWindowSpecial(ndi.bGui);
#endif

	//#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
	//	ui.addWindowSpecial(subs.bGui);
	//#endif

	//ui.addWindowSpecial(surfingPreview.bGui_Extra);

	//--

	// -> 2. Extra Params to include into Layout Presets

	//// Preview
	////ui.addExtraParamToLayoutPresets(surfingPreview.getParameters());//group
	//ui.addExtraParamToLayoutPresets(surfingPreview.bGui);
	//ui.addExtraParamToLayoutPresets(surfingPreview.bGui_PreviewBig);
	////ui.addExtraParamToLayoutPresets(surfingPreview.bFullScreen);
	//ui.addExtraParamToLayoutPresets(bGui_BarControl);

	//// Other
	//ui.addExtraParamToLayoutPresets(ui.bMinimize);
	//ui.addExtraParamToLayoutPresets(bGui_SkipTimers);

	////ui.addExtraParamToLayoutPresets(presetsManager.playerSurfer.bGui);

#ifdef USE_ofxSurfingOsc
	ui.addExtraParamToLayoutPresets(oscHelper.bGui_Plots);
#endif

#ifdef USE_ofxSurfingMoods 
	ui.addExtraParamToLayoutPresets(moods.bGui_PreviewWidget);
	ui.addExtraParamToLayoutPresets(moods.bGui_ManualSlider);
#endif

	//--

	// -> Initiates when adding process finished!
	ui.startup();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui_SkipTimers()
{
	if (bGui_SkipTimers)
	{
		ui.setNextWindowAfterWindowNamed(bGui_Main);
		//if (bGui_Main) ui.setNextWindowAfterWindowNamed(bGui_Main.getName());

		if (ui.BeginWindow(bGui_SkipTimers))
		{
			float ___w1 = ui.getWidgetsWidth(1);
			float ___w2 = ui.getWidgetsWidth(2);
			float ___h = ui.getWidgetsHeightUnit();

			// Manual triggers
			ui.Add(bDoSkipTime, OFX_IM_BUTTON_BIG, 2, true);
			string s = "Manually Trig a Skip Time";
			ui.AddTooltip(s);

			ui.Add(bDoSkipReverse, OFX_IM_BUTTON_BIG, 2, false);
			s = "Manually Trig a Rev Direction";
			ui.AddTooltip(s);

			ui.AddSpacingSeparated();

			//--

			// Big enablers
			ui.Add(bENABLE_TimersGlobal, OFX_IM_TOGGLE_BIG_XXL_BORDER);
			s = "Global ByPass for both Timers";
			ui.AddTooltip(s);

			ui.AddSpacing();

			//--

			if (bENABLE_TimersGlobal)
			{
				if (!ui.bMinimize)
				{
					if (ui.BeginTree("BPM CLOCK", bMODE_SkipTime || bMODE_SkipReverse))
					{
						ui.Add(bpm, OFX_IM_DEFAULT);
						ui.Add(bpmDivider, OFX_IM_DEFAULT);

						if (ui.AddButton("HALF", OFX_IM_BUTTON_SMALL, 2))
						{
							bpm /= 2.0f;
						}

						ui.SameLine();

						if (ui.AddButton("DOUBLE", OFX_IM_BUTTON_SMALL, 2))
						{
							bpm *= 2.0f;
						}

						// Reset
						ui.Add(bDoResetBpm, OFX_IM_BUTTON_SMALL);

						ui.EndTree();
					}

					ui.AddSpacingSeparated();
				}

				//--

				// Skip Time

				ui.Add(bMODE_SkipTime, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);
				string s = "When Timer happens position is randomized";
				ui.AddTooltip(s);

				if (bMODE_SkipTime)
					if (bENABLE_TimersGlobal)
					{
						if (ui.BeginTree("A SKIP TIME", bMODE_SkipTime.get()))
						{
							if (bMODE_SkipTime && bENABLE_TimersGlobal)
							{
								ui.Add(divBeatSkipper, OFX_IM_DEFAULT);

								// Draw progress bar
								ui.Add(timer_SkipTime, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							ui.Add(bMODE_SkipLooped, OFX_IM_TOGGLE_SMALL, 2, true);
							string s = "Random goes to any point";
							ui.AddTooltip(s);

							ui.Add(bMODE_SkipPowered, OFX_IM_TOGGLE_SMALL, 2, false);
							s = "Random goes to a closer or far point";
							ui.AddTooltip(s);

							if (bMODE_SkipPowered)
							{
								ui.Add(skipPower);
								string s = "How much close or far is the random";
								ui.AddTooltip(s);
							}

							ui.EndTree();
						}

						ui.refreshLayout();
					}

				//--

				ui.AddSpacingSeparated();

				// Skip Rev

				ui.Add(bMODE_SkipReverse, OFX_IM_TOGGLE_BIG_XXL_BORDER_BLINK);
				s = "When Timer happens direction is switched";
				ui.AddTooltip(s);

				if (bMODE_SkipReverse)
					if (bENABLE_TimersGlobal)
					{
						if (ui.BeginTree("B SKIP REV", bMODE_SkipReverse.get()))
						{
							ui.refreshLayout();

							if (bMODE_SkipReverse && bENABLE_TimersGlobal)
							{
								ui.Add(divBeatReverse, OFX_IM_DEFAULT);

								// draw progress bar
								ui.Add(timer_SkipRev, OFX_IM_PROGRESS_BAR_NO_TEXT);
							}

							ui.EndTree();
						}
						ui.refreshLayout();
					}

				ui.AddSpacingSeparated();

				ui.Add(bDoResetEngine, OFX_IM_BUTTON_SMALL);
			}

			ui.EndWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui_Preview()
{
	// Append the source name to the window header label.

	string n = "";

#ifndef USE_MINIMAL_ofxSurfingVideoSkip
#if defined(USE_ofxSurfingFxChannel) || defined(USE_ofxSurfingFxPro) 
	if (indexPreviewSource < previewSources.size()) n = previewSources[indexPreviewSource];
#endif
#endif

	static ofParameter<bool> bLock{ "Lock", false };

	//--
	// 
	//TODO: look ImGui demo example
	// https://github.com/ocornut/imgui/issues/5627
	// 
	// 1. Preview Floating window

	if (surfingPreview.bGui_MiniPreview)
	{
		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (surfingPreview.bAutoResize_Preview) flagsw += ImGuiWindowFlags_AlwaysAutoResize;

		static bool bready = false;

		//static float step;
		//static bool bdostep = false;

		float _w = surfingPreview.fboPreview.getWidth();
		float _h = surfingPreview.fboPreview.getHeight();
		//_w += 100;

		float _ratio = _w / _h;

		//TODO:
		// https://github.com/ocornut/imgui/issues/5627#event-7303371114
		//float aspect_ratio = 1;
		//float aspect_ratio = surfingPreview.fboPreview.getWidth() / surfingPreview.fboPreview.getHeight();
		//float aspect_ratio = 16.0f / 9.0f;

		/*
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), AspectRatio, (void*)&aspect_ratio);   // Aspect ratio\
		*/

		///*
		static ofRectangle win;// = ui.getWindowShape();
		static float spx;// = ui.getWidgetsSpacingX();
		static float spy;// = ui.getWidgetsSpacingY();
		static float wwin;// = win.getWidth();
		static float hwin;// = win.getHeight();//not used

		//static float wwintar;
		//static float hwintar;
		//static float wwintar = wwin + (2 * spx);
		//static float hwintar = (wwin * _ratio) + (2 * spy);

		static float wwintar;
		wwintar = wwin;
		//if (bdostep) {
		//	bdostep = false;
		//	wwintar += step;
		//}

		static float hwintar;
		hwintar = wwin / _ratio;

		/*
		float frame_height = ImGui::GetFrameHeight();
		hwintar += frame_height;
		hwintar += ImGui::GetStyle().WindowBorderSize * 2;
		//hwintar += 15;
		*/

		float hoffset = 0;
		hoffset += ImGui::GetFrameHeight();// For title-bar. Multiply by 2 if you have a menu.
		hoffset += 1 * ImGui::GetStyle().WindowPadding.y;

		if (bready)
		{
			ImGui::SetNextWindowSize(ImVec2(wwintar, hwintar + hoffset), ImGuiCond_Always);
			//ImGui::SetNextWindowSize(ImVec2(wwintar, hwintar),
			//	bLock.get() ? ImGuiCond_Always : ImGuiCond_Appearing);
		}

		//--

		if (ui.BeginWindow(surfingPreview.bGui_MiniPreview, flagsw))
		{
			///*
			spx = ui.getWidgetsSpacingX();
			spy = ui.getWidgetsSpacingY();
			win = ui.getWindowShape();
			wwin = win.getWidth();
			hwin = win.getHeight();
			bready = true;
			//*/

			ofxImGuiSurfing::DrawFboPreview(surfingPreview.fboPreview);

			//string n = surfingPreview.bGui_MiniPreview.getName();
			//ui.AddLabelBig(n);

			//--

			//TODO:
			// lock ratio
			// wheel to resize
			/*
			{
				bool bCtrl = ImGui::GetIO().KeyCtrl; // CTRL key to fine tunning

				ImGui::SetItemUsingMouseWheel();

				if (ImGui::IsItemHovered())
				{
					float wheel = ImGui::GetIO().MouseWheel;

					if (wheel)
					{
						if (ImGui::IsItemActive())
						{
							ImGui::ClearActiveID();
						}

						float resolution = 10;//px
						//float resolution = 1.f / MOUSE_WHEEL_STEPS;

						step = wheel * (bCtrl ? resolution * MOUSE_WHEEL_FINETUNE_CTRL_RATIO : resolution);
						bdostep = true;
					}
				}
			}
			*/

			//--

			ui.EndWindow();
		}
	}

	//--

	// 2. Preview Extra stuff window

	if (!ui.bMinimize)
	{
		if (surfingPreview.bGui_Extra)
		{
			if (ui.BeginWindow(surfingPreview.bGui_Extra))
			{
				ui.Add(bLock, OFX_IM_TOGGLE_ROUNDED_MINI);

				// Preview Big
				ui.Add(surfingPreview.bGui_PreviewBig, OFX_IM_TOGGLE_ROUNDED);
				if (surfingPreview.bGui_PreviewBig)
				{
					ui.Indent();
					{
						ui.AddCombo(surfingPreview.scaleModeIndex, surfingPreview.scaleModenames);

						//-

						ui.Add(surfingPreview.bFullScreen, OFX_IM_TOGGLE_ROUNDED);
						if (!surfingPreview.bFullScreen)
						{
							ui.Add(surfingPreview.bInDocked, OFX_IM_TOGGLE_ROUNDED_MINI);
							if (!surfingPreview.bInDocked)
								ui.Add(surfingPreview.rectDraggable.bEditMode, OFX_IM_TOGGLE_ROUNDED_MINI);
						}

						//TODO:
						//ui.Add(colorBg);//TODO: fix
					}
					ui.Unindent();
				}

				//--

				// Preview Mini Floating

				ui.Add(surfingPreview.bGui_MiniPreview, OFX_IM_TOGGLE_ROUNDED);
				if (surfingPreview.bGui_MiniPreview)
				{
					ui.Indent();
					ui.Add(surfingPreview.bAutoResize_Preview, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui.Unindent();
				}

				// Debug only for floating window active
				if (surfingPreview.bGui_MiniPreview)
				{
					ui.Separator();

					ui.Add(surfingPreview.bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);
					if (surfingPreview.bDebug)
					{
						ui.Indent();
						ofxImGuiSurfing::BasicInfosWindow();
						ui.Unindent();
					}
				}

				ui.EndWindow();
			}
		}
	}

	// if minimized will draw only the floating preview, not the extra window
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui_Main()
{
	//--

	// Engine

	// Skip player window

	if (bGui_Main)
	{
		//IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;
		ofxImGuiSurfing::SetWindowContraintsWidth(200);

		if (ui.BeginWindow(bGui_Main))
		{
			float ___w1;
			float ___w2;
			float ___h = ui.getWidgetsHeightUnit();

			// Minimize
			if (ui.Add(ui.bMinimize, OFX_IM_TOGGLE_ROUNDED))
			{
				if (ui.bMinimize) surfingPreview.bGui_Extra = false;
			};

			ui.AddSpacingSeparated();

			//--

			// Video Title / File

			if (!ui.bMinimize)
			{
				if (ui.BeginTree("FILES"))
				{
					std::string n = videoName;

					//if (ui.BeginTree(n))
					if (ui.BeginTree("VIDEO"))
					{
						//bool bBlink = (n == "\"NO FILE\"");//video file path not settled yet!
						bool bBlink = !player.isLoaded();

						ui.BeginBlinkText(bBlink);
						{
							ui.AddLabel(videoName.get());
							string s = videoFilePath.get();
							ui.AddTooltip(s);
						}
						ui.EndBlinkText(bBlink);

						ui.AddSpacing();

						//TODO:
						//SurfingGuiTypes st = (bBlink ? OFX_IM_BUTTON_SMALL_BORDER_BLINK : OFX_IM_BUTTON_SMALL);
						SurfingGuiTypes st = OFX_IM_BUTTON_SMALL;

						ui.BeginBlinkFrame(bBlink);
						{
							if (ui.AddButton("Open File", st))
							{
								doOpenDialogToSetPath();
							}
						}
						ui.EndBlinkFrame(bBlink);

						if (ui.AddButton("Generate Thumbs", st))
						{
							doGenerateThumbs();
						}

						ui.AddSpacing();

						//--

						ui.EndTree();
					}

					//--

					if (ui.BeginTree("AUDIO"))
					{
						bool bBlink = !player.isLoadedAudio();
						ui.BeginBlinkText(bBlink);
						{
							ui.AddLabel(player.name_Audio);
							string s = player.getPathAudio();
							ui.AddTooltip(s);
						}
						ui.EndBlinkText(bBlink);

						ui.AddSpacing();

						SurfingGuiTypes st = OFX_IM_BUTTON_SMALL;
						if (ui.AddButton("Open Audio File", st))
						{
							player.doOpenDialogPathAudio();
						}

						ui.Add(player.volume, OFX_IM_HSLIDER_MINI);
						//ui.Add(player.volume, OFX_IM_KNOB_DOTKNOB, 2);

						//TODO:
						// center
						/*
						string lb = "AUDIO";
						float w = ui.getWidgetsWidth(4);
						ui.pushStyleFont(2);
						auto sz = ImGui::CalcTextSize(lb.c_str()).x;
						ui.popStyleFont();
						ImGui::Dummy(ImVec2(w + sz / 2, 0));
						//ImGui::Dummy(ImVec2(w, 0));
						ImGui::SameLine();
						ui.AddLabelBig(lb);

						ImGui::Dummy(ImVec2(w, 0));
						ImGui::SameLine();
						ui.Add(player.volume, OFX_IM_KNOB_DOTKNOB, 2);
						//ui.Add(volumeVideo, OFX_IM_STEPPER);
						*/

						ui.EndTree();
					}

					ui.EndTree();
				}

				ui.AddSpacingSeparated();
			}

			//--

			// Preview Screens

			if (ui.Add(bGui_Previews, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM))
			{
				// workflow
				if (!bGui_Previews) surfingPreview.bGui_Extra = false;
			};

			// minimized
			if (ui.bMinimize) {
				if (bGui_Previews)
				{
					ui.AddSpacing();
					ui.Indent();
					{
						// Preview Big
						ui.Add(surfingPreview.bGui_PreviewBig, OFX_IM_TOGGLE_BUTTON_ROUNDED);

						// Preview Float
						ui.Add(surfingPreview.bGui_MiniPreview, OFX_IM_TOGGLE_BUTTON_ROUNDED);

						// Source Selector
#ifndef USE_MINIMAL_ofxSurfingVideoSkip
#if defined(USE_ofxSurfingFxChannel) || defined(USE_ofxSurfingFxPro) 
						if (surfingPreview.bGui_MiniPreview)
						{
							if (previewSources.size() > 1)
								ui.AddCombo(indexPreviewSource, previewSources);
						}
#endif
#endif
					}
					ui.Unindent();
				}
			}

			// expanded
			//if (!ui.bMinimize)
			else {
				if (bGui_Previews)
				{
					ui.Indent();
					{
						// Preview Float
						ui.Add(surfingPreview.bGui_MiniPreview, OFX_IM_TOGGLE_BUTTON_ROUNDED);

						// Preview Big
						ui.Add(surfingPreview.bGui_PreviewBig, OFX_IM_TOGGLE_BUTTON_ROUNDED);

						//if (surfingPreview.bGui_PreviewBig)
						{
							ui.Indent();

							// Control Bar
							ui.Add(bGui_BarControl, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							if (bGui_BarControl) ui.Add(bAutoHide_BarControl, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							ui.Add(bTheme, OFX_IM_TOGGLE_ROUNDED_MINI);

							ui.Unindent();
						}

						// Source Selector
#ifndef USE_MINIMAL_ofxSurfingVideoSkip
#if defined(USE_ofxSurfingFxChannel) || defined(USE_ofxSurfingFxPro) 
						if (surfingPreview.bGui_MiniPreview)
						{
							if (previewSources.size() > 1)
								ui.AddCombo(indexPreviewSource, previewSources);
						}
#endif
#endif
						ui.Indent();
						ui.Add(surfingPreview.bGui_Extra, OFX_IM_TOGGLE_BUTTON_ROUNDED_MINI);
						ui.Unindent();
					}
					ui.Unindent();
				}
			}

			//--

#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
			ui.AddSpacingSeparated();
			ui.Add(subs.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED);
#endif
			//--

#if defined(USE_ofxSurfingFxChannel) || defined(USE_ofxSurfingFxPro) 
			ui.Add(channelFx.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED);
#endif
			//--

			// Skip Panel

			ui.AddSpacingSeparated();

			ui.Add(bGui_SkipTimers, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

			ui.AddSpacingSeparated();

			//--

			{
				// Play

				___w1 = ui.getWidgetsWidth(1);
				___w2 = ui.getWidgetsWidth(2);

				ofxImGuiSurfing::AddBigToggleNamed(bPlay, ___w1, 3.0f * ___h, "PLAYING", "PLAY", true);

				//if (!ui.bMinimize)
				{
					ui.AddSpacing();
					ui.Add(bMODE_Edit, OFX_IM_TOGGLE_MEDIUM_BORDER);
				}

				ui.AddSpacingSeparated();

				//--

				// Preset Clicker

#ifdef USE_ofxSurfingPresets__VIDEO_SKIP

				//ui.Add(presetsManager.bGui, OFX_IM_TOGGLE_ROUNDED);

				if (!presetsManager.bGui)
				{
					//ui.AddLabelBig("PRESETS", false, true);

					bool bHeader = true;
					bool bMinimal = ui.bMinimize;
					bool bShowMinimize = true;
					bool bNoExtras = false;

					presetsManager.draw_ImGui_ClickerSimple(bHeader, bMinimal, bShowMinimize, bNoExtras);

					ui.AddSpacingSeparated();
				}
#endif
				//--

				// Time 

				string s;
				if (ui.bMinimize) s = (videoTIME.get()).data();
				else s = ("Time " + videoTIME.get()).data();

				ui.AddLabelBig(s, false, true);
				ui.AddSpacing();

				//--

				// Position Bigger

				ofParameter<float> p = position.cast<float>();
				auto tmpRef = p.get();
				std::string name = p.getName();
				bool bReturn = false;

				___w1 = ui.getWidgetsWidth(1);
				___w2 = ui.getWidgetsWidth(2);

				if (bMODE_Loop)
				{
					bReturn = (ofxImGuiSurfing::AddHSliderRanged2(p, ImVec2(___w1, ___h), position_In, position_Out, true, true));
					IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(p);
				}
				else
				{
					bReturn = (ofxImGuiSurfing::AddHSlider(p, ImVec2(___w1, ___h), true, true));
					IMGUI_SUGAR__SLIDER_ADD_MOUSE_WHEEL(p);
				}
				if (bReturn)
				{
					position = p;
				}

				//--

				ui.AddSpacingSeparated();

				//--

				// Range

				if (bMODE_Loop)
				{
					// In / Out 

					string s;
					if (!bMODE_Beat)s = "IN > OUT";
					else s = "IN > DURATION";
					ui.AddLabelBig(s, false, true);

					ui.Add(position_In, OFX_IM_HSLIDER_MINI_NO_LABELS);
					s = "Mark In";
					ui.AddTooltip(s);

					if (!bMODE_Beat)
					{
						ui.Add(position_Out, OFX_IM_HSLIDER_MINI_NO_LABELS);
						s = "Mark Out";
						ui.AddTooltip(s);

						//ui.Add(position_Out);
					}
					else
					{
						/*
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ui.Add(position_Out, OFX_IM_HSLIDER_MINI_NO_LABELS);
						ImGui::PopItemFlag();
						*/

						ui.Add(beatDuration, OFX_IM_HSLIDER_MINI_NO_NAME);
						s = "Beats Duration";
						ui.AddTooltip(s);

						//ui.Add(position_Out, OFX_IM_INACTIVE);
					}

					ui.AddSpacingSeparated();
				}

				//--

				// Beat mode

				if (bMODE_Loop)
				{
					ui.Add(bMODE_Beat, OFX_IM_TOGGLE_MEDIUM);
					string s = bMODE_Beat ?
						"Out Mark is settled \nby Beats Duration" : "Out Mark can be \nmanually settled";
					ui.AddTooltip(s);
					//ui.AddSpacing();

					if (bMODE_Beat)
					{
						//ui.Add(beatDuration, OFX_IM_SLIDER);
						if (!ui.bMinimize) {
							ui.Add(beatRescale, OFX_IM_STEPPER);
							s = "Beat Scale";
							ui.AddTooltip(s);
						}
					}

					ui.AddSpacingSeparated();
				}

				//--

				// Loop / Reverse

				ui.Add(bMODE_LoopedBack, OFX_IM_TOGGLE_MEDIUM, 2, true);
				s = "Alternate Direction";
				ui.AddTooltip(s);
				ui.Add(bMODE_Reversed, OFX_IM_TOGGLE_MEDIUM, 2);
				s = "Direction";
				ui.AddTooltip(s);
				ui.AddSpacingSeparated();

				//--

				// Speed

				ui.Add(speedNorm, OFX_IM_SLIDER);
				if (!ui.bMinimize) ui.Add(speedNorm, OFX_IM_STEPPER);
				ui.Add(bDoResetSpeed, OFX_IM_BUTTON_SMALL);
				ui.AddSpacingSeparated();

				//--

				{
					ui.refreshLayout();

					// FineTune
					{
						static ofParameter<bool> bFineTune{ "FINE TUNE", false };
						ui.Add(bFineTune, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
						//ui.AddSpacing();

						if (bFineTune)
						{
							ui.Indent();
							{
								ui.AddLabelBig(("Frame " + videoFRAME.get()).data());

								ui.AddSpacing();

								//--

								// Mark clip In / Out

								//if (bMODE_Edit)

								if (bMODE_Loop)
								{
									ui.AddSpacing();

									if (bMODE_Beat)
									{
										ui.Add(bSet_In, OFX_IM_BUTTON_SMALL, 2, true);
										ui.Add(bGo_In, OFX_IM_BUTTON_SMALL, 2);
									}
									else
									{
										ui.Add(bSet_In, OFX_IM_BUTTON_SMALL, 2, true);
										ui.Add(bSet_Out, OFX_IM_BUTTON_SMALL, 2);
									}
									ui.AddSpacing();
								}

								//--

								{
									ui.PushButtonRepeat(true);
									{
										if (ui.AddButton("-", OFX_IM_BUTTON_MEDIUM, 2))
										{
											//workflow
											if (bPlay) bPlay = false;

											bDoKickL = true;
										}
										ui.SameLine();
										if (ui.AddButton("+", OFX_IM_BUTTON_MEDIUM, 2))
										{
											//workflow
											if (bPlay) bPlay = false;

											bDoKickR = true;
										}
									}
									ui.PopButtonRepeat();
								}

								//ui.AddSpacing();

								ui.Add(position, OFX_IM_STEPPER);

								if (bMODE_Loop)
								{
									ui.AddSpacing();
									ui.Add(position_In, OFX_IM_STEPPER);

									if (!bMODE_Beat) ui.Add(position_Out, OFX_IM_STEPPER);
								}

								//--

								// Mark clip In / Out

								//if (bMODE_Edit)

								if (bMODE_Loop)
								{
									ui.AddSpacing();

									if (bMODE_Beat)
									{
									}
									else
									{
										ui.Add(bGo_In, OFX_IM_BUTTON_SMALL, 2, true);
										ui.Add(bGo_Out, OFX_IM_BUTTON_SMALL, 2);
									}
								}
							}
							ui.Unindent();
						}
					}
				}
			}

			//--

			if (bMODE_Loop)
			{
				ui.AddSpacingSeparated();

				ui.Add(bDoResetAll, OFX_IM_BUTTON_SMALL);
			}

			//--

			/*
			if (!ui.bMinimize)
			{
				ui.AddSpacingSeparated();
				ui.refreshLayout();

				// Extra

				ui.Add(ui.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED);
				if (ui.bExtra)
				{
					ui.Indent();
					{
						ui.refreshLayout();

						ui.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED);
						ui.Add(ui.bAutoResize, OFX_IM_TOGGLE_BUTTON_ROUNDED);

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
					ui.Unindent();
				}
			}
			*/

			ui.EndWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui_Docking()
{
	ui.BeginDocking();
	{
		//dockingPopulate(); // -> initialize and "bypass" layout presets system if required

		//--

		//if (bDockingReset)
		//{
		//	bDockingReset = false;
		//	dockingReset();
		//}

		//--

		//if (ui.bGui_Menu) draw_ImGui_Menu();
	}
	ui.EndDocking();
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::draw_ImGui()
{
	if (!bGui) return;

	//--

	ui.Begin();
	{
		// Docking
		draw_ImGui_Docking();

		//----

		if (bGui_Main)
		{
			// Main
			draw_ImGui_Main();

			// Timers
			if (bGui_SkipTimers) draw_ImGui_SkipTimers();
		}

		//--

		// Subtitles
#ifdef USE_ofxSurfingTextSubtitle__VIDEO_SKIP
		subs.drawImGui();
#endif

		//--

		// Floating Preview / Fbo image
		draw_ImGui_Preview();

		//--

		// Osc
#ifdef USE_ofxSurfingOsc
		oscHelper.drawImGui();
#endif
		//--
	}
	ui.End();

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
#ifdef USE_ofxSurfingFxPro
	fxPro.drawGui();
#endif
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::doOpenDialogToSetPath()
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "Set path";

	// Open the Open File Dialog
	std::string str = "Select a video file. Must be encoded in .mov HAP codec! \n";
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

	// Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		//processOpenFileSelection(openFileResult);

		ofFile file(openFileResult.getPath());
		videoName = file.getBaseName();
		videoFilePath = file.getAbsolutePath();

		/*
		videoName = openFileResult.getName();
		videoFilePath = openFileResult.getPath();
		*/

		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "video name: " << videoName;
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "video path: " << videoFilePath;

		loadMovie(videoFilePath);
	}
	else
	{
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "User hit cancel";
	}
}

//TODO: WIP. not working as expected...
// image looks green and position seems not handled.
//--------------------------------------------------------------
void ofxSurfingVideoSkip::doGenerateThumbs()
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__);

	bFboReady = false;

	//--

	// A. FFMPEG script
	// https://ottverse.com/thumbnails-screenshots-using-ffmpeg/
	// create the command
	std::string someCmd;

	// OF_APP/bin/data/thumbs

	// remove if exists
	auto pathData = ofToDataPath("thumbs/", true);
	someCmd = "rmdir /s /q " + pathData;
	doRunCommand(someCmd);

	// create dir
	someCmd = "mkdir data\\thumbs";
	doRunCommand(someCmd);

	//--

	// video aspect ratio
	float _w = player.getWidth();
	float _h = player.getHeight();
	float _ratio = _w / _h;

	// thumb size
	float h = BarHeight;
	float w = BarHeight * _ratio;

	// amount of thumbs to fill the bar
	int _amount = getBarRectangle().getWidth() / w;
	//_amount -= 1;
	cout << "amount of thumbs: " << _amount << endl << endl;

	//--

	// build the command
	someCmd = "ffmpeg -i ";
	someCmd += videoFilePath.get() + " ";//source video

	// size
	//someCmd += "-s 320x240 ";
	someCmd += "-s " + ofToString(int(w)) + "x" + ofToString(int(h)) + " ";

	someCmd += "-vf ";
	someCmd += "\"select='not(mod(n,";

	// Amount of thumbs

	// A
	// one still per second at 30 fps
	//someCmd << "30";
	//int _fps = player.getTotalNumFrames() / player.getDuration();
	//someCmd << ofToString(_fps);

	// B
	int _amountMod = player.getTotalNumFrames() / _amount;
	//int _amountMod = (player.getTotalNumFrames()) / (_amount+1);
	//int _amountMod = (player.getTotalNumFrames()) / (_amount-1);

	//if(!bFit) _amountMod -= 1;
	_amountMod -= 1;
	//_amountMod += 1;

	//if (bFit) {
	//	float widthExpect = getBarRectangle().getWidth() - (_amount * w);
	//	thumbGap = widthExpect / _amount;
	//}

	cout << "num frames: " << player.getTotalNumFrames() << endl << endl;
	cout << "amount mod: " << _amountMod << endl << endl;

	someCmd += ofToString(_amountMod);
	someCmd += "))',setpts='N/(30*TB)'\" ";
	someCmd += "-f image2 ";
	someCmd += pathData + "thumb_%02d.jpg";//files

	//--

	// Log & Run!

	// a. Blocking
	//doRunCommand(someCmd);

	// b. Using threading
	commandThread.set(someCmd);
	if (!commandThread.isThreadRunning())
	{
		commandThread.startThread();
		ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "Starting Thread!";
	}
	else
	{
		ofLogError("ofxSurfingVideoSkip") << (__FUNCTION__) << "Thread already running! Wait to work is done!";
	}

	//--

	/*
#ifdef TARGET_WIN32
	someCmd << "taskkill /F /IM ffmpeg.exe";//TODO: force
	cout << someCmd;
	cout << ofSystem(someCmd.str().c_str()) << endl;
#endif
#ifdef TARGET_OSX
	someCmd << "say Hello";
#endif
	cout << "> DONE !" << endl;
#endif
	*/

	//--

	// B. Internal
	// extracting thumbs using ofxHapPlayer API
	/*
	//player.stop();
	player.play();

	ofImageType type = OF_IMAGE_COLOR;
	//ofImageType type = OF_IMAGE_COLOR_ALPHA;

	int n = 10;
	for (size_t i = 0; i < n; i++)
	{
		float p = i * (1.0f / n);
		cout << p << ", ";
		player.stop();
		player.setPosition(p);

		////((ofApp*)ofGetAppPtr())->update();
		//ofGetAppPtr()->update();
		//ofGetAppPtr()->draw();

		player.play();
		player.update();

		ofTexture* t = player.getTexture();
		int w = t->getWidth();
		int h = t->getHeight();

		ofFbo fbo;
		fbo.allocate(w, h);
		fbo.begin();
		ofClear(0);
		player.draw(0, 0);
		fbo.end();

		ofPixels pix;
		pix.allocate(w, h, type);

		fbo.readToPixels(pix);
		//t->readToPixels(pix);

		ofImage img;
		img.allocate(w, h, type);
		img.setFromPixels(pix);
		img.update();

		////img.setFromPixels(player.getPixels());
		//float thumbWidth = 120;//change this for another thumb size
		//img.resize(thumbWidth, thumbWidth * (img.getHeight() / img.getWidth()));

		string _ni = ofToString(i);
		string _path = _root + _ni + ".jpg";

		img.saveImage(_path);
	}
	*/

	//--

	// a. Blocking
	// Load thumbs to images
	//loadThumbs();
}

//--

#ifdef USE_OF_BEAT_CLOCK__VIDEO_SKIP

//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_BeatTick() // callback to receive BeatTicks
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << "Beat #" << beatClock.getBeat();

#ifdef USE_ofxSurfingMoods
	moods.doBeatTick();
#endif

}

////--------------------------------------------------------------
//void ofxSurfingVideoSkip::Changed_BeatBpm()
//{
//	ofLogNotice("ofxSurfingVideoSkip")<<(__FUNCTION__) << "BPM " << beatClock.getBpm();
//
//	//bpm.set(beatClock.getBpm());
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
void ofxSurfingVideoSkip::draw_ImGui_Menu()
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
	ofAddListener(ofEvents().mouseScrolled, this, &ofxSurfingVideoSkip::mouseScrolled);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::removeMouseListeners()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxSurfingVideoSkip::mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxSurfingVideoSkip::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxSurfingVideoSkip::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxSurfingVideoSkip::mouseReleased);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxSurfingVideoSkip::mouseScrolled);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingVideoSkip::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxSurfingVideoSkip::keyReleased);
}

//--------------------------------------------------------------
void ofxSurfingVideoSkip::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingVideoSkip::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxSurfingVideoSkip::keyReleased);
}


//--------------------------------------------------------------
void ofxSurfingVideoSkip::Changed_VideoBarControl(bool& b)
{
	ofLogNotice("ofxSurfingVideoSkip") << (__FUNCTION__) << ofToString(b ? "TRUE" : "FALSE");

	static bool bGui_BarControl_PRE = !bGui_BarControl.get();
	if (bGui_BarControl.get() != bGui_BarControl_PRE)
	{
		bGui_BarControl_PRE = bGui_BarControl.get();
	}
	//else return;

	if (!bGui_BarControl.get())
	{
		ofHideCursor();
	}
	else
	{
		ofShowCursor();
	}
}