#include "Headers.hpp"
#include "Node.hpp"


Node::Node()
{
    //std::cout << "New Node created!" << '\n';
}


Node::Node(int key)
{
    this->key = key;
    //std::cout << "New Node created! With key : " << key <<  '\n';
}


Node::~Node()
{
    if (next != NULL)
	{
		delete next;
		next = NULL;
		//cout << "------->  Node deleted." << endl;
	}
	else
	{
		//cout << "------->  Node deleted." << endl;
	}
}


Node* Node::getNext()
{
    return next;
}


int Node::getKey()
{
    return key;
}


void Node::setNext(Node* next)
{
    this->next = next;
}


void Node::setKey(int key)
{
    this->key = key;
}
