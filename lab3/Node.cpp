#include "Node.h"
#include <iostream>

Node::Node()
	: numRes(0)
{
    for (int i = 0; i < MAX_RESISTORS_PER_NODE; ++i) // Preset the resIDArray to -1s
    {
        resIDArray[i] = -1;
    }
}

Node::~Node()
{

}

bool Node::canAddResistor(int rIndex)
{
    if (numRes == 5)
        return false;
    else
        return true;
}

void Node::addResistor(int rIndex) // Add a resistor to node
{
    resIDArray[numRes] = rIndex;
    ++numRes;
}

void Node::print(int nodeIndex, Resistor** res_array_ptr) // Print out node
{
    std::cout << "Connections at node " << nodeIndex << ": " << numRes << " resistor(s)" << std::endl;
    for (int i = 0; i < numRes; ++i)
    {
        std::cout << "  " << **(res_array_ptr + resIDArray[i]) << std::endl;
    }

}