#pragma once
#include "resistors.h"
class ind :
    public resistors
{
private:
    complex <double> impedance;


public:
    ind(string namev, Node* end, Node* strat, double impedace);
	virtual complex<double> getComplex();


	virtual void poly() {};
};

