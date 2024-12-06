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
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this); 
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
return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
 return new BalancedSelection(*this); 
}

EconomySelection::EconomySelection() :lastSelectedIndex(-1)
{
}

const FacilityType & EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{   
    // assuming there's a facility that its category is ECONOMY
    bool isEconomy= false;
    int size= facilitiesOptions.size();
    int last;
     for (int i=lastSelectedIndex+1;!isEconomy; i= (i+ 1) % size){
        if(facilitiesOptions[i].getCategory()== FacilityCategory::ECONOMY){
            isEconomy=true;
            last=i;
        }
     }
     lastSelectedIndex=last;
     return facilitiesOptions[last];

}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this); 
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1)
{
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    // assuming there's a facility that its category is ENVIRONMENT
    bool isEnv= false;
    int size= facilitiesOptions.size();
    int last;
     for (int i=lastSelectedIndex+1;!isEnv; i= (i+ 1) % size){
        if(facilitiesOptions[i].getCategory()== FacilityCategory::ENVIRONMENT){
            isEnv=true;
            last=i;
        }
     }
     lastSelectedIndex=last;
     return facilitiesOptions[last];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

SustainabilitySelection * SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this); 
}
