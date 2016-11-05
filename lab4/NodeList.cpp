#include "NodeList.h"

NodeList::NodeList()
	: head(NULL)
{

}

Node*& NodeList::get_node()
{
	return head;
}

Node* NodeList::c_get_node() const
{
	return head;
}

Node* NodeList::node_exsist(int nodeid) const
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
	std::cout << "Connections at node " << temp_ptr->get_nodeid() << ": " << temp_ptr->resistor_count() << " resistor(s)" << std::endl;
	temp_ptr->c_get_resistor_list().print_resistor_list();
}

void NodeList::print_node_all() const
{
	Node* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		std::cout << "Connections at node " << temp_ptr->get_nodeid() << ": " << temp_ptr->resistor_count() << " resistor(s)" << std::endl;
		temp_ptr->c_get_resistor_list().print_resistor_list();
		temp_ptr = temp_ptr->c_get_next();
	}
}

void NodeList::delete_resistor_all()
{
	Node* temp_ptr = head;
	Node* temp_save = NULL;
	Resistor* res_temp_ptr = NULL;
	Resistor* res_save = NULL;
	while (temp_ptr != NULL)
	{
		res_temp_ptr = temp_ptr->get_resistor_list().get_resistor();
		while (res_temp_ptr != NULL)
		{
			res_save = res_temp_ptr->c_get_next();
			delete res_temp_ptr;
			temp_ptr->get_resistor_list().get_resistor() = res_save;
			res_temp_ptr = temp_ptr->get_resistor_list().get_resistor();
			if (res_temp_ptr == NULL)
			{
				break;
			}
			else
			{
				if (res_temp_ptr->c_get_next() == NULL)
				{
					delete res_temp_ptr;
					res_temp_ptr = NULL;
					temp_ptr->get_resistor_list().get_resistor() = NULL;
				}
				else
				{
					res_temp_ptr = res_temp_ptr->c_get_next();
				}
			}
		}
		temp_save = temp_ptr->c_get_next();
		delete temp_ptr;
		head = temp_save;
		temp_ptr = head;
		if (temp_ptr == NULL)
		{
			return;
		}
		else
		{
			if (temp_ptr->c_get_next() == NULL)
			{
				delete temp_ptr;
				temp_ptr = NULL;
				head = NULL;
			}
			else
			{
				temp_ptr = temp_ptr->c_get_next();
			}
		}
	}
}