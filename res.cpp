#include "res.h"

res::res(string namev, Node* end, Node* strat, double impedacev): resistors(namev, end , strat)
{
	complex<double> tmp(impedacev, 0.0);
	impedance = tmp;
}

complex<double> res::getComplex()
{
	return impedance;
}
