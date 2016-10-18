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

Rparser::~Rparser()
{
    delete[] res_array;
    delete[] node_array;
}

void Rparser::run()
{
	std::string in_buf, in_command;

	std::map<std::string, command> command_map // Map commands to string
	{
        { "maxVal", maxVal },
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
				switch (command_map.at(in_command)) // Commands
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

void Rparser::_maxVal(std::vector<std::string>& in_str)
{
    error_q e;
    int node_n = 1, res_n = 1;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it)
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
        try
        {
            size_t pos;
            node_n = stoi(*it, &pos);
            if (pos < it->size())
                e.error_add(e_invalid_arg(200));
        }
        catch (std::invalid_argument&)
        {
            e.error_add(e_invalid_arg(200));
        }
        // Could also be out_of_range
        if (node_n <= MIN_NODE_NUMBER)
            e.error_add(e_maxval_smaller(200));
        if (++it != in_str.end())
        {
            try
            {
                size_t pos;
                res_n = stoi(*it, &pos);
                if (pos < it->size())
                    e.error_add(e_invalid_arg(199));
            }
            catch (std::invalid_argument&)
            {
                e.error_add(e_invalid_arg(199));
            }
            // Could also be out_of_range
            if (res_n <= MIN_RES_NUMBER)
                e.error_add(e_maxval_smaller(199));
        }
    }

    if (e.no_error())
    {
        node_array_ptr = new Node*[node_n];
        res_array_ptr = new Resistor*[res_n];
		/*for (int i = 0; i < node_n; ++i)
		{
			*(node_array_ptr + i) = new Node();
		}
		for (int i = 0; i < res_n; ++i)
		{
			*(res_array_ptr + i) = new Resistor();
		}*/
    }
    else
        throw e;
}

void Rparser::_insertR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    double resistance = 1;
    int nodeid1 = 1, nodeid2 = 2;
	int counter = 0;
	for (auto it = in_str.begin(); it != in_str.end(); ++it)
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
                    if (nodeid1 < MIN_NODE_NUMBER || nodeid1 > MAX_NODE_NUMBER)
                        e.error_add(e_node_out_of_range(198, nodeid1));
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
                        if (nodeid2 < MIN_NODE_NUMBER || nodeid2 > MAX_NODE_NUMBER)
                            e.error_add(e_node_out_of_range(197, nodeid2));
                        if (nodeid1 == nodeid2)
                            e.error_add(e_both_terminal_no_connect(100, nodeid1));
                    }
                }
            }
        }
    }

    if (e.no_error())
        std::cout << "Inserted: resistor " << name << " " << std::fixed << std::setprecision(2) << resistance << " Ohms " << nodeid1 << " -> " << nodeid2 << std::endl;
    else
        throw e;
}

void Rparser::_modifyR(std::vector<std::string>& in_str)
{
    error_q e;
    std::string name;
    double resistance = 1;
    int counter = 0;
    for (auto it = in_str.begin(); it != in_str.end(); ++it)
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
            }
        }
    }

    if (e.no_error())
        std::cout << "Modified: resistor " << name << " to " << std::setprecision(2) << std::fixed << resistance << " Ohms" << std::endl;
    else
        throw e;
}

void Rparser::_printR(std::vector<std::string>& in_str)
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
            std::cout << "Print: all resistors" << std::endl;
        else
            std::cout << "Print: resistor " << name << std::endl;
    }
    else
        throw e;
}

void Rparser::_printNode(std::vector<std::string>& in_str)
{
    error_q e;
    std::string nodeid;
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
    {
        nodeid = *it;
        if (nodeid != "all")
        {
            int int_nodeid = 1;
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
            if (int_nodeid < MIN_NODE_NUMBER || int_nodeid > MAX_NODE_NUMBER) // Check if nodeid in range
            {
                e.error_add(e_node_out_of_range(200, int_nodeid));
            }
        }
    }

    if (e.no_error())
    {
        if (nodeid == "all")
            std::cout << "Print: all nodes" << std::endl;
        else
            std::cout << "Print: node " << nodeid << std::endl;
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
            std::cout << "Deleted: all resistors" << std::endl;
        else
            std::cout << "Deleted: resistor " << name << std::endl;
    }
    else
        throw e;
}