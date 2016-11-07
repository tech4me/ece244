#ifndef NODE_H
#define NODE_H

#include "Resistor.h"
#include "ResistorList.h"

class Node
{
private: 
	int node_id; // ID of node
	ResistorList resistor_list;
	Node* next;
	double voltage;
	bool is_set;

public:
    
    Node(int nodeid);
	~Node();

	ResistorList& get_resistor_list();
	ResistorList c_get_resistor_list() const;

	Node*& get_next(); 
	Node* c_get_next() const;

	double get_voltage() const; // Return voltage
	void set_voltage(double voltage_); // Set the voltage
	void set_voltage_cal(double voltage_); // Set voltage in calculation without changing is_set
	void unset_voltage(); // Unset the voltage
	bool voltage_is_set() const; // Determine if voltage is set
	int get_nodeid() const; // Get nodeid
	int resistor_count(); // Get the number of resistors on this node
};

#endif	/* NODE_H */
