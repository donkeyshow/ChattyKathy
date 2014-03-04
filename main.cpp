#include <iostream>
#include <string>
#include "SocketUDPComm.h"
#include "LinkedList.h"
#include "Message.h"
#include <fstream>
#include <ctime>

using namespace std;

void	RcvdDataCallBack(const char* data, const char* remIPAddr);
int	StartTwoWayComm();
char	GetColor();
char	DerpMenu(); 
void	AddNode(string message);
void	ClearData(Node* headNode);
void	WashConsole();

LinkedList messageList;

const int IPADDR_BUFFER_SIZE = 16;
const int SEND_BUFFER_SIZE = 1024;
int g_remoteSendPort = 55551;

HANDLE g_consoleHndl; //console window handle
COORD pos1 = {1, 22};
COORD pos2 = {12, 12};
string userColor = "color 9";
strintg defaultColor = "color";

void main()
{
	///////////////
	//This creates a log file in the directory of the executable with the current date
	//and writes "chat started: current date and time" to it
	///////////////
	string tempFilePath;
    ifstream inFile;
    ofstream outFile;
    ofstream logFile;
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    system("echo %cd%\\chatLog> filePath.txt");
    inFile.open("filePath.txt");
    getline(inFile,tempFilePath);
    inFile.close();
    outFile.open("filePath.txt", fstream::out);
    outFile << tempFilePath << now->tm_mday
    << (now->tm_mon + 1)
    << (now->tm_year + 1900) << ".txt" << endl;
    outFile.close();
    inFile.open("filePath.txt");
    getline(inFile,Message::_logFilePath);
    inFile.close();
    system("del filePath.txt");
    string makeFile = "echo. > ";
    makeFile.append(Message::_logFilePath);
    system(makeFile.c_str());
    logFile.open(Message::_logFilePath.c_str(), fstream::out);
    logFile << "Chat started: "
    << now->tm_mday << "/"
    << (now->tm_mon + 1)
    << "/" << (now->tm_year + 1900) << " "
    << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec
    << endl;
    logFile.close();


    g_consoleHndl = GetStdHandle(STD_OUTPUT_HANDLE); //gets handle on window for UI changes
    

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
					//::system("color A");
					break;
				case '2':
					userColor = "color B";
					//::system("color B");
					break;
				case '3':
					userColor = "color C";
					//::system("color C");
					break;
				case '4':
					userColor = "color D";
					//::system("color D");
					break;
				case '5':
					userColor = "color E";
					//::system("color E");
					break;
				case '6':
					userColor = "color F";
					//::system("color F");
					break;
				case '7':
					userColor = "color 9";
					//::system("color 9");
					break;
				default:
					cout << "Not a valid option. defaulting to blue." << endl;
					colorChoice = 0;
					//this is clobally set regardless
					//::system("color 9");
					break;
				}
					// Uncomment one only and set the correct IP addr for sender
					// once we get user input, clear the screen to get full message buffer potential
					::system("cls");
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
	//char sendBuff[SEND_BUFFER_SIZE];
	CSocketUDPComm senderSocket;
	int retVal = 0;
	string textToSend;
	char temp[SEND_BUFFER_SIZE];

	try
	{
		// Prompt the user for an IP connection to which to connect
		cout << "Enter an IP address of the machine to which to connect: ";
		cin >> ipAddr;
		::system("cls");

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
			textToSend.clear();

			system(userColor.c_str()); // set our color to globally defined user color

			SetConsoleCursorPosition(g_consoleHndl, pos1); //set our position to bottom left

			getline(cin, textToSend);

			strcpy_s(temp, SEND_BUFFER_SIZE, textToSend.c_str());
			
			if(temp[0] == 0)
				break;
			
			AddNode(textToSend);
			
			senderSocket.SendData(temp);
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
	// implement device to check _consoleBuffer for full
	// if it is full, wash the console, then add a new node with the current data
	// if not full, add node only
	// Store received data in linked list
	AddNode(data);

	system(defaultColor.c_str()); // set their color to system defined default color

	SetConsoleCursorPosition(g_consoleHndl, pos2); //set their position to middle right

	// Print to the screen what was received
	cout << data << endl;
}

void AddNode(string message)
{
	if (Message::_consoleBuffer >= 24)
		WashConsole();

	Message* currentMessage = new Message;
	currentMessage->SetMessage(message);
	messageList.AddLinkToBack(currentMessage);
	currentMessage->AppendToLog();
	++Message::_consoleBuffer;
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
	Node* headNode = messageList.GetFirstNode();
	Node* tailNode = messageList.GetLastNode();
	string currentData = ((Message*)tailNode->_data)->GetChatMessage();
	
	ClearData(headNode);

	Message* saveMe = new Message;
	saveMe->SetMessage(currentData);
	messageList.AddLinkToBack(saveMe);
	++Message::_consoleBuffer;

	::system("cls");

	saveMe->PrintToConsole();
}

char GetColor()
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

char DerpMenu()
{
	char choice;

	cout << "a) Start a chat" << endl
		<< "b) Quit" << endl;

	cin >> choice;

	//cout << endl;

	return choice;
}
