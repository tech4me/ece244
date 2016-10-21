#include "Resistor.h"
#include <iomanip>

Resistor::Resistor(int rIndex_, string name_, double resistance_, int endpoints_[2]) // Constructor to init variables
	: resistor_id(rIndex_), name(name_), resistance(resistance_), endpointNodeIDs{endpoints_[0], endpoints_[1]}
{

}

Resistor::~Resistor() // Empty destructor
{

}

string Resistor::getName() const // const member function
{
	return name;
}

double Resistor::getResistance() const // const member function
{
	return resistance;
}

void Resistor::setResistance(double resistance_)
{
    resistance = resistance_;
}

ostream& operator<<(ostream& out, const Resistor& in) // Overloaded operator<< to print Resistor
{
    return  out << std::left << std::setfill(' ') << std::setw(20) << in.getName() << " " << std::setfill(' ') << std::setw(8) << in.getResistance() << " Ohms " << in.endpointNodeIDs[0] << " -> " << in.endpointNodeIDs[1];
}
