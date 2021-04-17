#include "cap.h"


cap::cap(string namev, Node* end, Node* strat, double impedacev) : resistors(namev, end, strat)
{
	complex<double> tmp(0.0 , impedacev);
	impedance = tmp;
}

complex<double> cap::getComplex()
{
	return impedance;
}