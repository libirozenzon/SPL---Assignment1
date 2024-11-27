#include "SelectionPolicy.h"


NaiveSelection::NaiveSelection()
{
    lastSelectedIndex=-1; // no facility selected yet
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if(facilitiesOptions.empty()){
        throw std:: invalid_argument("facilitiesOptions is empty");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return " Last selected index = " + std::to_string(lastSelectedIndex);
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this); // no need for copy constructor
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : 
{
}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    // TODO: insert return statement here
}

const string BalancedSelection::toString() const
{
return string();
}

BalancedSelection * BalancedSelection::clone() const
{
return nullptr;
}

EconomySelection::EconomySelection()
{
}

const FacilityType & EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
// TODO: insert return statement here
}

const string EconomySelection::toString() const
{
    return string();
}

EconomySelection *EconomySelection::clone() const
{
    return nullptr;
}

SustainabilitySelection::SustainabilitySelection()
{
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    // TODO: insert return statement here
}

const string SustainabilitySelection::toString() const
{
    return string();
}

SustainabilitySelection * SustainabilitySelection::clone() const
{
return nullptr;
}
