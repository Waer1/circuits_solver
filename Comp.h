#pragma once

#include <iostream>
#include <complex>

#include "Node.h"

using namespace std;

class Comp
{
	string Name;
	Node * Endnode;
	Node * Startnode;
	bool battryUsed;

public:
	double pi = 3.141593;
	Comp(string namev, Node * end, Node * start);
	void print();
	int getEndNode();
	int getStartNode();
	virtual void poly();
	bool takencomp();
	void setbattry();
	bool getbattry();

};

