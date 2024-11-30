#pragma once
#include <string>
#include <vector>
#include <sstream> 
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;
        int getConstructLimit() const;
        static string typeToString(SettlementType type); 

    private:
            const string name;
            SettlementType type;
};