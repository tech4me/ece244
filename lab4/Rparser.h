#ifndef RPARSER_H
#define RPARSER_H

#include "Resistor.h"
#include "Node.h"
#include "NodeList.h"
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>
#include <typeinfo>

static const int MIN_RES_NUMBER = 0;

enum command {insertR, modifyR, printR, printNode, deleteR, setV, unsetV, solve }; // Commands list
namespace enum_error_priority
{
	enum error_priority // The priority for error reporting
	{
		e_node_voltage_not_set            = 1000,
		e_r_name_not_found                = 999,
		e_r_name_exist                    = 998,
		e_nodeid_not_found                = 997,
		e_both_terminal_no_connect        = 996,
		e_invalid_arg                     = 10,
		e_negative_r                      = 9,
		e_r_name_cannot_be_all            = 8,
		e_too_many_args                   = 7,
		e_too_few_args                    = 6
	};
}

class e_type : public std::exception // Father class for the exception classes to inherient from
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
    bool operator() (e_type a, e_type b) // Compare priority in priority_queue
    {
        return a.priority < b.priority;
    }
};

class error_q : public std::exception // Class to print out error with highest priority
{
private:
    std::priority_queue<e_type, std::vector<e_type>, cmp> error_msg_queue;

public:

    void error_add(e_type excp) // Add a new error to the error queue
    {
        error_msg_queue.push(excp);
    }

    bool no_error() const // if there is no error to report
    {
        return error_msg_queue.empty() ? true : false;
    }

    virtual const char* what() const throw() // Error print
    {
        return error_msg_queue.top().e_msg.c_str();
    }

    ~error_q() throw() {};
};

class e_node_voltage_not_set : public e_type
{
public:
	e_node_voltage_not_set(int pos) : e_type(enum_error_priority::e_node_voltage_not_set + pos, "no nodes have their voltage set") {};
	~e_node_voltage_not_set() throw() {};
};

class e_r_name_not_found : public e_type
{
public:
    e_r_name_not_found(int pos, std::string name) : e_type(enum_error_priority::e_r_name_not_found + pos, "resistor " + name + " not found") {};
    ~e_r_name_not_found() throw() {};
};

class e_r_name_exist : public e_type
{
public:
    e_r_name_exist(int pos, std::string name) : e_type(enum_error_priority::e_r_name_exist + pos, "resistor " + name + " already exists") {};
    ~e_r_name_exist() throw() {};
};

class e_nodeid_not_found : public e_type
{
public:
	e_nodeid_not_found(int pos, int nodeid) : e_type(enum_error_priority::e_nodeid_not_found + pos, "node " + std::to_string(static_cast<long long>(nodeid)) + " not found") {};
	~e_nodeid_not_found() throw() {};
};

class e_invalid_arg : public e_type
{
public:
    e_invalid_arg(int pos) : e_type(enum_error_priority::e_invalid_arg, "invalid argument") {};
    ~e_invalid_arg() throw() {};
};

class e_negative_r : public e_type
{
public:
    e_negative_r(int pos) : e_type(enum_error_priority::e_negative_r, "negative resistance") {};
    ~e_negative_r() throw() {};
};

class e_r_name_cannot_be_all : public e_type
{
public:
	e_r_name_cannot_be_all(int pos) : e_type(enum_error_priority::e_r_name_cannot_be_all, "resistor name cannot be the keyword \"all\"") {};
    ~e_r_name_cannot_be_all() throw() {};
};

class e_both_terminal_no_connect : public e_type
{
public:
    e_both_terminal_no_connect(int pos, int in) : e_type(enum_error_priority::e_both_terminal_no_connect, "both terminals of resistor connect to node " + std::to_string(static_cast<long long>(in))) {};
	~e_both_terminal_no_connect() throw() {};
};

class e_too_many_args : public e_type
{
public:
	e_too_many_args(int pos) : e_type(enum_error_priority::e_too_many_args, "too many arguments") {};
    ~e_too_many_args() throw() {};
};

class e_too_few_args : public e_type
{
public:
	e_too_few_args(int pos) : e_type(enum_error_priority::e_too_few_args, "too few arguments") {};
    ~e_too_few_args() throw() {};
};

class Rparser
{
private:
	NodeList node_list;

    // Commands functions
	void _insertR(std::vector<std::string>& in_str);
	void _modifyR(std::vector<std::string>& in_str);
	void _printR(std::vector<std::string>& in_str);
	void _printNode(std::vector<std::string>& in_str);
	void _deleteR(std::vector<std::string>& in_str);
	//void _setV(std::vector<std::string>& in_str);
	//void _unsetV(std::vector<std::string>& in_str);
	//void _solve(std::vector<std::string>& in_str);
public:
    Rparser();
    ~Rparser();
	void run(); // RUN FUNCTION
};

#endif


