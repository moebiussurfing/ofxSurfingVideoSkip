
/*

	TODO

	make video loader/dialog runtime
	add non hap loader too
	integrate with ofxSoundObjects


*/


//--

#pragma once

#define USE_SOUND_PLAYER_STAND_ALONE

#include "ofMain.h"
#include "ofxHapPlayer.h"

class PlayerAV
{
public:

	PlayerAV() {
	};

	~PlayerAV() {
		exit();
	};

	//--

	// Video Player
	// Hap 
	ofxHapPlayer player;

	// Audio
	ofSoundStreamSettings outSettings;
	ofSoundStream outStream;
	std::vector<ofSoundDevice> outDevices;

	//--

	// Audio
#ifdef USE_SOUND_PLAYER_STAND_ALONE

	ofSoundPlayer playerAudio;

	bool loadAudio(std::string path) {
		bool b = playerAudio.load(path);

		if (b) ofLogNotice("PlayerAV") << "SUCCESS Loaded Audio " << path;
		else ofLogError("PlayerAV") << "Audio File not found " << path;

		playerAudio.setVolume(volume);
		playerAudio.setMultiPlay(false);

		//workflow
		//auto play
		playerAudio.play();
		playerAudio.setPosition(this->getPosition());//link both players

		return b;
	};

	ofParameterGroup params{ "PLAYER" };
	ofParameterGroup params_Video{ "VIDEO" };
	ofParameterGroup params_Audio{ "AUDIO" };
	ofParameter<float> volume{ "Volume", 0.5, 0, 1 };
	ofParameter<float> position{ "Position", 0, 0, 1 };
	ofParameter<float> positionAudio{ "Position Audio", 0, 0, 1 };
	ofParameter<string> path_Audio{ "PathAudio", "" };
	ofParameter<string> path_Video{ "PathVideo", "" };
	string name_Audio = "NO FILE";
	string name_Video = "NO FILE";
	ofParameter<string> path_Srt{ "PathSrt", "" };

#endif

	void setup() {
		setup_Video();
		setup_Audio();

		ofAddListener(params.parameterChangedE(), this, &PlayerAV::Changed_Params);
	};

	void setup_Video()
	{
		params_Video.add(path_Video);
		params_Video.add(position);
		params.add(params_Video);
	}

	void setup_Audio()
	{
#ifdef USE_SOUND_PLAYER_STAND_ALONE
		params_Audio.add(path_Audio);
		params_Audio.add(volume);
		params_Audio.add(positionAudio);
		playerAudio.setVolume(volume);
		params.add(params_Audio);
#endif
		//--

		//hardcoded device
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

	// bLinkFiles true to load audio and srt files too! must have same name
	bool load(string path, bool bLinkFiles = false)
	{
		path_Video = path;
		bool b = player.load(path_Video);
		if (b) ofLogNotice("PlayerAV") << "SUCCESS Loading Video File " << path_Video;
		else ofLogError("PlayerAV") << "Video File not found" << path_Video;

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

	void update() {

		if (position != player.getPosition()) position.setWithoutEventNotifications(player.getPosition());

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

	void exit() {
		outStream.close();

		ofRemoveListener(params.parameterChangedE(), this, &PlayerAV::Changed_Params);
	};

	// setters
	void setPaused(bool b) {
		player.setPaused(b);
		playerAudio.setPaused(b);
	};
	void setPosition(float pos) {
		player.setPosition(pos);
		playerAudio.setPosition(pos);
	};
	void setVolume(float vol) {
		//player.setVolume(vol);
		playerAudio.setVolume(vol);
	};
	void setSpeed(float speed) {
		player.setSpeed(speed);
		playerAudio.setSpeed(speed);
	};
	void setLoopState(ofLoopType state) {
		player.setLoopState(state);
		playerAudio.setLoop(state);
	};

	// getters
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
	bool isLoadedAudio() const {
		return playerAudio.isLoaded();
	};
	string getPath() const {
		if (isLoaded()) return path_Video.get();
		else return string("NO FILE");
	}
	string getPathAudio() const {
		if (isLoadedAudio()) return path_Audio.get();
		else return string("NO FILE");
	}
	string getPathSrt() const {
		if (isLoaded()) return path_Srt.get();
		else return string("NO FILE");
	}
	bool isError() const {
		return (player.getError().length());
	};

	//--

	// Audio

	void setVolumenUp()
	{
		volume = MIN(volume + 0.1, 1);
	};

	void setVolumenDown()
	{
		volume = MAX(volume - 0.1, 0);
	};

	//--

	void Changed_Params(ofAbstractParameter& e) // patch change
	{
		std::string name = e.getName();

		// volume
		if (name == volume.getName())
		{
			volume.setWithoutEventNotifications(ofClamp(volume, 0, 1));
			player.setVolume(volume.get());
			playerAudio.setVolume(volume.get());
		}
	};

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
	}

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
	}
private:

};
