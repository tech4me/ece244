#ifndef RESISTOR_H
#define RESISTOR_H

#include <string>
#include <iostream>

class Resistor
{
private:
	std::string name; // C++ string holding the label
    double resistance; // resistance (in Ohms)
	int endpointNodeIDs[2]; // IDs of nodes it attaches to

	Resistor* next;

public:
    Resistor(std::string name_, double resistance_, int endpoints_[2]);
	~Resistor();

	Resistor*& get_next();
	Resistor* c_get_next() const;
    
    std::string get_name() const; // returns the name
    double get_resistance() const; // returns the resistance
	int get_large_endpoint_id() const; // returns the larger endpoint id
    
    void set_resistance (double resistance_);
    
    // you *may* create either of the below to print your resistor
    void print ();
    friend std::ostream& operator<<(std::ostream&,const Resistor&);
};

std::ostream& operator<<(std::ostream&, const Resistor&);

#endif	/* RESISTOR_H */
