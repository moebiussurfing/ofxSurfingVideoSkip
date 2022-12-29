
/*

	TODO

	add split independent mode.
		player with loop toggle.
		not linked to video
	make video loader/dialog runtime
	add non hap loader too
	add vector of pointer players
	integrate with ofxSoundObjects


*/


//--

#pragma once

#define USE_SOUND_PLAYER_STAND_ALONE

#include "ofMain.h"

#include "ofxHapPlayer.h"
#include "ofxSurfingHelpers.h"

class PlayerAV
{
public:

	PlayerAV() {
	};

	~PlayerAV() {
		exit();
	};

	//--

private:

	string path_GLOBAL = "PlayerAV";
	string path_AppSettings = "Settings_PlayerAV.json";

	// Video Player
	// Hap 
	ofxHapPlayer player;

	// Audio
	ofSoundStreamSettings outSettings;
	ofSoundStream outStream;
	std::vector<ofSoundDevice> outDevices;

	//--

public:

	ofParameterGroup params{ "PLAYER" };
	ofParameterGroup params_Video{ "VIDEO" };

	ofParameter<string> path_Video{ "Path Video", "" };
	ofParameter<bool> bLoop{ "Loop", false };
	ofParameter<float> position{ "Position", 0, 0, 1 };

#ifdef USE_SOUND_PLAYER_STAND_ALONE
	ofParameterGroup params_Audio{ "AUDIO" };
	ofParameter<string> path_Audio{ "Path Audio", "" };
	ofParameter<float> volumeAudio{ "Volume Audio", 0.5, 0, 1 };
	ofParameter<bool> bLinkAudio{ "Link Audio", false };
	ofParameter<bool> bPlayAudio{ "Play Audio", false };
	ofParameter<void> bStopAudio{ "Stop Audio" };
	ofParameter<bool> bLoopAudio{ "Loop Audio", false };
	ofParameter<float> positionAudio{ "Position Audio", 0, 0, 1 };
	string name_Audio = "NO FILE";
#endif

private:

	string name_Video = "NO FILE";

	ofParameter<string> path_Srt{ "PathSrt", "" };

public:

	void setup()
	{
		setup_Video();
		setup_Audio();

		ofAddListener(params.parameterChangedE(), this, &PlayerAV::Changed_Params);

		startup();
	};

private:

	void startup() {
		ofxSurfingHelpers::loadGroup(params, path_GLOBAL + "/" + path_AppSettings);
	};

	void setup_Video()
	{
		params_Video.add(path_Video);
		params_Video.add(bLoop);
		params_Video.add(position);

		position.setSerializable(false);

		params.add(params_Video);
	};

	void setup_Audio()
	{
#ifdef USE_SOUND_PLAYER_STAND_ALONE
		params_Audio.add(path_Audio);
		params_Audio.add(bLinkAudio);
		params_Audio.add(bPlayAudio);
		params_Audio.add(bStopAudio);
		params_Audio.add(bLoopAudio);
		params_Audio.add(positionAudio);
		params_Audio.add(volumeAudio);

		params.add(params_Audio);

		// to starts at zero always!
		positionAudio.setSerializable(false);
#endif
		//--

		// hardcoded device
		int deviceOut_Port = 0;

		outDevices.clear();
		outDevices = outStream.getDeviceList(ofSoundDevice::Api::MS_DS);
		ofLogNotice("PlayerAV") << outDevices;

		size_t sampleRate = 48000;
		size_t bufferSize = 256;
		size_t numBuffers = 4;
		size_t numInputChannels = 0;
		size_t numOutputChannels = 2;

		outSettings.bufferSize = bufferSize;
		outSettings.numBuffers = numBuffers;
		outSettings.sampleRate = sampleRate;
		outSettings.numInputChannels = numInputChannels;
		outSettings.numOutputChannels = numOutputChannels;
		outSettings.setOutListener(ofGetAppPtr());
		outSettings.setOutDevice(outDevices[deviceOut_Port]);

		outStream.setup(outSettings);
	};

public:

	void update()
	{
		if (position != player.getPosition()) position.setWithoutEventNotifications(player.getPosition());
		if (positionAudio != playerAudio.getPosition()) positionAudio.setWithoutEventNotifications(playerAudio.getPosition());

		player.update();//?
		ofSoundUpdate();
	};

