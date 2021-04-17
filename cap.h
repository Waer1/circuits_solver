#pragma once
#include "resistors.h"
class cap :
    public resistors
{

private:
    complex <double> impedance;

public:
    cap(string namev, Node* end, Node* strat, double impedace);
	virtual complex<double> getComplex();

	virtual void poly() {};
};

