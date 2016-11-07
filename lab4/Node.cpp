#include "Node.h"
#include <iostream>

Node::Node(int nodeid)
	: node_id(nodeid)
	, next(NULL)
	, voltage(0)
	, is_set(false)
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

double Node::get_voltage() const
{
	return voltage;
}

void Node::set_voltage(double voltage_)
{
	voltage = voltage_;
	is_set = true;
}

void Node::set_voltage_cal(double voltage_)
{
	voltage = voltage_;
}

void Node::unset_voltage()
{
	voltage = 0;
	is_set = false;
}

bool Node::voltage_is_set() const
{
	return is_set ? true : false;
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