	void draw(float x, float y) {
		player.draw(x, y);
	};

	void draw(float x, float y, float w, float h) {
		player.draw(x, y, w, h);
	};

	void draw(ofRectangle r) {
		player.draw(r.getX(), r.getY(), r.getWidth(), r.getHeight());
	};

private:

	void exit() {
		outStream.close();

		ofRemoveListener(params.parameterChangedE(), this, &PlayerAV::Changed_Params);

		ofxSurfingHelpers::CheckFolder(path_GLOBAL);
		ofxSurfingHelpers::saveGroup(params, path_GLOBAL + "/" + path_AppSettings);
	};

	//--

public:

	// Setters

	void setPaused(bool b) {
		player.setPaused(b);

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		if (bLinkAudio) playerAudio.setPaused(b);
#endif
	};
	void setPosition(float pos) {
		player.setPosition(pos);

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		if (bLinkAudio) playerAudio.setPosition(pos);
#endif
	};
	void setVolume(float vol) {
		//player.setVolume(vol);

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		playerAudio.setVolume(vol);
#endif
	};
	void setSpeed(float speed) {
		player.setSpeed(speed);

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		if (bLinkAudio) playerAudio.setSpeed(speed);
#endif
	};
	void setLoopState(ofLoopType state) {
		player.setLoopState(state);

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		if (bLinkAudio) playerAudio.setLoop(state);
#endif
	};

	//--

	// Getters

	float getWidth() const {
		return player.getWidth();
	};
	float getHeight() const {
		return player.getHeight();
	};
	bool isPlaying() const {
		return player.isPlaying();
	};
	bool isPaused() const {
		return player.isPaused();
	};
	bool isMovieDone() const {
		return player.getIsMovieDone();
	};
	int	getTotalNumFrames() const {
		return player.getTotalNumFrames();
	};
	float getPosition() const {
		return player.getPosition();
	};
	float getDuration() const {
		return player.getDuration();
	};
	bool isLoaded() const {
		return player.isLoaded();
	};
	string getPath() const {
		if (isLoaded()) return path_Video.get();
		else return string("NO FILE");
	};
	string getPathSrt() const {
		if (isLoaded()) return path_Srt.get();
		else return string("NO FILE");
	};
	bool isError() const {
		return (player.getError().length());
	};

#ifdef USE_SOUND_PLAYER_STAND_ALONE
	float getPositionAudio() const {
		return playerAudio.getPosition();
	};
	int getPositionAudioMS() const {
		return playerAudio.getPositionMS();
	};
	bool isLoadedAudio() const {
		return playerAudio.isLoaded();
	};
	//float getDurationAudio() const {
	//	return playerAudio.getPosition();//?
	//};
	string getPathAudio() const {
		if (isLoadedAudio()) return path_Audio.get();
		else return string("NO FILE");
	};
#endif

	//--

	// Audio

#ifdef USE_SOUND_PLAYER_STAND_ALONE

	void setVolumenUp()
	{
		volumeAudio = MIN(volumeAudio + 0.1, 1);
	};

	void setVolumenDown()
	{
		volumeAudio = MAX(volumeAudio - 0.1, 0);
	};
#endif

	//--

private:

	void Changed_Params(ofAbstractParameter& e) // patch change
	{
		std::string name = e.getName();

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		if (name == path_Audio.getName())
		{
			if (!bLinkAudio)
			{
				this->loadAudio(path_Audio);
			}
		}
		if (name == volumeAudio.getName())
		{
			volumeAudio.setWithoutEventNotifications(ofClamp(volumeAudio, 0, 1));
			player.setVolume(volumeAudio.get());
			if (!bLinkAudio)
				playerAudio.setVolume(volumeAudio.get());
		}
		if (name == positionAudio.getName())
		{
			if (!bLinkAudio)
				playerAudio.setPosition(positionAudio);
		}
		if (name == bLoop.getName())
		{
			playerAudio.setLoop(bLoop);
		}
		if (name == bLinkAudio.getName())
		{
			if (bLinkAudio) path_Audio.setSerializable(false);
			else path_Audio.setSerializable(true);
		}
		if (name == bPlayAudio.getName())
		{
			if (!bLinkAudio) {
				if (bPlayAudio) {
					if (playerAudio.isPlaying()) playerAudio.setPaused(false);
					else playerAudio.play();
				}
				else {
					if (playerAudio.isPlaying()) playerAudio.setPaused(true);
					else playerAudio.stop();
				}
			}
		}
		if (name == bStopAudio.getName())
		{
			bPlayAudio.setWithoutEventNotifications(false);
			playerAudio.stop();
		}
#endif
	};

public:

