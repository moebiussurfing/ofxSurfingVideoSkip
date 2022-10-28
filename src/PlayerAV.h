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

		if (b) ofLogNotice("PlayerAV") << "Loaded Audio " << path;
		else ofLogError("PlayerAV") << "Audio File not found " << path;

		playerAudio.setVolume(volume);
		playerAudio.setMultiPlay(false);
		playerAudio.play();
		return b;
	};

	ofParameterGroup params_Audio{ "AUDIO" };
	ofParameter<float> volume{ "Volume", 0.5, 0, 1 };
	ofParameter<float> positionAudio{ "Position", 0, 0, 1 };
	ofParameter <string> path_Audio{ "PathAudio", "" };

#endif

	void setup() {
		setup_Audio();
	};

	void setup_Audio()
	{
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

		//--

#ifdef USE_SOUND_PLAYER_STAND_ALONE
		params_Audio.add(volume);
		params_Audio.add(positionAudio);
		params_Audio.add(path_Audio);
		playerAudio.setVolume(volume);
#endif
	};

	bool load(string path) {
		bool b = player.load(path);
		if (b) ofLogNotice("PlayerAV") << "Loading Video File " << path;
		else ofLogError("PlayerAV") << "Video File not found" << path;
		return b;
	};

	void update() {
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

		ofRemoveListener(params_Audio.parameterChangedE(), this, &PlayerAV::Changed_Params);
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
	int	getTotalNumFrames() const {
		return player.getTotalNumFrames();
	};
	float getDuration() const {
		return player.getDuration();
	};
	bool isLoaded() const {
		return player.isLoaded();
	};
	bool isPlaying() const {
		return player.isPlaying();
	};
	bool isPaused() const {
		return player.isPaused();
	};
	float getPosition() const {
		return player.getPosition();
	};
	bool getIsMovieDone() const {
		return player.getIsMovieDone();
	};
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
		}
	};

private:

};
