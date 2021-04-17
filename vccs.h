#pragma once
#include "dependant.h"
class vccs : public dependant
{
    double coff;
    Node* endref;
    Node* startref;


public:
    vccs(string namev, Node* end, Node* strat, Node* endre, Node* startre, double coff);
    double getCoff();
    Node* getEndNoderef();
    Node* getstartNoderef();




};

