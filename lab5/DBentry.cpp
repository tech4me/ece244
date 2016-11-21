#include "DBentry.h"
#include <iostream>

DBentry::DBentry()
    : IPaddress(0)
    , active(false)
{

}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active)
    : name(_name)
    , IPaddress(_IPaddress)
    , active(_active)
{

}

DBentry::~DBentry()
{

}


void DBentry::setActive(bool _active)
{
    active = _active;
}

std::string DBentry::getName() const
{
    return name;
}

unsigned int DBentry::getIPaddress() const
{
    return IPaddress;
}

bool DBentry::getActive() const
{
    return active;
}

std::ostream& operator<< (std::ostream& out, const DBentry& rhs)
{
    return out << rhs.getName() << " : " << rhs.getIPaddress() << " : " << (rhs.getActive() ? "active" : "inactive") << std::endl;
}