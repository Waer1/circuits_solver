#include "Comp.h"

Comp::Comp(string namev, Node* end, Node* start):Name(namev),battryUsed(0)
{
	Endnode = end;
	Startnode = start;
	Endnode->increment();
	Startnode->increment();
}

void Comp::print()
{
	cout << "the component which have name : (" << Name << " )" << "tied with end node ";
	Endnode->print();
	cout << "the start node is :";
	Startnode->print();


}

int Comp::getEndNode()
{
	return Endnode->getnum();
}

int Comp::getStartNode()
{
	return Startnode->getnum();
}

void Comp::poly()
{
}

bool Comp::takencomp()
{

	return (Endnode->getstnode() && Startnode->getstnode()) ? (1) : (0);

}

void Comp::setbattry()
{
	battryUsed = 1;
}

bool Comp::getbattry()
{
	return battryUsed;
}


