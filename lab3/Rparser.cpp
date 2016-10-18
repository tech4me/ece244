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

Rparser::Rparser() : node_array_ptr(NULL), res_array_ptr(NULL), maxval_is_set(false), node_n(-1), res_n(-1), current_node_n(-1), current_res_n(-1)
{

}

Rparser::~Rparser()
{
    for (int i = 0; i <= current_node_n; ++i)
    {
        delete *(node_array_ptr + i);
    }
    for (int i = 0; i < current_res_n; ++i)
    {
        delete *(res_array_ptr + i);
    }
    delete[] res_array_ptr;
    delete[] node_array_ptr;
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
                case maxVal:
                    _maxVal(in_str);
                    break;
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
    node_n = 1;
    res_n = 1;
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

    if (e.no_error()) // Create or Re-Create the dynamic allocated space for the array of pointer to Node / Resistor
    {
        if (maxval_is_set)
        {
            for (int i = 0; i <= current_node_n; ++i)
            {
                delete *(node_array_ptr + i);
            }
            for (int i = 0; i < current_res_n; ++i)
            {
                delete *(res_array_ptr + i);
            }
            delete[] node_array_ptr;
            delete[] res_array_ptr;

            MAX_NODE_NUMBER = node_n;
            node_array_ptr = new Node*[node_n + 1];
            res_array_ptr = new Resistor*[res_n];

            current_node_n = node_n;
            current_res_n = 0;

            for (int i = 0; i <= node_n; ++i)
            {
                *(node_array_ptr + i) = new Node();
            }
        }
        else
        {
            MAX_NODE_NUMBER = node_n;
            node_array_ptr = new Node*[node_n + 1];
            res_array_ptr = new Resistor*[res_n];
            maxval_is_set = true;

            for (int i = 0; i <= node_n; ++i)
            {
                *(node_array_ptr + i) = new Node();
            }

            current_node_n = node_n;
        }
        std::cout << "New network: max node number is " << node_n << "; max resistors is " << res_n << std::endl;
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

                        for (int i = 0; i < current_res_n; ++i) // Check for resistor name
                        {
                            if ((*(res_array_ptr + i))->getName() == name)
                            {
                                e.error_add(e_r_name_exist(0, name));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (e.no_error())
    {
        if ((*(node_array_ptr + nodeid1))->canAddResistor(current_res_n))
        {
            if ((*(node_array_ptr + nodeid2))->canAddResistor(current_res_n))
            {
                if (nodeid1 == nodeid2)
                    e.error_add(e_both_terminal_no_connect(0, nodeid1));
                else
                {
                    if (current_res_n < res_n)
                    {
                        int endpoints[2] = { nodeid1, nodeid2 };
                        *(res_array_ptr + current_res_n) = new Resistor(current_res_n, name, resistance, endpoints); // Add a new resistor

                        (*(node_array_ptr + nodeid1))->addResistor(current_res_n);
                        (*(node_array_ptr + nodeid2))->addResistor(current_res_n);

                        ++current_res_n;

                        std::cout << "Inserted: resistor " << name << " " << std::fixed << std::setprecision(2) << resistance << " Ohms " << nodeid1 << " -> " << nodeid2 << std::endl;
                    }
                    else
                    {
                        e.error_add(e_r_array_full(0));
                    }
                }
            }
            else
            {
                e.error_add(e_node_full(0));
            }
        }
        else
        {
            e.error_add(e_node_full(0));
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
    {
        int name_r_id = -1;
        for (int i = 0; i < current_res_n; ++i) // Check for resistor name
        {
            if ((*(res_array_ptr + i))->getName() == name)
                name_r_id = i;
        }
        if (name_r_id != -1)
        {
            double pre_r = (*(res_array_ptr + name_r_id))->getResistance();
            (*(res_array_ptr + name_r_id))->setResistance(resistance);
            std::cout << "Modified: resistor " << name << " from " << std::setprecision(2) << std::fixed << pre_r <<" Ohms to " << std::setprecision(2) << std::fixed << resistance << " Ohms" << std::endl;
        }
        else
        {
            e.error_add(e_r_name_not_found(0, name));
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
        int name_r_id = -1;
        for (int i = 0; i < current_res_n; ++i) // Check for resistor name
        {
            if ((*(res_array_ptr + i))->getName() == name)
                name_r_id = i;
        }
        if (name_r_id != -1)
        {
            std::cout << "Print:" << std::endl;
            std::cout << **(res_array_ptr + name_r_id) << std::endl;
        }
        else if ((name_r_id == -1) && (name == "all"))
        {
            std::cout << "Print:" << std::endl;
            for (int i = 0; i < current_res_n; ++i) // Print every resistor
            {
                std::cout << **(res_array_ptr + i) << std::endl;
            }
        }
        else
        {
            e.error_add(e_r_name_not_found(0, name));
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
        if (nodeid != "all")
        {
            std::cout << "Print:" << std::endl;
            (*(node_array_ptr + int_nodeid))->print(int_nodeid, res_array_ptr);
        }
        else
        {
            std::cout << "Print:" << std::endl;
            for (int i = 0; i <= MAX_NODE_NUMBER; ++i)
            {
                (*(node_array_ptr + i))->print(i, res_array_ptr);
            }
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
            for (int i = 0; i <= current_node_n; ++i)
            {
                delete *(node_array_ptr + i);
            }
            for (int i = 0; i < current_res_n; ++i)
            {
                delete *(res_array_ptr + i);
            }
            for (int i = 0; i <= node_n; ++i)
            {
                *(node_array_ptr + i) = new Node();
            }
            std::cout << "Deleted: all resistors" << std::endl;
        }
        if (!e.no_error())
            throw e;
    }
    else
        throw e;
}