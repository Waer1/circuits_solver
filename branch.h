#pragma once
#include <string>
#include "Node.h"


class branch
{
	Node* endNode;
	Node* startNode;
	string name;

public:
	branch(Node* endv, Node* startv);
	Node* getendNode();
	Node* getstartNode();
	string getname();

};
