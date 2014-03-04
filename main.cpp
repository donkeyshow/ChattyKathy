#include <iostream>
#include <string>
#include "SocketUDPComm.h"
#include "LinkedList.h"
#include "Message.h"

using namespace std;

void	RcvdDataCallBack(const string data, const string remIPAddr);
int		StartTwoWayComm();
int		GetColor();
char	DerpMenu(); 
void	AddNode(string message);
void	ClearData(Node* headNode);
void	WashConsole();

LinkedList messageList;

const int IPADDR_BUFFER_SIZE = 16;
const int SEND_BUFFER_SIZE = 1024;
int g_remoteSendPort = 55551;

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
				int colorChoice = GetColor();
				
				switch (colorChoice)
				{
				case 1:
					::system("color 2");
					break;
				case 2:
					::system("color 3");
					break;
				case 3:
					::system("color 4");
					break;
				case 4:
					::system("color 5");
					break;
				case 5:
					::system("color 6");
					break;
				case 6:
					::system("color 8");
					break;
				case 7:
					::system("color 9");
					break;
				default:
					cout << "Not a valid option. defaulting to blue." << endl;
					colorChoice = 0;
					::system("color 9");
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
	char sendBuff[SEND_BUFFER_SIZE];
	CSocketUDPComm senderSocket;
	int retVal = 0;

	try
	{
		// Prompt the user for an IP connection to which to connect
		cout << "Enter an IP address of the machine to which to connect: ";
		cin >> ipAddr;

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

		int cnt = 1;
		while (!senderSocket.ShallTerminateNow())
		{
			sprintf_s(sendBuff, SEND_BUFFER_SIZE, "%s %d", "Hello", cnt++);
			senderSocket.SendData(sendBuff);
			Sleep(1000);
		}
	}
	catch (...)
	{
		senderSocket.SetTerminateNow(true);
		cout << "Error: Sender loop ending!" << endl;
		retVal = -1;
	}

	return retVal;
}

void RcvdDataCallBack(const string data, const string remIPAddr)
{
	// implement device to check _consoleBuffer for full
	// if it is full, wash the console, then add a new node with the current data
	// if not full, add node only
	// Store received data in linked list
	AddNode(data);

	// Print to the screen what was received
	cout << endl << data;
}

void AddNode(string message)
{
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
	string currentData = ((Message*)tailNode->_data).GetMessage();
	
	ClearData(headNode);

	Message* saveMe = new Message;
	saveMe->SetMessage(currentData);
	messageList.AddLinkToBack(saveMe);
	saveMe->AppendToLog();
	++Message::_consoleBuffer;

	::system("cls");

	saveMe->PrintToConsole();
}

int GetColor()
{
	int choice;

	cout << "1: Green" << endl
		<< "2: Aqua" << endl
		<< "3: Red" << endl
		<< "4: Purple" << endl
		<< "5: Yellow" << endl
		<< "6: Gray" << endl
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

	cout << endl;

	return choice;

}