#include "Resistor.h"
#include <iomanip>

Resistor::Resistor(std::string name_, double resistance_, int endpoints_[2]) // Constructor to init variables
	: name(name_)
	, resistance(resistance_)
	, endpointNodeIDs{ endpoints_[0], endpoints_[1]}
	, next(NULL)
{

}

Resistor::~Resistor() // Empty destructor
{

}

Resistor*& Resistor::get_next()
{
	return next;
}
Resistor* Resistor::c_get_next() const // const member function
{
	return next;
}

std::string Resistor::get_name() const // const member function
{
	return name;
}

double Resistor::get_resistance() const // const member function
{
	return resistance;
}

int Resistor::get_large_endpoint_id() const // const member function
{
	return endpointNodeIDs[0] > endpointNodeIDs[1] ? endpointNodeIDs[0] : endpointNodeIDs[1];
}

void Resistor::set_resistance(double resistance_)
{
    resistance = resistance_;
}

std::ostream& operator<<(std::ostream& out, const Resistor& in) // Overloaded operator<< to print Resistor
{
	return  out << std::left << std::setfill(' ') << std::setw(20) << in.get_name() << " " << std::setfill(' ') << std::setw(8) << in.get_resistance() << " Ohms " << in.endpointNodeIDs[0] << " -> " << in.endpointNodeIDs[1];
}
