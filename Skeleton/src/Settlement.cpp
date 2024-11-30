#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type)
{

}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}


const string Settlement::toString() const
{
    // convert enum to string
    std:: string strType= "";
    switch (type)
    {
    case SettlementType::VILLAGE:
        strType= "VILLAGE";
    case SettlementType::CITY:
       strType= "CITY";
    case SettlementType::METROPOLIS:
        strType= "METROPOLIS";
    default:
        strType= "Unknown";
    }

    return "Name: " + name + ", Type: " + strType;
}

int Settlement:: getConstructLimit() const{
    return static_cast<int>(getType()) + 1;
}


