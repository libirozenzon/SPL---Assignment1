#include "Action.h"

BaseAction::BaseAction(): status(ActionStatus::ERROR), errorMsg("")
{
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status= ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status= ActionStatus::ERROR;
    this->errorMsg=std::move(errorMsg);
    std:: cout<< "an error occured"+ errorMsg; // should i write it like this?
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

std::string BaseAction::actionStatus() const {
    if (status==ActionStatus::COMPLETED) {
        return "COMPLETED";
        }
    else {
        return "ERROR";
    }
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps)
{
}

void SimulateStep::act(Simulation & simulation)
{
    for(int i=0; i<numOfSteps;i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    return "step" + std::to_string(numOfSteps) + actionStatus()+"\n"; 
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string & settlementName, const string & selectionPolicy) : BaseAction(),settlementName(settlementName), selectionPolicy(selectionPolicy)
{


}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy* newPolicy= nullptr;
    // naive selection policy
    if(selectionPolicy=="nve"){
        newPolicy= new NaiveSelection(); // need to delete
    }
    // balances selection policy
    else if(selectionPolicy=="bal"){
        newPolicy= new BalancedSelection(0,0,0); // need to delete
    }

    // economy selection policy
    else if(selectionPolicy=="eco"){
        newPolicy= new EconomySelection(); // need to delete
    }

    // sustainability selection policy
    else if(selectionPolicy=="env"){
        newPolicy= new SustainabilitySelection(); // need to delete
    }
    //error
    else{
        error("cannot create this plan");
    }

    if(newPolicy!=nullptr){
        // checking if settlement exists
        if(simulation.isSettlementExists(settlementName)){
            Settlement& newSett= simulation.getSettlement(settlementName);
            simulation.addPlan(newSett,newPolicy);
            complete();
        }

        else{
            error("cannot create this plan");
        }
        

    }
    
}

const string AddPlan::toString() const
{
    return "plan" + settlementName + selectionPolicy + actionStatus() + "\n";
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string & settlementName, SettlementType settlementType) :BaseAction(),settlementName(settlementName), settlementType(settlementType)
{
}

void AddSettlement::act(Simulation & simulation)
{
        Settlement* newSett= new Settlement(settlementName,settlementType); // need to delete
        bool isExist= simulation.addSettlement(newSett);
        if(isExist){
            error("settlemnt already exists");
        }
        else{
            complete();
        }

    
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
    return "settlement" + settlementName + std::to_string(static_cast<int>(settlementType)) + actionStatus();
}

AddFacility::AddFacility(const string & facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
BaseAction(),facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
        FacilityType newFac = FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
        if (simulation.addFacility(newFac)==true) {
            complete();
        }
        else {
            error("Facility already exists");
        }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    return "facility" + facilityName + std::to_string(static_cast<int>(facilityCategory))
     + std::to_string(price) + std::to_string(lifeQualityScore) + std::to_string(economyScore)
     + std::to_string(environmentScore) + actionStatus() + "\n";
}

PrintPlanStatus::PrintPlanStatus(int planId): BaseAction(), planId(planId)
{
}

void PrintPlanStatus::act(Simulation &simulation)
{ if (simulation.getPlanCounter()<=planId){
    error("Plan does not exists");
}
else {
    simulation.getPlan(planId).printStatus();
    complete();
}
}

PrintPlanStatus * PrintPlanStatus::clone() const
{
return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
return "planStatus" + std::to_string(planId) + actionStatus() +"\n";
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): BaseAction(),planId(planId), newPolicy(newPolicy)
{
}

void ChangePlanPolicy::act(Simulation &simulation)
{
    //check if working also with the special policy 
    // check if planID exists
    if(!simulation.isPlanExist(planId)){
        error("cannot change selection policy");
    }
    else{
        Plan & currPlan= simulation.getPlan(planId);
        std::string policy= currPlan.getSelectionPolicy();
        if(policy==newPolicy){
            error("cannot change selection policy");
        }
        else if(newPolicy=="bal"){
            // taking care of the underconstruction vector
            int LifeQualityScore=0;
            int EconomyScore=0;
            int EnvironmentScore=0;
            for (const Facility* facility : currPlan.getUnderConstuction()) {
                LifeQualityScore+=facility->getLifeQualityScore();
                EconomyScore+=facility->getEconomyScore();
                EnvironmentScore+=facility->getEnvironmentScore();
            }
            for (const Facility* facility2 : currPlan.getFacilities()) {
                LifeQualityScore+=facility2->getLifeQualityScore();
                EconomyScore+=facility2->getEconomyScore();
                EnvironmentScore+=facility2->getEnvironmentScore();
            }
            SelectionPolicy* currPolicy= new BalancedSelection(LifeQualityScore,EconomyScore,EnvironmentScore);
            currPlan.setSelectionPolicy(currPolicy);
            complete();
        }
    
        else{
            SelectionPolicy* currPolicy= simulation.readSelectionPolicy(newPolicy); //deal with ptr later
            if(currPolicy==nullptr){
                error("cannot change selection policy");
            }

            else{

                currPlan.setSelectionPolicy(currPolicy);
                complete();
            }
        }
    }
}


ChangePlanPolicy * ChangePlanPolicy::clone() const
{
return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
    return "ChangePlanPolicy" + std::to_string(planId) + newPolicy + actionStatus() + "\n";
}

PrintActionsLog::PrintActionsLog() : BaseAction()
{
}

void PrintActionsLog::act(Simulation &simulation)
{
    std::string ss = "";
    for (const BaseAction* act : simulation.getActionLog()) {
        ss = ss + act->toString();
    }
    std::cout << ss + "\n";
}

PrintActionsLog * PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
   return "PrintActionsLog" + actionStatus() + "\n";
}

Close::Close() : BaseAction()
{
}

void Close::act(Simulation &simulation)
{
    std::string ss = "";
    for (int i=0;i<simulation.getPlanCounter();i++) {
        Plan& plan = simulation.getPlan(i);
        ss = ss + plan.toString();
    }
    std::cout << ss;
    complete();
}

Close * Close::clone() const
{
return new Close(*this);
}

const string Close::toString() const
{
    return "close" + actionStatus() + "\n";
}

BackupSimulation::BackupSimulation() : BaseAction()
{
}

void BackupSimulation::act(Simulation &simulation)
{
    simulation.backup();
    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    return "backup" + actionStatus() + "\n";
}

RestoreSimulation::RestoreSimulation(): BaseAction()
{
}

void RestoreSimulation::act(Simulation &simulation)
{
    bool ifRestore= simulation.restore();
    if (ifRestore) {
        complete();
    }
    else{
        error("No backup available");
    } 
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    return "restore" + actionStatus() + "\n";
}
