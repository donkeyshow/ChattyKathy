#ifndef _LINKEDLIST_H_02032014_REVISED_02242014
#define _LINKEDLIST_H_02032014_REVISED_02242014

// this class creates a classic queue.  first in first out, adding to the back and removing from the front

struct	Node // define the node structure
{
	void*	_data; // declare a void pointer for data -- can point to any type of object rather than using a template
	Node*	_next; // declare the pointer to the next node
	Node*	_prev; // a link back to the previous node

	Node()
	{
		_data = nullptr; // set the values to null in the constructor
		_prev = nullptr;
		_next = nullptr;
	}

	Node* RemoveLinks(); // node function to remove links at specific node
};

class	LinkedList
{
private:
	Node*	_head; // members to store the head of the list, tail of the list and its length
	Node*	_tail;
	long	_listLen;

public:

	LinkedList();
	~LinkedList();
	void	AddLinkToBack(void* ptr); // method to add a node to the back of the list
	void*	RemoveLinkFromFront(); // method to remove a node from the front of the list.  returns a pointer to the data that was in the node
	Node*	GetFirstNode();
	Node*	GetLastNode();
	long	GetListLength();
};

#endif // !_LINKEDLIST_H_02032014_REVISED_02242014
