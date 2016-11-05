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

public:
    
    Node(int nodeid);
	~Node();

	ResistorList& get_resistor_list();
	ResistorList c_get_resistor_list() const;

	Node*& get_next(); 
	Node* Node::c_get_next() const;

	int get_nodeid() const; // Get nodeid
	int resistor_count(); // Get the number of resistors on this node
};

#endif	/* NODE_H */
