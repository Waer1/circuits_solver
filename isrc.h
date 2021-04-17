#pragma once
#include "independant.h"
class isrc :
    public independant
{
    double Value, phase;
    complex <double> current;

public:
    isrc(string namev, Node* end, Node* strat, double value, double phase);
    pair<double, double> getCompPhase();
    pair<double, double> getComplex();

	complex<double> getCurrent();

};

