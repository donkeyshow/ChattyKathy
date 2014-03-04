#include "Message.h"

string Message::_logFilePath = string();

Message::Message()
{

}

const string Message::GetMessage()
{
	return _message;
}

void Message::SetMessage( string message )
{
	_message = message;
}

void Message::AppendToLog()
{
	std::fstream fs; // create a file stream object

	fs.open(Message::_logFilePath, std::fstream::in | std::fstream::out | std::fstream::app); // open the shell file for read write and append

	fs << this->_message << endl; // output song path to file

	fs.close();
	fs.clear(std::ios_base::goodbit);
}

void Message::PrintToConsole()
{
	cout << this->_message << endl;
}

Message::~Message()
{

}
