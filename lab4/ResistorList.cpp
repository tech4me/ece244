#include "ResistorList.h"

ResistorList::ResistorList()
	: head(NULL)
{

}

Resistor*& ResistorList::get_resistor()
{
	return head;
}

Resistor* ResistorList::c_get_resistor() const
{
	return head;
}

void ResistorList::insert_resistor(std::string name, double resistance, int endpoints[2])
{
	Resistor* temp_ptr = head;
	Resistor* pre_ptr = head;
	if (temp_ptr == NULL)
	{
		head = new Resistor(name, resistance, endpoints);
		return;
	}
	while (temp_ptr != NULL)
	{
		pre_ptr = temp_ptr;
		temp_ptr = temp_ptr->c_get_next();
	}
	pre_ptr->get_next() = new Resistor(name, resistance, endpoints);
	return;
}

Resistor* ResistorList::find_resistor_in_resistor_list(std::string name) const
{
	Resistor* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		if (temp_ptr->get_name() == name)
		{
			return temp_ptr;
		}
		temp_ptr = temp_ptr->c_get_next();
	}
	return NULL;
}

void ResistorList::print_resistor_list() const
{
	Resistor* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		std::cout << *temp_ptr << std::endl;
		temp_ptr = temp_ptr->c_get_next();
	}
}