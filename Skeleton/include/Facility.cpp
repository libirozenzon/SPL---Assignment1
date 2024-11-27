#include "Facility.h"


FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) 
: name(name), 
category(category), 
price(price), 
lifeQuality_score(lifeQuality_score), 
economy_score(economy_score), 
environment_score(environment_score) 
{
}

const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return economy_score;
}

int FacilityType::getEconomyScore() const
{
return environment_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
:FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), 
 settlementName(settlementName),  status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price)
{ 
}

Facility::Facility(const FacilityType &type, const string &settlementName)
: FacilityType(type), settlementName(settlementName),  status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost())
{
}

const string & Facility::getSettlementName() const
{
    return name;
}

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

FacilityStatus Facility::step()
{
    if (status==FacilityStatus::UNDER_CONSTRUCTIONS) {
        timeLeft--;
        if (timeLeft==0) {
            status=FacilityStatus::OPERATIONAL;
        }
    }

    return status;
}

void Facility::setStatus(FacilityStatus status)
{
    this->status=status; //creates copy of the status
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

string facilityStatusToString(FacilityStatus status) {
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS: return "Under Construction";
        case FacilityStatus::OPERATIONAL: return "Operational";
        // Add other statuses as necessary
        default: return "Unknown";
    }
}

string facilityCategoryToString(FacilityCategory category) {
    switch (category) {
        case FacilityCategory::LIFE_QUALITY: return "Life Quality";
        case FacilityCategory::ECONOMY: return "Economy";
        case FacilityCategory::ENVIRONMENT: return "Environment";
        default: return "Unknown";
    }
}

const string Facility::toString() const
{
    return "Facility Name: " + getName() + "\n" +  // Name from FacilityType
           "Settlement: " + getSettlementName() + "\n" +  // Settlement name
           "Category: " + facilityCategoryToString(category) + "\n" +  // Facility category
           "Price: " + std::to_string(getCost()) + "\n" +  // Price
           "Life Quality Score: " + std::to_string(getLifeQualityScore()) + "\n" +
           "Economy Score: " + std::to_string(getEconomyScore()) + "\n" +
           "Environment Score: " + std::to_string(getEnvironmentScore()) + "\n" +
           "Status: " +  facilityStatusToString(status) + "\n" +  // Facility status
           "Time Left: " + std::to_string(getTimeLeft());
}


