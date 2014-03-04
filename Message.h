#ifndef _MESSAGE_H_02182014
#define _MESSAGE_H_02182014

#include <string>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class	Message
{
private:
	string	_message;

public:
	static string _logFilePath;
	static int _consoleBuffer;

	Message();
	~Message();
	const string GetMessage();
	void	SetMessage(string message);
	void	AppendToLog();
	void	PrintToConsole();
};

#endif  // !_SONG_H_02182014
