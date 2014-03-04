#include "LinkedList.h"

LinkedList::LinkedList()
{
	_head = _tail = nullptr;
	_listLen = 0;
}

void LinkedList::AddLinkToBack(void* ptr) // takes a pointer to anything
{

	// create a new node
	Node* newNode = new Node;

	// set the data to the pointer parameter
	newNode->_data = ptr;

	if(_head == nullptr) // if no nodes exist, both head and tail are new node
		_head = _tail = newNode;
	else
	{
		newNode->_prev = _tail; // link back to the previous node
		_tail->_next = newNode; // the current tail pointer now points to the new node memory location -- creates the link from the old to the new 
		_tail = newNode; // tail is now the new node
	}

	++_listLen;
}

void* LinkedList::RemoveLinkFromFront() // returns a pointer to the data
{
	if (_head == nullptr) // if list is empty
		return _head;

	Node* temp = _head; // save the location of the first node
	void* oldNodeData = _head->_data; // save the data that is in our head node

	if(_head == _tail) // if its the only node
		_head = _tail = nullptr; // everything is null
	else
	{	
		_head = _head->_next;
		_head->_prev = nullptr;
	}

	delete temp; // clear the memory of the first node
	--_listLen;

	return oldNodeData; // return old first object's data for deletion

}

Node* LinkedList::GetFirstNode()
{
	return _head;
}

Node* LinkedList::GetLastNode()
{
	return _tail;
}

long LinkedList::GetListLength()
{
	return _listLen;
}

Node* Node::RemoveLinks() // removes node at this position
{
	Node* beforeCurrent = this->_prev;
	Node* afterCurrent = this->_next;

	if (afterCurrent != nullptr) // if we arent the last
	{
		afterCurrent->_prev = beforeCurrent; // point after current to before current
	}

	if(beforeCurrent != nullptr) // if we arent the first
	{
		beforeCurrent->_next = afterCurrent; // point before current to after current
	}

	this->_prev = _next = nullptr; // remove current node links

	return this; // returns node to be deleted
}

LinkedList::~LinkedList()
{

}
