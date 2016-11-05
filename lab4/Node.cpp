#include "Node.h"
#include <iostream>

Node::Node(int nodeid)
	: node_id(nodeid)
	, next(NULL)
{
    
}

Node::~Node()
{

}

ResistorList& Node::get_resistor_list()
{
	return resistor_list;
}

ResistorList Node::c_get_resistor_list() const
{
	return resistor_list;
}

Node*& Node::get_next()
{
	return next;
}

Node* Node::c_get_next() const
{
	return next;
}

int Node::get_nodeid() const
{
	return node_id;
}

int Node::resistor_count()
{
	Resistor* temp_ptr = resistor_list.c_get_resistor();
	int count = 0;
	while (temp_ptr != NULL)
	{
		++count;
		temp_ptr = temp_ptr->c_get_next();
	}
	return count;
}