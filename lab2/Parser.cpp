#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define MIN_NODE_NUMBER 0
#define MAX_NODE_NUMBER 5000

enum command { insertR, modifyR, printR, printNode, deleteR }; // Commands list

class e_invalid_arg : public std::exception
{
	virtual const char* what() const throw()
	{
		return "invalid argument";
	}
};

class e_negative_r : public std::exception
{
	virtual const char* what() const throw()
	{
		return "negative resistance";
	}
};

class e_node_out_of_range : public std::exception
{
public:
	e_node_out_of_range(std::string in) : input(in) { temp = std::string("node ") + input + std::string(" is out of permitted range ") + STR(MIN_NODE_NUMBER) + std::string("-") + STR(MAX_NODE_NUMBER); };
	~e_node_out_of_range() throw() {};
private:
	std::string input;
	std::string temp;

	virtual const char* what() const throw()
	{
		return temp.c_str();
	}
};

class e_r_name_cannot_be_all : public std::exception
{
	virtual const char* what() const throw()
	{
		return "resistor name cannot be the keyword \"all\"";
	}
};

class e_both_terminal_no_connect : public std::exception
{
public:
	e_both_terminal_no_connect(std::string in) : input(in) { temp = "both termials of resistor connect to node" + input; };
	~e_both_terminal_no_connect() throw() {};
private:
	std::string input;
	std::string temp;

	virtual const char* what() const throw()
	{
		return temp.c_str();
	}
};

class e_too_many_args : public std::exception
{
	virtual const char* what() const throw()
	{
		return "too many arguments";
	}
};

class e_too_few_args : public std::exception
{
	virtual const char* what() const throw()
	{
		return "too few arguments";
	}
};

int parser();
void _insertR(std::stringstream& in_str_stream);
void _modifyR(std::stringstream& in_str_stream);
void _printR(std::stringstream& in_str_stream);
void _printNode(std::stringstream& in_str_stream);
void _deleteR(std::stringstream& in_str_stream);

int main()
{
	parser();
	return 0;
}

int parser()
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
		in_str_stream >> in_command;
		try
		{
			switch (command_map.at(in_command)) // Commands
			{
			case insertR:
				_insertR(in_str_stream);
				break;
			case modifyR:
				_modifyR(in_str_stream);
				break;
			case printR:
				_printR(in_str_stream);
				break;
			case printNode:
				_printNode(in_str_stream);
				break;
			case deleteR:
				_deleteR(in_str_stream);
				break;

			}
		}
		catch (const std::out_of_range& e) // Undefined command
		{
			std::cout << "Error: invalid command" << std::endl;
			//std::cout << e.what() << std::endl; // Print out exception type
		}
		catch (const std::exception& e) // Other
		{
			std::cout << "Error: " << e.what() << std::endl; // Print out exception type
		}

		std::cout << "> "; // Sign for each line
		std::getline(std::cin, in_buf);
	}
	return -1;
}

void _insertR(std::stringstream& in_str_stream)
{
	std::stringstream temp_str_stream;
	temp_str_stream << in_str_stream.str();
	std::string temp;
	std::string dump;
	temp_str_stream >> dump;
	int counter = 0;
	while (temp_str_stream >> temp)
	{
		++counter;
	}
	if (counter < 4)
	{
		throw e_too_few_args();
	}
	else if (counter > 4)
	{
		throw e_too_many_args();
	}
	else
	{
		std::string name;
		std::string resistance;
		std::string nodeid1;
		std::string nodeid2;
		double double_res;
		try
		{
			in_str_stream >> name;
			if (name == "all")
			{
				throw e_r_name_cannot_be_all();
			}

			in_str_stream >> resistance;
			double_res = std::stod(resistance);
			if (double_res < 0)
			{
				throw e_negative_r();
			}

			in_str_stream >> nodeid1;
			in_str_stream >> nodeid2;
			int temp1 = std::stoi(nodeid1); // Check if nodeid contains other character
			if (temp1 < MIN_NODE_NUMBER || temp1 > MAX_NODE_NUMBER) // Check if nodeid in range
			{
				throw e_node_out_of_range(nodeid1);
			}
			int int_digit;
			std::stringstream temp_nodeid;
			temp_nodeid << nodeid1;
			temp_nodeid >> int_digit;
			if (temp_nodeid.peek() != std::char_traits<char>::eof()) // Check for digit sign
			{
				throw e_invalid_arg();
			}

			temp_nodeid.str("");

			int temp2 = std::stoi(nodeid2); // Check if nodeid contains other character
			if (temp2 < MIN_NODE_NUMBER || temp2 > MAX_NODE_NUMBER) // Check if nodeid in range
			{
				throw e_node_out_of_range(nodeid2);
			}
			temp_nodeid << nodeid2;
			temp_nodeid >> int_digit;
			if (temp_nodeid.peek() != std::char_traits<char>::eof()) // Check for digit sign
			{
				throw e_invalid_arg();
			}
			
			if (temp1 == temp2)
			{
				throw e_both_terminal_no_connect(nodeid1);
			}
		}
		catch (const std::invalid_argument& e)  // Cannot convert to int
		{
			throw e_invalid_arg();
		}
		std::cout << "Inserted: resistor " << name << " " << std::fixed << std::setprecision(2) << double_res << " Ohms " << nodeid1 << " -> " << nodeid2 << std::endl;
	}
}

