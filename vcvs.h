#pragma once
#include "dependant.h"
class vcvs :
    public dependant
{

    double coff;
    Node * endref;
    Node* startref;


public:
    vcvs(string namev, Node* end, Node* strat , Node * endre ,Node * startre, double coffv);
    double getCoff();
    Node* getEndNoderef();
    Node* getstartNoderef();

};

