#ifndef RPARSER_H
#define RPARSER_H

#include "Resistor.h"
#include "Node.h"
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>
#include <typeinfo>

static int MAX_NODE_NUMBER = 0;
static const int MIN_NODE_NUMBER = 0;
static const int MIN_RES_NUMBER = 0;

enum command { maxVal, insertR, modifyR, printR, printNode, deleteR }; // Commands list

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

class e_maxval_smaller : public e_type
{
public:
    e_maxval_smaller(int pos) : e_type(1000 + pos, "maxVal arguments must be greater than 0") {}; // With priority 1000, higher is more likely to get reported
    ~e_maxval_smaller() throw() {};
};

class e_r_name_not_found : public e_type
{
public:
    e_r_name_not_found(int pos, std::string name) : e_type(999 + pos, "resistor " + name + " not found") {}; // With priority 999, higher is more likely to get reported
    ~e_r_name_not_found() throw() {};
};

class e_r_array_full : public e_type
{
public:
    e_r_array_full(int pos) : e_type(997 + pos, "resistor array is full") {}; // With priority 997, higher is more likely to get reported
    ~e_r_array_full() throw() {};
};

class e_node_full : public e_type
{
public:
    e_node_full(int pos) : e_type(996 + pos, "node is full") {}; // With priority 996, higher is more likely to get reported
    ~e_node_full() throw() {};
};

class e_r_name_exist : public e_type
{
public:
    e_r_name_exist(int pos, std::string name) : e_type(995 + pos, "resistor " + name + " already exists") {}; // With priority 995, higher is more likely to get reported
    ~e_r_name_exist() throw() {};
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
    Node** node_array_ptr;
    Resistor** res_array_ptr;
    bool maxval_is_set;
    int node_n;
    int res_n;
    int current_node_n;
    int current_res_n;

    void _maxVal(std::vector<std::string>& in_str);
	void _insertR(std::vector<std::string>& in_str);
	void _modifyR(std::vector<std::string>& in_str);
	void _printR(std::vector<std::string>& in_str);
	void _printNode(std::vector<std::string>& in_str);
	void _deleteR(std::vector<std::string>& in_str);
public:
    Rparser();
    ~Rparser();
	void run(); // RUN FUNCTION
};

#endif


