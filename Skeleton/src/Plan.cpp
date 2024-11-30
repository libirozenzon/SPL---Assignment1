#include "Plan.h"


Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions): plan_id(planId), settlement(settlement),
selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE) , facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0)
{
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if(selectionPolicy!=nullptr){
        delete this->selectionPolicy;
    }
    this->selectionPolicy=selectionPolicy; 
}

void Plan::step()
{
    // remember to delete whats necessary 
    int constLim= settlement.getConstructLimit();
    if(status== PlanStatus::AVALIABLE){
        while ((int) underConstruction.size() < constLim) {
            Facility* newFacility= new Facility (selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            addFacility(newFacility);
        }
    }
    for (auto iter = underConstruction.begin(); iter != underConstruction.end(); ) {
    Facility* facility = *iter;
    FacilityStatus newStat = facility->step();  // Step function on the facility

    if (newStat == FacilityStatus::OPERATIONAL) {
        // If the facility is operational, move it to 'facilities' vector
        facilities.push_back(facility);  // Add to 'facilities'
        iter = underConstruction.erase(iter);  // Remove from 'underConstruction'
    } else {
        ++iter;  // Move to the next facility in the loop
    }

    // change status if necessary
    if(constLim== (int)underConstruction.size()){
        status= PlanStatus::BUSY;
    }
    else{
        status= PlanStatus::AVALIABLE;
    }
}


}


const vector<Facility*>& Plan::getFacilities() const
{
 return facilities;
}

void Plan::addFacility(Facility *facility)
{
    //adding to underconstruction vector
    underConstruction.push_back(facility);
}

const string Plan::toString() const
{
    std:: stringstream ss;
    ss << "Plan ID: " << plan_id << "\n";
    ss << "Settlement Name: " << settlement.getName() << "\n";
    ss << "Settlement Type: " << Settlement::typeToString(settlement.getType()) << "\n"; // Use Settlement::typeToString
    ss << "Status: " << (status == PlanStatus::AVALIABLE ? "AVAILABLE" : "BUSY") << "\n";
    ss << "Life Quality Score: " << life_quality_score << "\n";
    ss << "Economy Score: " << economy_score << "\n";
    ss << "Environment Score: " << environment_score << "\n";
    ss << "Facilities: " << facilities.size() << " operational\n";
    ss << "Under Construction: " << underConstruction.size() << " facilities\n";

    return ss.str();
}

// copy constructor

Plan::Plan(const Plan& other) 
    : plan_id(other.plan_id),
      settlement(other.settlement),  // Settlement is assumed to be const & and doesn't require deep copy
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),  // Clone the selection policy
      status(other.status),
      facilityOptions(other.facilityOptions),  // References are shared, assuming facilityOptions doesn't change
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      facilities(),  // Explicitly initialize the facilities vector
      underConstruction()   // Initialize underConstruction here
    {
    
    // Deep copy facilities
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    // Deep copy underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

//destructor
Plan::~Plan() {
    delete selectionPolicy;  // Clean up selectionPolicy

    // Clean up dynamically allocated facilities
    for (Facility* facility : facilities) {
        delete facility;
    }
    for (Facility* facility : underConstruction) {
        delete facility;
    }
}