void _modifyR(std::stringstream& in_str_stream)
{
	std::stringstream temp_str_stream;
	temp_str_stream << in_str_stream.str();
	std::string temp;
	std::string dump;
	temp_str_stream >> dump;
	int counter = 0;
	while (temp_str_stream >> temp)
	{
		++counter;
	}
	if (counter < 2)
	{
		throw e_too_few_args();
	}
	else if (counter > 2)
	{
		throw e_too_many_args();
	}
	else
	{
		std::string name;
		std::string resistance;
		double double_res;
		in_str_stream >> name;
		if (name == "all")
		{
			throw e_r_name_cannot_be_all();
		}
		in_str_stream >> resistance;
		try
		{
			double_res = std::stod(resistance);
			if (double_res < 0)
			{
				throw e_negative_r();
			}
		}
		catch (const std::invalid_argument& e)  // Cannot convert to int
		{
			throw e_invalid_arg();
		}
		std::cout << "Modified: resistor " << name << " to " << std::setprecision(2) << std::fixed << double_res << " Ohms" << std::endl;
	}
}

void _printR(std::stringstream& in_str_stream)
{
	std::string name;
	if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_few_args();
	}
	in_str_stream >> name;
	if (name == "all")
	{
		std::cout << "Print: all resistors" << std::endl;
	}
	else
	{
		std::cout << "Print: resistor " << name << std::endl;
	}
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_many_args();
	}
}

void _printNode(std::stringstream& in_str_stream)
{
	std::string nodeid;
	if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_few_args();
	}
	in_str_stream >> nodeid;
	if (nodeid == "all")
	{
		std::cout << "Print: all nodes" << std::endl;
	}
	else
	{
		try
		{
			int temp = std::stoi(nodeid); // Check if nodeid contains other character
			if (temp < MIN_NODE_NUMBER || temp > MAX_NODE_NUMBER) // Check if nodeid in range
			{
				throw e_node_out_of_range(nodeid);
			}
			int int_digit;
			std::stringstream temp_nodeid;
			temp_nodeid << nodeid;
			temp_nodeid >> int_digit;
			if (temp_nodeid.peek() != std::char_traits<char>::eof()) // Check for digit sign
			{
				throw e_invalid_arg();
			}
			std::cout << "Print: node " << nodeid << std::endl;
		}
		catch (const std::invalid_argument& e) // Cannot convert to int
		{
			throw e_invalid_arg();
		}
		catch (const std::out_of_range& e) // Out of the bound of int
		{
			throw e_node_out_of_range(nodeid);
		}
	}
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_many_args();
	}
}

void _deleteR(std::stringstream& in_str_stream)
{
	std::string name;
	if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_few_args();
	}
	in_str_stream >> name;
	if (name == "all")
	{
		std::cout << "Deleted: all resistors" << std::endl;
	}
	else
	{
		std::cout << "Deleted: resistor " << name << std::endl;
	}
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
	{
		throw e_too_many_args();
	}
}
