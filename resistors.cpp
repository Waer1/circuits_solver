#include "resistors.h"

resistors::resistors(string namev, Node* end, Node* strat):Comp(namev , end , strat)
{
}

complex<double> resistors::getComplex()
{
	return complex<double>();
}
