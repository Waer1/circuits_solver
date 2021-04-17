#pragma once
#include "independant.h"
class vsrc :
    public independant
{
    double Value, phase;
    complex <double> voltage;
    
    
public:
    vsrc(string namev, Node* end, Node* strat, double value ,double phase);
    pair<double, double> getCompPhase();
    pair<double, double> getComplex();

	complex<double> getVolt();


};

