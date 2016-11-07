#include "NodeList.h"
#include <iomanip>

NodeList::NodeList()
	: head(NULL)
{

}

bool NodeList::voltage_difference_not_ok(const std::vector<double>& current, const std::vector<double>& previous) const
{
	double max_voltage = 0;
	auto it_p = previous.begin();
	for (auto it_c = current.begin(); it_c != current.end(); ++it_c)
	{
		if (abs(abs(*it_c) - abs(*it_p)) > MIN_ITERATION_CHANGE)
		{
			return true;
		}
		++it_p;
	}
	return false;
}

double NodeList::get_equation_r(Node* ptr) const
{
	double result = 0;
	Resistor* temp_ptr = ptr->c_get_resistor_list().c_get_resistor();
	while (temp_ptr != NULL)
	{
		result = result + 1 / temp_ptr->get_resistance();
		temp_ptr = temp_ptr->c_get_next();
	}
	return result ? (1 / result) : 0;
}

double NodeList::get_equation_v_r(Node* ptr) const
{
	double result = 0;
	Resistor* temp_ptr = ptr->c_get_resistor_list().c_get_resistor();
	while (temp_ptr != NULL)
	{
		if (ptr->get_nodeid() == temp_ptr->get_end_id_1())
		{
			result += (node_exsist(temp_ptr->get_end_id_2())->get_voltage() / temp_ptr->get_resistance());
		}
		else
		{
			result += (node_exsist(temp_ptr->get_end_id_1())->get_voltage() / temp_ptr->get_resistance());
		}
		temp_ptr = temp_ptr->c_get_next();
	}
	return result;
}

Node*& NodeList::get_node()
{
	return head;
}

Node* NodeList::c_get_node() const
{
	return head;
}

Node* NodeList::node_exsist(int nodeid) const // Determin if a node exsist in nodelist
{
	Node* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		if (temp_ptr->get_nodeid() == nodeid)
		{
			return temp_ptr;
		}
		temp_ptr = temp_ptr->c_get_next();
	}
	return NULL;
}

Node* NodeList::insert_node(int nodeid) // Insert node to node_list with the right order
{
	Node* node_exsist_return = node_exsist(nodeid);
	if (node_exsist_return != NULL)
	{
		return node_exsist_return;
	}
	Node* temp_ptr = head;
	Node* pre_ptr = head;
	if (temp_ptr == NULL)
	{
		head = new Node(nodeid);
		return head;
	}
	if (temp_ptr->get_nodeid() > nodeid)
	{
		head = new Node(nodeid);
		head->get_next() = temp_ptr;
		return head;
	}
	while (temp_ptr != NULL)
	{
	    if (temp_ptr->get_nodeid() > nodeid)
		{
			pre_ptr->get_next() = new Node(nodeid);
			pre_ptr->get_next()->get_next() = temp_ptr;
			return pre_ptr->c_get_next();
		}
		else
		{
			pre_ptr = temp_ptr;
			temp_ptr = temp_ptr->c_get_next();
		}
	}
	pre_ptr->get_next() = new Node(nodeid);
	return pre_ptr->c_get_next();
}

Resistor* NodeList::find_resistor_in_node_list(std::string name) const
{
	Node* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		Resistor* found_res = temp_ptr->c_get_resistor_list().find_resistor_in_resistor_list(name);
		if (found_res != NULL)
		{
			return found_res;
		}
		temp_ptr = temp_ptr->c_get_next();
	}
	return NULL;
}

void NodeList::print_resistor(std::string name) const
{
	Resistor* temp_ptr = head->c_get_resistor_list().find_resistor_in_resistor_list(name);
	std::cout << *temp_ptr << std::endl;
}

void NodeList::print_node(int nodeid) const
{
	Node* temp_ptr = node_exsist(nodeid);
	if (temp_ptr->resistor_count() != 0) // Only print if node has resistors
	{
		std::cout << "Connections at node " << temp_ptr->get_nodeid() << ": " << temp_ptr->resistor_count() << " resistor(s)" << std::endl;
		temp_ptr->c_get_resistor_list().print_resistor_list();
	}
}

