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


const string Settlement::toString() const {
    return "Name: " + name + ", Type: " + typeToString(type);
}

int Settlement::getConstructLimit() const {
    return static_cast<int>(type) + 1;
}

// Function definition for converting enum to string
string Settlement::typeToString(SettlementType type) {
    switch (type) {
    case SettlementType::VILLAGE:
        return "VILLAGE";
    case SettlementType::CITY:
        return "CITY";
    case SettlementType::METROPOLIS:
        return "METROPOLIS";
    default:
        return "Unknown";
    }
}


