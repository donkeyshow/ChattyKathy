#ifndef _MESSAGE_H_02242014
#define _MESSAGE_H_02242014

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <time.h> // time_t, tm, localtime
#include <iomanip> // time function ( put_time )
#include <chrono> // system_clock

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
	const	string GetChatMessage();
	void	SetMessage(string message);
	void	AppendToLog();
	void	PrintToConsole();
};

#endif  // !_MESSAGE_H_02242014
