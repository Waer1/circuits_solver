#include "Node.h"

Node::Node(int n):num(n), count(0),taken(0),basnodestate(0),normal(0)
{
	complex<double>lol(978420.5, 3948148.6);
	voltage = lol;
}

int Node::getnum()
{
	return num;
}

void Node::setVoltage(complex<double> v)
{
	voltage = v;
}

void Node::print()
{
	std::cout << "the node number : ( " << num << " ) " << std::endl;
}

void Node::increment()
{
	count++;
}

bool Node::IsComplex()
{
	return (count < 3) ? (false) : (true);
}

complex<double> Node::getVolt()
{
	return voltage;
}

bool Node::getstbas()
{
	return basnodestate;
}

void Node::setusedbas()
{
	basnodestate = 1;
}

bool Node::getnormal()
{
	return normal;
}

void Node::setnormal()
{
	normal = 1;
}

bool Node::getstnode()
{
	return taken;
}

void Node::setusednode()
{
	taken = 1;
}