	void doOpenDialogPathAudio()
	{
		ofLogNotice("PlayerAV") << (__FUNCTION__) << "Set audio path";

		// Open the Open File Dialog
		std::string str = "Select an audio file.";
		ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

		// Check if the user opened a file
		if (openFileResult.bSuccess)
		{
			//processOpenFileSelection(openFileResult);

			ofFile file(openFileResult.getPath());
			name_Audio = file.getBaseName();
			path_Audio = file.getAbsolutePath();

			ofLogNotice("PlayerAV") << (__FUNCTION__) << "Name: " << name_Audio;
			ofLogNotice("PlayerAV") << (__FUNCTION__) << "Path: " << path_Audio;
			loadAudio(path_Audio);
		}
		else
		{
			ofLogNotice("PlayerAV") << (__FUNCTION__) << "User hit cancel";
		}
	};

	void doOpenDialogPathVideo()
	{
		ofLogNotice("PlayerAV") << (__FUNCTION__) << "Set video path";

		// Open the Open File Dialog
		std::string str = "Select a video file. Must be encoded in .mov HAP codec! \n";
		ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

		// Check if the user opened a file
		if (openFileResult.bSuccess)
		{
			//processOpenFileSelection(openFileResult);

			ofFile file(openFileResult.getPath());
			name_Video = file.getBaseName();
			path_Video = file.getAbsolutePath();

			ofLogNotice("PlayerAV") << (__FUNCTION__) << "Name: " << name_Video;
			ofLogNotice("PlayerAV") << (__FUNCTION__) << "Path: " << path_Video;
			load(path_Video);
		}
		else
		{
			ofLogNotice("PlayerAV") << (__FUNCTION__) << "User hit cancel";
		}
	};

public:

	// bLinkFiles true to load audio and srt files too! must have same name
	//bool load(string path, bool bLinkFiles = false)
	bool load(string path)
	{
		bool bLinkFiles = bLinkAudio;

		path_Video = path;
		bool b = player.load(path_Video);
		if (b) ofLogNotice("PlayerAV") << "Success: loaded Video File " << path_Video;
		else ofLogError("PlayerAV") << "Error: Video File not found" << path_Video;

		if (b)
		{
			// link both players A/V and prepare srt file path too to be getted after!
			if (bLinkFiles)
			{
				ofFile file(path_Video.get());

				// construct audio path
				path_Audio = file.getEnclosingDirectory() + file.getBaseName() + ".wav";
				ofLogNotice("PlayerAV") << "Auto .wav File " << path_Audio;
				this->loadAudio(path_Audio);

				//this->setPosition(positionAudio);

				// construct srt path
				path_Srt = file.getBaseName() + ".srt";
				path_Srt = file.getEnclosingDirectory() + path_Srt.get();
				ofLogNotice("PlayerAV") << "Auto .srt File " << path_Srt;
			}
		}

		return b;
	};

	//--

	// Audio

#ifdef USE_SOUND_PLAYER_STAND_ALONE

private:

	ofSoundPlayer playerAudio;

public:

	bool loadAudio(std::string path) {
		bool b = playerAudio.load(path);

		if (b) ofLogNotice("PlayerAV") << "Success: Loaded Audio file " << path;
		else ofLogError("PlayerAV") << "Error: Audio File not found " << path;

		playerAudio.setVolume(volumeAudio);
		playerAudio.setMultiPlay(false);
		playerAudio.setLoop(bLoopAudio);

		//workflow
		//auto play
		playerAudio.play();
		if (bLinkAudio) playerAudio.setPosition(this->getPosition());//link both players

		// labels
		ofFile file(path_Audio.get());
		name_Audio = file.getBaseName();
		//path_Audio = file.getAbsolutePath();

		return b;
	};
#endif

};
