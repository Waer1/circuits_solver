#include "vcvs.h"

vcvs::vcvs(string namev, Node* end, Node* strat, Node* endre, Node* startre, double coffv):dependant(namev , end ,strat) ,coff(coffv)
{
	endref = endre;
	startref = startre;
}

double vcvs::getCoff()
{
	return coff;
}

Node* vcvs::getEndNoderef()
{
	return endref;
}

Node* vcvs::getstartNoderef()
{
	return startref;
}
