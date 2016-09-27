#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <exception>

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define MIN_NODE_NUMBER 0
#define MAX_NODE_NUMBER 5000

enum command { insertR, modifyR, printR, printNode, deleteR }; // Commands list

class e_invalid_arg : public std::exception
{
	virtual const char* what() const
	{
		return "invalid argument";
	}
};

class e_node_out_of_range : public std::exception
{
public:
	e_node_out_of_range(std::string in) : input(in) { temp = std::string("node ") + input + std::string(" is out of permitted range ") + STR(MIN_NODE_NUMBER) + std::string(" ") + STR(MAX_NODE_NUMBER); };
	
private:
	std::string input;
	std::string temp;

	virtual const char* what() const
	{
		return temp.c_str();
	}
};

class e_too_many_args : public std::exception
{
	virtual const char* what() const
	{
		return "too many arguments";
	}
};

class e_too_few_args : public std::exception
{
	virtual const char* what() const
	{
		return "too few arguments";
	}
};

int parser();
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
				std::cout << "1" << std::endl;
				break;
			case modifyR:
				std::cout << "2" << std::endl;
				break;
			case printR:
				break;
			case printNode:
				_printNode(in_str_stream);
				break;
			case deleteR:
				_deleteR(in_str_stream);
				break;

			}
		}
		catch (const std::out_of_range&) // Undefined command
		{
			std::cerr << "invalid command" << std::endl;
			//std::cerr << e.what() << std::endl; // Print out exception type
		}
		catch (const std::exception& e) // Other
		{
			std::cerr << e.what() << std::endl; // Print out exception type
		}

		std::getline(std::cin, in_buf);
	}
	return -1;
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
			int temp = std::stoi(nodeid);
			if (temp < MIN_NODE_NUMBER || temp > MAX_NODE_NUMBER)
			{
				throw e_node_out_of_range(nodeid);
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
