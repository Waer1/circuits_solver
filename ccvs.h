#pragma once
#include "dependant.h"
class ccvs :
    public dependant
{

    double coff;
    string nameref;
    Node* endref;
    Node* startref;


public:
    ccvs(string namev, Node* end, Node* strat, Node* endre, Node* startre, string namere, double coff);
    double getCoff();
    Node* getEndNoderef();
    Node* getstartNoderef();
    string getrefname();
    

};

