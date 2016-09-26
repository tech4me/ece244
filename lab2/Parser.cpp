#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <exception>


enum command {insertR, modifyR, printR, printNode, deleteR};

int parser();

int main()
{
    parser();
    return 0;
}

int parser()
{
    std::string in_buf, in_command;

    std::map<std::string, command> command_map
    {
        {"insertR", insertR},
            {"modifyR", modifyR},
            {"printR", printR},
            {"printNode", printNode},
            {"deleteR", deleteR},
    };

    std::getline(std::cin, in_buf);
    while(!std::cin.eof())
    {
        std::stringstream in_str_stream(in_buf);
        in_str_stream >> in_command;
        try
        {
            switch(command_map.at(in_command))
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
                    break;
                case deleteR:
                    break;

            }
        }
        catch(const std::exception& e)
        {
        }

        std::getline(std::cin, in_buf);
    }    
}
