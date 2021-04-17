#include "isrc.h"


isrc::isrc(string namev, Node* end, Node* strat, double valuev, double phasev) :independant(namev, end, strat), Value(valuev), phase(phasev)
{
	double angle = phase * (pi / 180.0);
	current = polar(Value * 1.0, angle);
}

pair<double, double> isrc::getCompPhase()
{
	return pair<double, double>(Value, phase);
}

pair<double, double> isrc::getComplex()
{
	return pair<double, double>(real(current), imag(current));
}

complex<double> isrc::getCurrent()
{
	return current;
}
