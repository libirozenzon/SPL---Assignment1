#include "Action.h"

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
    std:: cout<< "an error occured"+ errorMsg; // should i write it like this?
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
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
    return string(); //TODO
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string & settlementName, const string & selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy)
{


}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy* newPolicy= nullptr;
    // naive selection policy
    if(selectionPolicy=="nve"){
        newPolicy= new NaiveSelection();
    }
    // balances selection policy
    else if(selectionPolicy=="bal"){
        newPolicy= new BalancedSelection(0,0,0);
    }

    // economy selection policy
    else if(selectionPolicy=="eco"){
        newPolicy= new EconomySelection();
    }

    // sustainability selection policy
    else if(selectionPolicy=="env"){
        newPolicy= new SustainabilitySelection();
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
    return string(); //TODO
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string & settlementName, SettlementType settlementType) :settlementName(settlementName), settlementType(settlementType)
{
}

void AddSettlement::act(Simulation & simulation)
{
    if(simulation.isSettlementExists(settlementName)){
        error("settlemnt already exists");
    }
    else{
        Settlement* newSett= new Settlement(settlementName,settlementType);
    }
}

AddSettlement *AddSettlement::clone() const
{
    return nullptr;
}

const string AddSettlement::toString() const
{
    return string();
}

// you can start here Rotem
AddFacility::AddFacility(const string & facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
}

AddFacility *AddFacility::clone() const
{
    return nullptr;
}

const string AddFacility::toString() const
{
    return string();
}

PrintPlanStatus::PrintPlanStatus(int planId)
{
}

void PrintPlanStatus::act(Simulation &simulation)
{
}

PrintPlanStatus * PrintPlanStatus::clone() const
{
return nullptr;
}

const string PrintPlanStatus::toString() const
{
return string();
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
{
}

void ChangePlanPolicy::act(Simulation &simulation)
{
}

ChangePlanPolicy * ChangePlanPolicy::clone() const
{
return nullptr;
}

const string ChangePlanPolicy::toString() const
{
    return string();
}

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Simulation &simulation)
{
}

PrintActionsLog * PrintActionsLog::clone() const
{
return nullptr;
}

const string PrintActionsLog::toString() const
{
    return string();
}

Close::Close()
{
}

void Close::act(Simulation &simulation)
{
}

Close * Close::clone() const
{
return nullptr;
}

const string Close::toString() const
{
    return string();
}

BackupSimulation::BackupSimulation()
{
}

void BackupSimulation::act(Simulation &simulation)
{
}

BackupSimulation *BackupSimulation::clone() const
{
    return nullptr;
}

const string BackupSimulation::toString() const
{
    return string();
}

RestoreSimulation::RestoreSimulation()
{
}

void RestoreSimulation::act(Simulation &simulation)
{
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return nullptr;
}

const string RestoreSimulation::toString() const
{
    return string();
}