void NodeList::print_node_all() const
{
	Node* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		if (temp_ptr->resistor_count() != 0) // Only print if node has resistors
		{
			std::cout << "Connections at node " << temp_ptr->get_nodeid() << ": " << temp_ptr->resistor_count() << " resistor(s)" << std::endl;
			temp_ptr->c_get_resistor_list().print_resistor_list();
		}
		temp_ptr = temp_ptr->c_get_next();
	}
}

void NodeList::delete_resistor(std::string name)
{
	Node* temp_ptr = head;
	Resistor* pre_res_ptr = NULL;
	Resistor* save_ptr = NULL;
	Resistor* found_res = NULL;
	while (temp_ptr != NULL)
	{
		found_res = temp_ptr->c_get_resistor_list().find_pre_res_in_resistor_list(pre_res_ptr, name); // found_res is a pointer to the found res and pre_res_ptr is a pointer to the previous element
		if (found_res != NULL)
		{
			if (pre_res_ptr == NULL) // The case of delete first resistor
			{
				temp_ptr->get_resistor_list().get_resistor() = found_res->c_get_next();
				delete found_res;
			}
			else // The other cases
			{
				save_ptr = found_res->c_get_next();
				delete found_res;
				pre_res_ptr->get_next() = save_ptr;
			}
		}
		temp_ptr = temp_ptr->c_get_next();
	}
}

void NodeList::delete_resistor_all()
{
	Node* temp_ptr = head;
	Node* temp_save = NULL;
	Resistor* res_temp_ptr = NULL;
	Resistor* res_save = NULL;
	while (temp_ptr != NULL) // Delete all nodes
	{
		res_temp_ptr = temp_ptr->get_resistor_list().get_resistor();
		while (res_temp_ptr != NULL) // Delete resistor for each node
		{
			res_save = res_temp_ptr->c_get_next();
			delete res_temp_ptr;
			temp_ptr->get_resistor_list().get_resistor() = res_save;
			res_temp_ptr = temp_ptr->get_resistor_list().get_resistor();
			if (res_temp_ptr == NULL) // Meet the end of resistorlist
			{
				break;
			}
		}
		temp_save = temp_ptr->c_get_next();
		delete temp_ptr;
		head = temp_save;
		temp_ptr = head;
		if (temp_ptr == NULL) // Meet the end of nodelist
		{
			return;
		}
	}
}

bool NodeList::check_voltage_set() const
{
	Node* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		if (temp_ptr->voltage_is_set())
		{
			return true;
		}
		temp_ptr = temp_ptr->c_get_next();
	}
	return false;
}

void NodeList::solve() const
{
	Node* temp_ptr = head;
	std::vector<double> current_voltage;
	std::vector<double> previous_voltage;
	while (temp_ptr != NULL) // Fill voltage vectors with voltages
	{
		current_voltage.push_back(temp_ptr->get_voltage());
		previous_voltage.push_back(0);
		temp_ptr = temp_ptr->c_get_next();
	}
	while (voltage_difference_not_ok(current_voltage, previous_voltage))
	{
		temp_ptr = head;
		Node* temp_ptr_copy = head;
		for (auto it_p = previous_voltage.begin(); it_p != previous_voltage.end(); ++it_p, temp_ptr_copy = temp_ptr_copy->c_get_next())
		{
			*it_p = temp_ptr_copy->get_voltage();
		}
		auto it_c = current_voltage.begin();
		while (temp_ptr != NULL)
		{
			if (!temp_ptr->voltage_is_set())
			{
				temp_ptr->set_voltage_cal(get_equation_r(temp_ptr)*get_equation_v_r(temp_ptr));
				*it_c = temp_ptr->get_voltage();
			}
			++it_c;
			temp_ptr = temp_ptr->c_get_next();
		}
	}
	// Print results
	Node* print_ptr = head;
	while (print_ptr != NULL)
	{
		std::cout << "  Node " << print_ptr->get_nodeid() << ": " << std::setprecision(2) << std::fixed << print_ptr->get_voltage() << " V" << std::endl;
		print_ptr = print_ptr->c_get_next();
	}
}