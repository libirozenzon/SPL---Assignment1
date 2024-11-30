#include "SelectionPolicy.h"
#include <stdexcept>
#include <limits.h>
#include <algorithm>


NaiveSelection::NaiveSelection() :lastSelectedIndex(-1)
{
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
    return " Naive Selection: Last selected index = " + std::to_string(lastSelectedIndex);
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this); // no need for copy constructor
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
: LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore)
{
}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int min = INT_MAX;
    int i = 0;
    int j = -1;
    for (const auto& facility : facilitiesOptions){
        j++;
        int quality = LifeQualityScore + facility.getLifeQualityScore();
        int economy = EconomyScore + facility.getEconomyScore();
        int environment = EnvironmentScore + facility.getEnvironmentScore();
        int min_value = std::min(std::min(quality, economy), environment);
        int max_value = std::max(std::max(quality, economy), environment);
        int temp = max_value - min_value;
        if (temp < min) {
            min = temp;
            i = j;
        }
    }
    // updating the scores according to the chosen facility
    LifeQualityScore = LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
    EconomyScore = EconomyScore + facilitiesOptions[i].getEconomyScore();
    EnvironmentScore = EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
    return facilitiesOptions[i];
}

const string BalancedSelection::toString() const
{
return "BalancedSelection: Life Quality Score: " + std::to_string(LifeQualityScore) + ", Economy Score: " + std::to_string(EconomyScore) + ", Environment Score: " + std::to_string(EnvironmentScore);
}

BalancedSelection * BalancedSelection::clone() const
{
 return new BalancedSelection(*this); // no need for copy constructor
}

EconomySelection::EconomySelection() :lastSelectedIndex(-1)
{
}

const FacilityType & EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{   
    // assumig there's a facilioty that its category is ECONOMY
    bool isEconomy= false;
    int size= facilitiesOptions.size();
    int i=lastSelectedIndex+1;
     for (i;!isEconomy; i= (i+ 1) % size){
        if(facilitiesOptions[i].getCategory()== FacilityCategory::ECONOMY){
            isEconomy=true;
        }
     }
     lastSelectedIndex=i;
     return facilitiesOptions[i];

}

const string EconomySelection::toString() const
{
    return " Economy Selection: Last selected index = " + std::to_string(lastSelectedIndex);
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this); // no need for copy constructor
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1)
{
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    // assumig there's a facilioty that its category is ENVIRONMENT
    bool isEnv= false;
    int size= facilitiesOptions.size();
    int i=lastSelectedIndex+1;
     for (i;!isEnv; i= (i+ 1) % size){
        if(facilitiesOptions[i].getCategory()== FacilityCategory::ENVIRONMENT){
            isEnv=true;
        }
     }
     lastSelectedIndex=i;
     return facilitiesOptions[i];
}

const string SustainabilitySelection::toString() const
{
    return " Sustainability Selection: Last selected index = " + std::to_string(lastSelectedIndex);
}

SustainabilitySelection * SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this); // no need for copy constructor
}