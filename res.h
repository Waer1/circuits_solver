#pragma once
#include "resistors.h"
class res :
    public resistors
{
private:
    complex <double> impedance;

public:
    res(string namev , Node * end , Node * strat , double impedace);
	virtual complex<double> getComplex();


	virtual void poly() {};


};

