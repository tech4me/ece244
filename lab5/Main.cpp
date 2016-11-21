#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"

void insert(std::vector<std::string>& in, TreeDB& DB);
void find(std::vector<std::string>& in, TreeDB& DB);
void remove(std::vector<std::string>& in, TreeDB& DB);
void printall(std::vector<std::string>& in, TreeDB& DB);
void printprobes(std::vector<std::string>& in, TreeDB& DB);
void removeall(std::vector<std::string>& in, TreeDB& DB);
void countactive(std::vector<std::string>& in, TreeDB& DB);
void updatestatus(std::vector<std::string>& in, TreeDB& DB);

int main()
{
    TreeDB db;
    std::string in_buf, in_command;
    std::cout << "> ";
    std::getline(std::cin, in_buf);
    while (!std::cin.eof())
    {
        std::stringstream in_buf_ss(in_buf);
        std::vector<std::string> in_str;
        std::string word;
        while (in_buf_ss >> word)
        {
            in_str.push_back(word); // Read input into a vector
        }
        in_command = *in_str.begin(); // The first word in vector
        if (in_command == "insert")
        {
            insert(in_str, db);
        }
        else if (in_command == "find")
        {
            find(in_str, db);
        }
        else if (in_command == "remove")
        {
            remove(in_str, db);
        }
        else if (in_command == "printall")
        {
            printall(in_str, db);
        }
        else if (in_command == "printprobes")
        {
            printprobes(in_str, db);
        }
        else if (in_command == "removeall")
        {
            removeall(in_str, db);
        }
        else if (in_command == "countactive")
        {
            countactive(in_str, db);
        }
        else if (in_command == "updatestatus")
        {
            updatestatus(in_str, db);
        }
        std::cout << "> ";
        std::getline(std::cin, in_buf);
        in_command = "";
    }
    return 0;
}

void insert(std::vector<std::string>& in, TreeDB& DB)
{
    std::string name;
    unsigned int ipaddress;
    bool status;
    auto it = in.begin(); // Command
    name = *++it; // name
    ipaddress = stoul(*++it); // ip
    if (*++it == "active") // status
    {
        status = true;
    }
    else
    {
        status = false;
    }
    DBentry * ptr = new DBentry(name, ipaddress, status);
    bool found = DB.insert(ptr);
    if (!found)
    {
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Error: entry already exists" << std::endl;
    }
}

void find(std::vector<std::string>& in, TreeDB& DB)
{
    std::string name;
    auto it = in.begin(); // Command
    name = *++it; // name
    DBentry* found = DB.find(name);
    if (found == NULL)
    {
        std::cout << "Error: entry does not exist" << std::endl;
    }
    else
    {
        std::cout << *found;
    }
}

void remove(std::vector<std::string>& in, TreeDB& DB)
{
    std::string name;
    auto it = in.begin(); // Command
    name = *++it; // name
    if (DB.remove(name))
    {
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Error: entry does not exist" << std::endl;
    }
}

void printall(std::vector<std::string>& in, TreeDB& DB)
{
    std::cout << DB;
}

void printprobes(std::vector<std::string>& in, TreeDB& DB)
{
    std::string name;
    auto it = in.begin(); // Command
    name = *++it; // name

    DBentry* found = DB.find(name);
    if (found == NULL)
    {
        std::cout << "Error: entry does not exist" << std::endl;
    }
    else
    {
        DB.printProbes();
    }
}

void removeall(std::vector<std::string>& in, TreeDB& DB)
{
    DB.clear();
    std::cout << "Success" << std::endl;
}

void countactive(std::vector<std::string>& in, TreeDB& DB)
{
    DB.countActive();
}

void updatestatus(std::vector<std::string>& in, TreeDB& DB)
{
    std::string name;
    bool status;
    auto it = in.begin(); // Command
    name = *++it; // name
    if (*++it == "active") // status
    {
        status = true;
    }
    else
    {
        status = false;
    }
    DBentry* found = DB.find(name);
    if (found == NULL)
    {
        std::cout << "Error: entry does not exist" << std::endl;
    }
    else
    {
        DB.find(name)->setActive(status);
        std::cout << "Success" << std::endl;
    }
}