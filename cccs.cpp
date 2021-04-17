#include "cccs.h"

cccs::cccs(string namev, Node* end, Node* strat, Node* endre, Node* startre, string namerev, double coffv) :dependant(namev, end, strat), nameref(namerev),coff(coffv)
{
	endref = endre;
	startref = startre;
}

double cccs::getCoff()
{
	return coff;
}

Node* cccs::getEndNoderef()
{
	return endref;
}

Node* cccs::getstartNoderef()
{
	return startref;
}

string cccs::getrefname()
{
	return nameref;
}
