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
        e_node_out_of_range(int in) : input(in) { temp = std::string("node ") + std::to_string(input) + std::string(" is out of permitted range ") + STR(MIN_NODE_NUMBER) + std::string("-") + STR(MAX_NODE_NUMBER); };
        ~e_node_out_of_range() throw() {};
    private:
        long long input;
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
        e_both_terminal_no_connect(int in) : input(in) { temp = "both termials of resistor connect to node " + std::to_string(input); };
        ~e_both_terminal_no_connect() throw() {};
    private:
        long long input;
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
        in_command = "";
    }
    return -1;
}

void _insertR(std::stringstream& in_str_stream)
{
    std::string name;
    double resistance;
    int nodeid1;
    int nodeid2;

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }
    in_str_stream >> name;
    if (name == "all")
    {
        throw e_r_name_cannot_be_all();
    }

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }
    in_str_stream >> resistance;
    if (in_str_stream.fail())
    {
        throw e_invalid_arg();
    }
    if (in_str_stream.peek() != std::char_traits<char>::to_int_type(' ')) // Check for character following int
    {
        throw e_invalid_arg();
    }
    if (resistance < 0)
    {
        throw e_negative_r();
    }

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }
    in_str_stream >> nodeid1;
    if (in_str_stream.fail())
    {
        throw e_invalid_arg();
    }
    if (in_str_stream.peek() != std::char_traits<char>::to_int_type(' ')) // Check for character following int
	{
		throw e_invalid_arg();
	}
    if (nodeid1 < MIN_NODE_NUMBER || nodeid1 > MAX_NODE_NUMBER) // Check if nodeid in range
    {
        throw e_node_out_of_range(nodeid1);
    }

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }
    in_str_stream >> nodeid2;
    if (in_str_stream.fail())
    {
        throw e_invalid_arg();
    }
    if ((in_str_stream.peek() != std::char_traits<char>::to_int_type(' ')) && ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof())) // Check for character following int
	{
		throw e_invalid_arg();
	}
    if (nodeid2 < MIN_NODE_NUMBER || nodeid2 > MAX_NODE_NUMBER) // Check if nodeid in range
    {
        throw e_node_out_of_range(nodeid2);
    }

    if (nodeid1 == nodeid2)
    {
        throw e_both_terminal_no_connect(nodeid1);
    }

    if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_many_args();
    }

    std::cout << "Inserted: resistor " << name << " " << std::fixed << std::setprecision(2) << resistance << " Ohms " << nodeid1 << " -> " << nodeid2 << std::endl;
}

void _modifyR(std::stringstream& in_str_stream)
{
    std::string name;
    double resistance;

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }
    in_str_stream >> name;
    if (name == "all")
    {
        throw e_r_name_cannot_be_all();
    }

    if ((in_str_stream >> std::ws).peek() == std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_few_args();
    }

    in_str_stream >> resistance;
    if(in_str_stream.fail())
    {
        throw e_invalid_arg();
    }
    if (in_str_stream.peek() != std::char_traits<char>::to_int_type(' ') && ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof())) // Check for character following int
    {
        throw e_invalid_arg();
    }
    if (resistance < 0)
    {
        throw e_negative_r();
    }
    if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
    {
        throw e_too_many_args();
    }
    std::cout << "Modified: resistor " << name << " to " << std::setprecision(2) << std::fixed << resistance << " Ohms" << std::endl;
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
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Print: all resistors" << std::endl;
    }
    else
    {
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Print: resistor " << name << std::endl;
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
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Print: all nodes" << std::endl;
    }
    else
    {
        std::stringstream temp(nodeid);
        int int_nodeid;
        temp >> int_nodeid;
        if (temp.fail())
        {
            throw e_invalid_arg();
        }
        if (temp.peek() != std::char_traits<char>::eof()) // Check for digit sign
        {
            throw e_invalid_arg();
        }
        if (int_nodeid < MIN_NODE_NUMBER || int_nodeid > MAX_NODE_NUMBER) // Check if nodeid in range
        {
            throw e_node_out_of_range(int_nodeid);
        }
        if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Print: node " << nodeid << std::endl;
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
	if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Deleted: all resistors" << std::endl;
    }
    else
    {
        if ((in_str_stream >> std::ws).peek() != std::char_traits<char>::eof()) // Error checking
        {
            throw e_too_many_args();
        }
        std::cout << "Deleted: resistor " << name << std::endl;
    }
}
