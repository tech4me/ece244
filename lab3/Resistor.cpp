#include "Resistor.h"

Resistor::Resistor(int rIndex_, string name_, double resistance_, int endpoints_[2])
	: resistor_id(rIndex_), name(name_), resistance(resistance_), endpointNodeIDs{endpoints_[0], endpoints_[1]}
{

}

inline string Resistor::getName() const
{
	return name;
}

inline double Resistor::getResistance() const
{
	return resistance;
}