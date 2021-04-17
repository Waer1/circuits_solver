#include "ccvs.h"

ccvs::ccvs(string namev, Node* end, Node* strat, Node* endre, Node* startre, string namerev, double coffv):dependant(namev,end,strat) ,nameref(namerev), coff(coffv)
{
	endref = endre;
	startref = startre;
	
}

double ccvs::getCoff()
{
	return coff;
}

Node* ccvs::getEndNoderef()
{
	return endref;
}

Node* ccvs::getstartNoderef()
{
	return startref;
}

string ccvs::getrefname()
{
	return nameref;
}

