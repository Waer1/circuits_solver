#include "vsrc.h"

vsrc::vsrc(string namev, Node* end, Node* strat, double valuev, double phasev):independant(namev, end ,strat) , Value(valuev),phase(phasev)
{
	double angle = phase * (pi / 180.0);
	voltage = polar(Value * 1.0, angle);
}

pair<double, double> vsrc::getCompPhase()
{
	return pair<double, double>(Value , phase);
}

pair<double, double> vsrc::getComplex()
{
	return pair<double, double>(real(voltage) , imag(voltage));
}

complex<double> vsrc::getVolt()
{
	return voltage;
}
