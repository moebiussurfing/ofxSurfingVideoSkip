#pragma once

class CommandThread : public ofThread
{
public:

	bool bDone;
	string s = "";

	void set(string _s) {
		ofLogNotice("commandThread") << (__FUNCTION__) << s;
		s = _s;
		bDone = false;
	};

	void doRunCommand(string _s) {
		ofLogNotice("commandThread") << (__FUNCTION__) << _s;
		cout << (_s) << endl << endl;
		cout << ofSystem(_s) << endl;
	};

	void CommandThread::threadedFunction() {
		ofLogNotice("commandThread") << (__FUNCTION__);

		if (s != "") doRunCommand(s);

		bDone = true;
		ofLogNotice("commandThread") << (__FUNCTION__) << "Done Run command!";

		this->stopThread();
	};

	bool isDone() {
		//ofLogNotice("commandThread") << (__FUNCTION__) << bDone;
		if (bDone) {
			bDone = false;
			return true;
		}
		else return false;
	};
};