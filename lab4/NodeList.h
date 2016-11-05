#ifndef NODELIST_H
#define NODELIST_H

#include "Node.h"

class NodeList
{
private:
	Node* head;

public:
	NodeList();
	Node*& get_node();
	Node* c_get_node() const;

	Node* node_exsist(int nodeid) const;
	Node* insert_node(int nodeid);
	Resistor* find_resistor_in_node_list(std::string name) const;

	void print_resistor(std::string name) const;
	void print_node(int nodeid) const;
	void print_node_all() const;

	void delete_resistor_all();
};

#endif