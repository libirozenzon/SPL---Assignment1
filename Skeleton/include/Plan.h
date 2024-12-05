#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <sstream> 
#include <string> 
#include <iostream>
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const int planID, const Settlement &settlement,SelectionPolicy *selectionPolicy, PlanStatus status,const vector<FacilityType> &facilityOptions,int life_quality_score, int economy_score, int environment_score);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        int getPlanID();
        PlanStatus getStatus();
        string getSelectionPolicy();
        SelectionPolicy*  getSelectionPolicyByPtr();
        const Settlement& getSettlement() const;
        const vector<Facility*>& getUnderConstuction() const;
        void addFacility(Facility* facility);
        void addToFacilities(Facility* facility);
        void addToUnderConstruction(Facility* facility);
        const string toString() const;
        ~Plan();
        Plan(const Plan& other);
        Plan& operator=(const Plan &other) = delete;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};
