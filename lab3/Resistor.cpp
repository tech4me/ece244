#include "Resistor.h"
#include <iomanip>

Resistor::Resistor(int rIndex_, string name_, double resistance_, int endpoints_[2])
	: resistor_id(rIndex_), name(name_), resistance(resistance_), endpointNodeIDs{endpoints_[0], endpoints_[1]}
{

}

Resistor::~Resistor()
{

}

string Resistor::getName() const
{
	return name;
}

double Resistor::getResistance() const
{
	return resistance;
}

void Resistor::setResistance(double resistance_)
{
    resistance = resistance_;
}

ostream& operator<<(ostream& out, const Resistor& in) // Overloaded operator<<
{
    return  out << in.getName() << std::setw(20) << " " << std::setw(8) << in.getResistance() << " Ohms " << in.endpointNodeIDs[0] << " -> " << in.endpointNodeIDs[1];
}