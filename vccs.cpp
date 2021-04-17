#include "vccs.h"

vccs::vccs(string namev, Node* end, Node* strat, Node* endre, Node* startre, double coffv) : dependant(namev, end, strat), coff(coffv)
{
	endref = endre;
	startref = startre;
}



double vccs::getCoff()
{
	return coff;
}

Node* vccs::getEndNoderef()
{
	return endref;
}

Node* vccs::getstartNoderef()
{
	return startref;
}
