#include "Simulation.h"


Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0)
{
    // opening the config file
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file at " << configFilePath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or lines starting with comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse the line into arguments
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);

        // Process each line based on its first word
        if (arguments[0] == "settlement") {
            // Format: settlement <settlement_name> <settlement_type>
            // make sure its 3 arguments
            if (arguments.size() != 3) {
                std::cerr << "Error: Invalid settlement line: " << line << std::endl;
                continue;
            }

            string settlementName = arguments[1];
            SettlementType type = static_cast<SettlementType>(std::stoi(arguments[2]));

            // Create and add the settlement with class function
            addSettlement(new Settlement(settlementName, type));
        }
        else if (arguments[0] == "facility") {
            // Format: facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
            if (arguments.size() != 6) {
                std::cerr << "Error: Invalid facility line: " << line << std::endl;
                continue;
            }

            string facilityName = arguments[1];
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(arguments[2]));
            int price = std::stoi(arguments[3]);
            int lifeqImpact = std::stoi(arguments[4]);
            int ecoImpact = std::stoi(arguments[5]);
            int envImpact = std::stoi(arguments[6]);

            // Create and add the facilityTypes to facilitiesOptions
            addFacility(FacilityType(facilityName, category, price, lifeqImpact, ecoImpact, envImpact)); // should i add new?
        }
        else if (arguments[0] == "plan") {
            // Format: plan <settlement_name> <selection_policy>
            if (arguments.size() != 3) {
                std::cerr << "Error: Invalid plan line: " << line << std::endl;
                continue;
            }

            string settlementName = arguments[1];
            string selectionPolicy = arguments[2];

            // Check if the settlement exists
            if (!isSettlementExists(settlementName)) {
                std::cerr << "Error: Settlement " << settlementName << " settlement does not exist." << std::endl;
                continue;
            }

            // Retrieve the settlement
            Settlement &settlement = getSettlement(settlementName);

            // Create the selection policy (you'll need to implement the logic for this)
            SelectionPolicy* policy = readSelectionPolicy(selectionPolicy);  // You'll need to implement this function

            // Add the plan
            addPlan(settlement, policy);
        }
        else {
            std::cerr << "Error: Unknown line type in config: " << line << std::endl;
        }
    }

    // Close the file after reading
    file.close();
}

SelectionPolicy* readSelectionPolicy(const string& policyType) {
    if (policyType == "nve") {
        // Create and return NaiveSelection policy
        return new NaiveSelection();
    }
    else if (policyType == "bal") {
        return new BalancedSelection(0,0,0);
    }
    else if (policyType == "eco") {
        // Create and return EconomySelection policy
        return new EconomySelection();
    }
    else if (policyType == "env") {
        // Create and return SustainabilitySelection policy
        return new SustainabilitySelection();
    }
    else {
        // If the policyType doesn't match, return nullptr or handle error
        return nullptr; // Or throw an exception if appropriate
    }
}

void Simulation::start()
{
}

void Simulation::addPlan(const Settlement & settlement, SelectionPolicy * selectionPolicy)
{
}

void Simulation::addAction(BaseAction *action)
{
}

bool Simulation::addSettlement(Settlement *settlement)
{
    return false;
}

bool Simulation::addFacility(FacilityType facility)
{
return false;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName)
{
    // TODO: insert return statement here
}

Plan &Simulation::getPlan(const int planID)
{
    // TODO: insert return statement here
}

void Simulation::step()
{
}

void Simulation::close()
{
}

void Simulation::open()
{
}


