#ifndef NODELIST_H
#define NODELIST_H

#include "Node.h"
#include <vector>

#define MIN_ITERATION_CHANGE 0.0001

class NodeList
{
private:
	Node* head;

	bool voltage_difference_not_ok(const std::vector<double>& current, const std::vector<double>& previous) const;
	double get_equation_r(Node* ptr) const; // Calculate the part with only R
	double get_equation_v_r(Node* ptr) const; // Calculate the part with both R and V

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

	void delete_resistor(std::string name);
	void delete_resistor_all();

	bool check_voltage_set() const;

	void solve() const;
};

#endif