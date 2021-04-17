#include "ind.h"

ind::ind(string namev, Node* end, Node* strat, double impedacev) : resistors(namev, end, strat)
{
	complex<double> tmp(0.0 , impedacev);
	impedance = tmp;
}

complex<double> ind::getComplex()
{
	return impedance;
}
