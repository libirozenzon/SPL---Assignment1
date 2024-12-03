#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;
#include <sstream>
#include <iostream>
#include <fstream> 
#include "Auxiliary.h" 

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        SelectionPolicy* readSelectionPolicy(const string&  policyType);
        void start();
        void TranslatingActions(const vector<string> &parsedAction);
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &FacilityName);
        bool isPlanExist(const int planID);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const int getPlanCounter() const; //added function
        void step();
        void close();
        void open();
        // Destructor
        ~Simulation();
        // Copy Constructor
        Simulation(const Simulation &other);
        // Copy Assignment Operator
        Simulation& operator=(const Simulation &other);
        // Move Constructor
        Simulation(Simulation &&other) ;
        // Move Assignment Operator
        Simulation& operator=(Simulation &&other) ;


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};