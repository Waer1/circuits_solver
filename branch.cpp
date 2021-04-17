#include "branch.h"

branch::branch(Node* endv, Node* startv)
{
	endNode = endv;
	startNode = startv;
	name = to_string(endNode->getnum()) + to_string(startNode->getnum());
}

Node* branch::getendNode()
{
	return endNode;
}

Node* branch::getstartNode()
{
	return startNode;
}

string branch::getname()
{
	return name;
}
