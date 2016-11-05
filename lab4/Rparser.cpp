/**
*
* Note to the error checking system:
* Since position of the error has higher priority then the order of the error list,
* the scores given to position are higher in a scale to ensure the correct output.
* The error checking system still can be improved to meet mordern C++ standard.
*
*/

#include "Rparser.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

Rparser::Rparser()
{

}

Rparser::~Rparser() // Destructor, free everything when exit
{
	
}

void Rparser::run()
{
	std::string in_buf, in_command;

	std::map<std::string, command> command_map // Map commands to string
	{
		{ "insertR", insertR },
		{ "modifyR", modifyR },
		{ "printR", printR },
		{ "printNode", printNode },
		{ "deleteR", deleteR },
	};

	std::cout << "> "; // Sign for each line
	std::getline(std::cin, in_buf);
	while (!std::cin.eof())
	{
		std::stringstream in_str_stream(in_buf);
		std::vector<std::string> in_str;
		std::string word;
		while (in_str_stream >> word)
		{
			in_str.push_back(word); // Read input into a vector of strings
		}

		if (in_str.begin() == in_str.end())
			std::cout << "Error: invalid command" << std::endl;
		else
		{
			in_command = *in_str.begin();
			try
			{
				switch (command_map.at(in_command)) // Commands handling
				{
				case insertR:
					_insertR(in_str);
					break;
				case modifyR:
					_modifyR(in_str);
					break;
				case printR:
					_printR(in_str);
					break;
				case printNode:
					_printNode(in_str);
					break;
				case deleteR:
					_deleteR(in_str);
					break;
				case setV:
					//_setV(in_str);
					break;
				case unsetV:
					//_unsetV(in_str);
					break;
				case solve:
					//_solve(in_str);
					break;
				}
			}
			catch (const std::out_of_range&) // Undefined command
			{
				std::cout << "Error: invalid command" << std::endl;
			}
			catch (const error_q& e) // Other
			{
				std::cout << "Error: " << e.what() << std::endl; // Print out exception type
			}
		}

		std::cout << "> "; // Sign for each line
		std::getline(std::cin, in_buf);
		in_command = "";
	}
}

void Rparser::_insertR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    double resistance = 1;
    int nodeid1 = 1, nodeid2 = 2;
	int counter = 0;
	for (auto it = in_str.begin(); it != in_str.end(); ++it) // Check for number of arguments
	{
		++counter;
	}
    if (counter < 5)
        e.error_add(e_too_few_args(100));
	else if (counter > 5)
        e.error_add(e_too_many_args(100));

    auto it = in_str.begin();
    if (++it != in_str.end())
    {
        name = *it;
        if (name == "all")
            e.error_add(e_r_name_cannot_be_all(200));
        else
        {
            if (++it != in_str.end())
            {
                try
                {
                    size_t pos;
                    resistance = stod(*it, &pos);
                    if (pos < it->size())
                        e.error_add(e_invalid_arg(199));
                }
                catch (std::invalid_argument&)
                {
                    e.error_add(e_invalid_arg(199));
                }
                // Could also be out_of_range
                if (resistance < 0)
                    e.error_add(e_negative_r(199));
                if (++it != in_str.end())
                {
                    try
                    {
                        size_t pos;
                        nodeid1 = stoi(*it, &pos);
                        if (pos < it->size())
                            e.error_add(e_invalid_arg(198));
                    }
                    catch (std::invalid_argument&)
                    {
                        e.error_add(e_invalid_arg(198));
                    }
                    // Could also be out_of_range
                    if (++it != in_str.end())
                    {
                        try
                        {
                            size_t pos;
                            nodeid2 = stoi(*it, &pos);
                            if (pos < it->size())
                                e.error_add(e_invalid_arg(197));
                        }
                        catch (std::invalid_argument&)
                        {
                            e.error_add(e_invalid_arg(197));
                        }
                        // Could also be out_of_range
						if (node_list.find_resistor_in_node_list(name) != NULL) // Find if resistor already exsist
						{
							e.error_add(e_r_name_exist(500, name));
						}
                    }
                }
            }
        }
    }

    if (e.no_error()) // If no input error from lab2
    {
        if (nodeid1 == nodeid2)
            e.error_add(e_both_terminal_no_connect(100, nodeid1));
        else
        {
			Node* node1 = node_list.insert_node(nodeid1);
			Node* node2 = node_list.insert_node(nodeid2);
            int endpoints[2] = { nodeid1, nodeid2 };
            node1->get_resistor_list().insert_resistor(name, resistance, endpoints); // Add a new resistor instance
			node2->get_resistor_list().insert_resistor(name, resistance, endpoints); // Add a new resistor instance

            std::cout << "Inserted: resistor " << name << " " << std::fixed << std::setprecision(2) << resistance << " Ohms " << nodeid1 << " -> " << nodeid2 << std::endl;
        }
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}

