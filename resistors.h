#pragma once
#include "Comp.h"
class resistors :
    public Comp
{

public:
    resistors(string namev, Node* end, Node* strat);
	virtual complex<double> getComplex();

	virtual void poly() {};
};

