#ifndef RESISTORLIST_H
#define RESISTORLIST_H

#include "Resistor.h"

class ResistorList
{
private:
	Resistor* head;

public:
	ResistorList();
	Resistor*& get_resistor();
	Resistor* c_get_resistor() const;

	void insert_resistor(std::string name, double resistance, int endpoints[2]);
	Resistor* find_resistor_in_resistor_list(std::string name) const; // Find the resistor with name return a pointer to the resistor, if not found return NULL
	Resistor* find_pre_res_in_resistor_list(Resistor*& pre_res_ptr, std::string name) const;

	void print_resistor_list() const; // Print all resistors
};

#endif