void Rparser::_modifyR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    double resistance = 1;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it) // Number of arguments
    {
        ++counter;
    }
    if (counter < 3)
        e.error_add(e_too_few_args(100));
    else if (counter > 3)
        e.error_add(e_too_many_args(100));

    auto it = in_str.begin();
    if (++it != in_str.end())
    {
        name = *it;
        if (name == "all")
            e.error_add(e_r_name_cannot_be_all(200));
        else
        {
            if (++it != in_str.end()) // Cases where problems with resistor
            {
                try
                {
                    size_t pos;
                    resistance = stod(*it, &pos);
                    if (pos < it->size())
                        e.error_add(e_invalid_arg(199));
                }
                catch (std::invalid_argument&)
                {
                    e.error_add(e_invalid_arg(199));
                }
                // Could also be out_of_range
                if (resistance < 0)
                    e.error_add(e_negative_r(199));
            }
        }
    }

    if (e.no_error())
    {
		Resistor* res_ptr = node_list.find_resistor_in_node_list(name);
		if (res_ptr != NULL)
		{
			double pre_r = res_ptr->get_resistance();
			int other_node = res_ptr->get_large_endpoint_id();
			res_ptr->set_resistance(resistance);
			node_list.node_exsist(other_node)->get_resistor_list().find_resistor_in_resistor_list(name)->set_resistance(resistance);
			std::cout << "Modified: resistor " << name << " from " << std::setprecision(2) << std::fixed << pre_r << " Ohms to " << std::setprecision(2) << std::fixed << resistance << " Ohms" << std::endl;
		}
		else
		{
			e.error_add(e_r_name_not_found(0, name)); // Can not find resistor
		}
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}

void Rparser::_printR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it) // total number of arguments
    {
        ++counter;
    }
    if (counter < 2)
        e.error_add(e_too_few_args(100));
    else if (counter > 2)
        e.error_add(e_too_many_args(100));

    auto it = in_str.begin();
    if (++it != in_str.end())
        name = *it;

    if (e.no_error())
    {
		if (node_list.find_resistor_in_node_list(name) != NULL)
		{
			std::cout << "Print:" << std::endl;
			node_list.print_resistor(name);
		}
		else
		{
			e.error_add(e_r_name_not_found(0, name)); // Can not find resistor
		}
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}

void Rparser::_printNode(std::vector<std::string>& in_str)
{
    error_q e;
    std::string nodeid;
    int int_nodeid = 1;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it) // total number of arguments
    {
        ++counter;
    }
    if (counter < 2)
        e.error_add(e_too_few_args(100));
    else if (counter > 2)
        e.error_add(e_too_many_args(100));

    auto it = in_str.begin();
    if (++it != in_str.end())
    {
        nodeid = *it;
        if (nodeid != "all")
        {
            try
            {
                size_t pos;
                int_nodeid = stoi(*it, &pos);
                if (pos < it->size())
                    e.error_add(e_invalid_arg(200));
            }
            catch (std::invalid_argument&)
            {
                e.error_add(e_invalid_arg(200));
            }
            // Could also be out_of_range
        }
    }

    if (e.no_error())
    {
        if (nodeid != "all")
        {
            std::cout << "Print:" << std::endl;
			if (node_list.node_exsist(int_nodeid) != NULL)
			{
				node_list.print_node(int_nodeid);
			}
			else
			{
				e.error_add(e_nodeid_not_found(0, int_nodeid));
			}
        }
        else
        {
            std::cout << "Print:" << std::endl;
			node_list.print_node_all();
        }
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}

void Rparser::_deleteR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it)
    {
        ++counter;
    }
    if (counter < 2)
        e.error_add(e_too_few_args(100));
    else if (counter > 2)
        e.error_add(e_too_many_args(100));

    auto it = in_str.begin();
    if (++it != in_str.end())
        name = *it;

    if (e.no_error())
    {
        if (name == "all")
        {
			node_list.delete_resistor_all();
            std::cout << "Deleted: all resistors" << std::endl;
        }
		else if (node_list.find_resistor_in_node_list(name) != NULL)
		{

		}
		else
		{
			e.error_add(e_r_name_not_found(0, name)); // Can not find resistor
		}
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}
