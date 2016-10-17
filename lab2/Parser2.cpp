#include <string>
#include <queue>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

const int MAX_NODE_NUMBER = 5000;
const int MIN_NODE_NUMBER = 0;

enum command { insertR, modifyR, printR, printNode, deleteR }; // Commands list

class e_type : public std::exception
{
public:
    int priority;
    std::string e_msg;
    ~e_type() throw() {};

protected:
    e_type(int x, std::string msg) : priority(x), e_msg(msg){};
};

class cmp
{
public:
    bool operator() (e_type a, e_type b)
    {
        return a.priority < b.priority;
    }
};

class error_q : public std::exception
{
private:
    std::priority_queue<e_type, std::vector<e_type>, cmp> error_msg_queue;

public:

    void error_add(e_type excp)
    {
        error_msg_queue.push(excp);
    }

    bool no_error() const
    {
        return error_msg_queue.empty() ? true : false;
    }

    virtual const char* what() const throw()
    {
        return error_msg_queue.top().e_msg.c_str();
    }

    ~error_q() throw() {};
};

class e_invalid_arg : public e_type
{
public:
    e_invalid_arg(int pos) : e_type(6 + pos, "invalid argument") {}; // With priority 6, higher is more likely to get reported
    ~e_invalid_arg() throw() {};
};

class e_negative_r : public e_type
{
public:
    e_negative_r(int pos) : e_type(5 + pos, "negative resistance") {}; // With priority 5, higher is more likely to get reported
    ~e_negative_r() throw() {};
};

class e_node_out_of_range : public e_type
{
public:
    e_node_out_of_range(int pos, int in) : e_type(4 + pos, "node " + std::to_string(static_cast<long long>(in)) + " is out of permitted range 0-" + std::to_string(static_cast<long long>(MAX_NODE_NUMBER))) {}; // With priority 4, higher is more likely to get reported
    ~e_node_out_of_range() throw() {};
private:
	long long input;
	std::string temp;

	virtual const char* what() const throw()
	{
		return temp.c_str();
	}
};

class e_r_name_cannot_be_all : public e_type
{
public:
    e_r_name_cannot_be_all(int pos) : e_type(3 + pos, "resistor name cannot be the keyword \"all\"") {} // With priority 3, higher is more likely to get reported
    ~e_r_name_cannot_be_all() throw() {};
};

class e_both_terminal_no_connect : public e_type
{
public:
    e_both_terminal_no_connect(int pos, int in) : e_type(2 + pos, "both terminals of resistor connect to node " + std::to_string(static_cast<long long>(in))) {}; // With priority 2, higher is more likely to get reported
	~e_both_terminal_no_connect() throw() {};
};

class e_too_many_args : public e_type
{
public:
    e_too_many_args(int pos) : e_type(1 + pos, "too many arguments") {} // With priority 1, higher is more likely to get reported
    ~e_too_many_args() throw() {};
};

class e_too_few_args : public e_type
{
public:
    e_too_few_args(int pos) : e_type(0 + pos, "too few arguments") {} // With priority 0, higher is more likely to get reported
    ~e_too_few_args() throw() {};
};

class Rparser
{
private:

	void _insertR(std::vector<std::string>& in_str);
	void _modifyR(std::vector<std::string>& in_str);
	void _printR(std::vector<std::string>& in_str);
	void _printNode(std::vector<std::string>& in_str);
	void _deleteR(std::vector<std::string>& in_str);
public:
	void run(); // RUN FUNCTION
};

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

int main()
{
    Rparser A;
    A.run();
    return 0;
}
