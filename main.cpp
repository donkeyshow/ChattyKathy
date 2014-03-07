/*
In class group project:
name: Bryan Wales, Brit Cornwell, Casey Benson
date: 02/24/2014
assignment: 5
description: This program initializes a UDP socket to send chat messages back and forth between two IP addresses.  When a message is sent or received,
it is added to a linked list, and appended to a log file.
sources: see name, cplusplus.com
*/

#include <iostream>
#include <string>
#include "SocketUDPComm.h"
#include "LinkedList.h"
#include "Message.h"
#include <fstream>

using namespace std;

void	RcvdDataCallBack(const char* data, const char* remIPAddr);
int		StartTwoWayComm();
char	GetColor();
char	DerpMenu(); 
void	AddNode(string message);
void	ClearData(Node* headNode);
void	WashConsole();
bool	CreateLog();
string	ExePath();

LinkedList messageList;

const int IPADDR_BUFFER_SIZE = 16;
const int SEND_BUFFER_SIZE = 1024;
const int MAX_PATH_SIZE = 4096;
int g_remoteSendPort = 55551;
string userColor = "color 9";
string defaultColor = "color";

void main()
{
	// create linked list var to hold messages
	char menuChoice;

	do 
	{
		menuChoice = toupper(DerpMenu());

		switch (menuChoice)
		{
		case 'A':
			{
				char colorChoice = GetColor();

				switch (colorChoice)
				{
				case '1':
					userColor = "color A";
					break;
				case '2':
					userColor = "color B";
					break;
				case '3':
					userColor = "color C";
					break;
				case '4':
					userColor = "color D";
					break;
				case '5':
					userColor = "color E";
					break;
				case '6':
					userColor = "color F";
					break;
				case '7':
					userColor = "color 9";
					break;
				default:
					cout << "Not a valid option. defaulting to blue." << endl;
					colorChoice = 0;
					//this is globally set regardless
					//::system("color 9");
					break;
				}
				// Uncomment one only and set the correct IP addr for sender
				CreateLog();
				StartTwoWayComm();
			}
			break;
		case 'B':
			return;
			break;
		default:
			cout << "Not a valid option." << endl;
			break;
		}
	} while (menuChoice != 'B');	
}

int StartTwoWayComm()
{
	char ipAddr[IPADDR_BUFFER_SIZE];
	CSocketUDPComm senderSocket;
	int retVal = 0;
	string textToSend;
	char temp[SEND_BUFFER_SIZE];

	try
	{
		// Prompt the user for an IP connection to which to connect
		cout << "Enter an IP address of the machine to which to connect: ";
		cin >> ipAddr;
		::system("cls"); // clear the screen after the final input to have a fresh console window

		// Before sending anything, start a listening thread so can see what
		// the remote user is saying.
		senderSocket.StartListenerThread(g_remoteSendPort, RcvdDataCallBack);

		// Now let the custom socket class start the loop that allows the
		// user to send text to the remote system through the console and
		// read text sent from the remote user.
		if (senderSocket.InitializeSenderSocket(ipAddr, g_remoteSendPort) != 0)
		{
			// Can't work without this so bypass loop
			throw;
		}

		cin.ignore(INT_MAX,'\n');

		do 
		{
			textToSend.clear(); // clear the input string

			system(userColor.c_str()); // set our color to globally defined user color

			getline(cin, textToSend); // get user input

			strcpy_s(temp, SEND_BUFFER_SIZE, textToSend.c_str()); // convert to c string

			if(temp[0] == 0) // if empty string, terminate loop
				break;

			AddNode(textToSend); // add to linked list

			senderSocket.SendData(temp); // send the message to the other user
			//Sleep(1000);

		} while (!senderSocket.ShallTerminateNow());
	}
	catch (...)
	{
		senderSocket.SetTerminateNow(true);
		cout << "Error: Sender loop ending!" << endl;
		retVal = -1;
	}

	return retVal;
}

void RcvdDataCallBack(const char* data, const char* remIPAddr)
{
	AddNode(data); // add the received data to the linked list

	system(defaultColor.c_str()); // set their color to system defined default color
		
	cout << data << endl; // Print to the screen what was received
}

void AddNode(string message)
{
	if (Message::_consoleBuffer >= 24) // check to see if console is full
		WashConsole(); // if so, WashConsole

	Message* currentMessage = new Message; // create a pointer to a new message object
	currentMessage->SetMessage(message); // store the current text as the message object data
	messageList.AddLinkToBack(currentMessage); // add the message to the linked list
	currentMessage->AppendToLog(); // add the message to the log file
	++Message::_consoleBuffer; // increment message count
}

void ClearData( Node* headNode)
{
	if(headNode == nullptr)
		return;

	Message* pointedData = (Message*)headNode->_data; // save pointer to song data -- cast nodeToDelete->data as Song pointer
	Node* afterCurrent = headNode->_next; // save pointer to the next node

	delete pointedData; // delete data
	messageList.RemoveLinkFromFront(); // remove current node
	--Message::_consoleBuffer;

	if(afterCurrent != nullptr) // if we aren't at the end of the list
	{
		ClearData(afterCurrent); // recurse function -- this is tail recursion
	}
}

void WashConsole()
{
	Node* headNode = messageList.GetFirstNode(); // get first node and store it in a node pointer
	Node* tailNode = messageList.GetLastNode(); // get last node and store it in a node pointer
	string currentData = ((Message*)tailNode->_data)->GetChatMessage(); // get the data in the last node and store it in a string

	ClearData(headNode); // clear the linked list

	Message* saveMe = new Message; // create a pointer to a new message object
	saveMe->SetMessage(currentData); // store the saved text as message object data
	messageList.AddLinkToBack(saveMe); // add the message to the linked list
	++Message::_consoleBuffer; // increment message count

	::system("cls"); // clear the screen

	saveMe->PrintToConsole(); // output the message
}

char GetColor() // get the console color from the user
{
	char choice;

	cout << "1: Green" << endl
		<< "2: Aqua" << endl
		<< "3: Red" << endl
		<< "4: Purple" << endl
		<< "5: Yellow" << endl
		<< "6: White" << endl
		<< "7: Blue" << endl;

	cin >> choice;

	return choice;
}

char DerpMenu() // generic choice function
{
	char choice;

	cout << "a) Start a chat" << endl
		<< "b) Quit" << endl;

	cin >> choice;

	//cout << endl;

	return choice;
}

bool	CreateLog()
{
	ofstream outputFile; // make an outfile object
	
	Message::_logFilePath = ExePath();
	Message::_logFilePath.append("\\");
	Message::_logFilePath.append("chatlog.txt");

	outputFile.open(Message::_logFilePath, ios::out|ios::trunc); // this will create the file in the working directory
	
	if(outputFile.is_open()) // if file was opened
	{
		outputFile.flush(); // clear the file
		outputFile.close(); // close
		return true; // all is good
	}
	cout << " es no good buey " << std::endl;
	return false;
}

string ExePath()
{
	string tempString, filePath;
	char buffer[MAX_PATH_SIZE];

	GetModuleFileName(NULL, buffer, MAX_PATH_SIZE); // WINDOWS ONLY FUNCTION
	
	for (int i = 0; i < MAX_PATH_SIZE; ++i)
	{
		if(buffer[i] == NULL)
			break;

		tempString.push_back(buffer[i]);
	}
	
	filePath = tempString.substr(0, tempString.find_last_of("\\"));
	
	return filePath;
}
