#include "Simulation.h"
#include <Action.h>
extern Simulation* backup;


Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
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
            if (arguments.size() != 7) {
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

            SelectionPolicy* policy = readSelectionPolicy(selectionPolicy);  // You'll need to implement this function
            if(policy==nullptr){
                std::cerr << "Error: selectionPolicy  " << selectionPolicy << " does not exist." << std::endl;
            }

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

SelectionPolicy* Simulation::readSelectionPolicy(const string& policyType) {
    if (policyType == "nve") {
        return new NaiveSelection();
    } else if (policyType == "bal") {
        return new BalancedSelection(0, 0, 0);
    } else if (policyType == "eco") {
        return new EconomySelection();
    } else if (policyType == "env") {
        return new SustainabilitySelection();
    } else {
        return nullptr; // Invalid policy type
    }
}


void Simulation::start()
{
    isRunning=true;
    std :: cout << "The simulation has started\n";
    while(isRunning){
        string input;
        std:: getline(std::cin,input);
        if(input=="close"){
            Close close = Close();
            close.act(*this);
            isRunning=false;
            std:: cout<< "simulation is finished";
        }
        else{
                vector<string> parsedAction= Auxiliary::parseArguments(input);
                TranslatingActions(parsedAction);

        }

    }
}

void Simulation:: TranslatingActions(const vector<string> &parsedAction){
    // step
    if(parsedAction[0]== "step"){
        SimulateStep newStep= SimulateStep(std::stoi(parsedAction[1]));
        newStep.act(*this);
        BaseAction* saveToLog= newStep.clone();
        actionsLog.push_back(saveToLog);
    }

    // plan
    if(parsedAction[0]== "plan"){
        AddPlan newPlan= AddPlan(parsedAction[1],parsedAction[2]);
        // in act- make sure there are no errors and print "cannot create this plan"
        newPlan.act(*this);
        BaseAction* saveToLog= newPlan.clone();
        actionsLog.push_back(saveToLog);

    }

    //settlement
    if(parsedAction[0]=="settlement"){
        SettlementType type;
        int numOftype= std::stoi(parsedAction[2]);
        if(numOftype==0){
            type= SettlementType::VILLAGE;
        }
        else if(numOftype==1){
            type= SettlementType::CITY;
        }
        else {
            type= SettlementType:: METROPOLIS;
        }
        // what if not 0/1/2- not handling it

        AddSettlement newSet= AddSettlement(parsedAction[1],type);
        newSet.act(*this);
        BaseAction* saveToLog= newSet.clone();
        actionsLog.push_back(saveToLog);

    }

    // facility 
    if(parsedAction[0]=="facility"){   
         FacilityCategory type;
        int numOftype= std::stoi(parsedAction[2]);
        if(numOftype==0){
            type= FacilityCategory::LIFE_QUALITY;
        }
        else if(numOftype==1){
            type= FacilityCategory::ECONOMY;
        }
        else {
            type= FacilityCategory::ENVIRONMENT;
        }
        // what if not 0/1/2- not handling it

        AddFacility newFac= AddFacility(parsedAction[1],type,std::stoi(parsedAction[3]),std::stoi(parsedAction[4]),std::stoi(parsedAction[5]),std::stoi(parsedAction[6]));
        newFac.act(*this);
        BaseAction* saveToLog= newFac.clone();
        actionsLog.push_back(saveToLog);

    }

    // print plan status
    if(parsedAction[0]== "planStatus"){
        PrintPlanStatus newPlanStat= PrintPlanStatus(std::stoi(parsedAction[1]));
        newPlanStat.act(*this);
        BaseAction* saveToLog= newPlanStat.clone();
        actionsLog.push_back(saveToLog);

    }

    //change policy
    if(parsedAction[0]== "changePolicy"){
        ChangePlanPolicy newPlanPol= ChangePlanPolicy(std::stoi(parsedAction[1]), parsedAction[2]);
        newPlanPol.act(*this);
        BaseAction* saveToLog= newPlanPol.clone();
        actionsLog.push_back(saveToLog);
    }

    // print actions log 
    if(parsedAction[0]== "log"){
        PrintActionsLog newActLog= PrintActionsLog();
        newActLog.act(*this);
        BaseAction* saveToLog= newActLog.clone();
        actionsLog.push_back(saveToLog);
    }

    //backup
    else if (parsedAction[0] == "backup")
    {
        BackupSimulation newBackup= BackupSimulation();
        newBackup.act(*this);
        BaseAction* saveToLog= newBackup.clone();
        actionsLog.push_back(saveToLog);

    }

    // restore

    else if(parsedAction[0]=="restore"){
        RestoreSimulation newResSim= RestoreSimulation();
        newResSim.act(*this);
        BaseAction* saveToLog= newResSim.clone();
        actionsLog.push_back(saveToLog);
    }
    
}

void Simulation::addPlan(const Settlement & settlement, SelectionPolicy * selectionPolicy)
{

    plans.push_back(Plan(planCounter,settlement,selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    // checking if settlement exists
    const std::string& name= settlement->getName(); 
    if(!isSettlementExists(name)){
        settlements.push_back(settlement);
        return false;
    }
    else{
        return true;
    }

}

bool Simulation::addFacility(FacilityType facility)
{
    const std::string& name= facility.getName(); 
    if(!isFacilityExists(name)){
    facilitiesOptions.push_back(facility);
        return true;
    }
    else{
        return false;
    }

}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for(Settlement *currSet: settlements){
        if((currSet->getName())==settlementName){
            return true;
        }

    }
        return false;
}

bool Simulation:: isFacilityExists(const string &FacilityName){
    for (const FacilityType& facility : facilitiesOptions) {
    const std::string& name = facility.getName();
    if (name == FacilityName) {
        return true;
    }
    }
    return false;

}

bool Simulation:: isPlanExist(const int planID){
    for(Plan &curr: plans){
        const int id= curr.getPlanID();
        if(id==planID){
            return true;
        }
    }
    return false;
}


// need to do try+catch

Settlement &Simulation::getSettlement(const string &settlementName)
{
        for(Settlement *currSet: settlements){
        if((currSet->getName())==settlementName){
            return *currSet;
        }
        }
       std::cerr << "Warning: Settlement not found.\n";
       return *settlements[0];

    }



Plan &Simulation::getPlan(const int planID)
{
        return plans[planID];
}

void Simulation::step()
{
    for(Plan& curr:plans){
        curr.step();
    }
}

const int Simulation::getPlanCounter() const
{
    return planCounter;
}

vector<BaseAction *> &Simulation::getActionLog()
{
    return actionsLog;
}

void Simulation::Backup()
{
    if (backup!=nullptr) {
        delete backup;
    }
    else {
        backup = new Simulation(*this);
    }
}
bool Simulation::restore()
{
    if (backup == nullptr)
    {
        return false;
    }
    *this = *backup;
    return true;
}

void Simulation::close()
{
    for(Plan& curr:plans){
        std :: cout << curr.toString();
    }
    isRunning=false;
}

void Simulation::open()
{
    isRunning=true;
}

// destructor
Simulation::~Simulation() {
    // Delete all settlements (pointers)
    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    // Delete all actions (pointers)
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    plans.clear(); // plans has a destructor itself

    // Facilities are not dynamically allocated, so no need to delete individual FacilityType objects.
    facilitiesOptions.clear();
}

// copy constructor

Simulation::Simulation(const Simulation& other) 
    : isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions() {
    // Deep copy of settlements
    for (const Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    // Deep copy of actions
    for (const BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());  // Assuming BaseAction has a virtual clone method
    }

    // Deep copy of plans
    // for (const Plan& plan : other.plans) {
    //     plans.push_back(plan);  
    // 




    for (const FacilityType& facility : other.facilitiesOptions) {
    facilitiesOptions.push_back(facility); // Uses copy constructor
    }

    for( Plan plan: other.plans){
        Settlement& newSet= getSettlement(plan.getSettlement().getName());
        Plan newPlan (plan.getPlanID(),newSet, (plan.getSelectionPolicyByPtr())->clone(),plan.getStatus() ,facilitiesOptions,plan.getlifeQualityScore(),plan.getEconomyScore(),plan.getEnvironmentScore()); 
        plans.emplace_back(newPlan);
        for(Facility* facility:plan.getFacilities()){
            newPlan.addToFacilities(facility->clone());
        }

        for(Facility* facility:plan.getUnderConstuction()){
            newPlan.addToUnderConstruction(facility->clone());
        }
    }


}

//assignment operator

Simulation& Simulation::operator=(const Simulation& other) {
    if (this == &other) {
        return *this;  // Self-assignment protection
    }

    // Clean up current resources
    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    plans.clear();

    // Deep copy from other
    isRunning = other.isRunning;
    planCounter = other.planCounter;

    for (const Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    for (const BaseAction* action : other.actionsLog) {
         actionsLog.push_back(action->clone());
     }

    // planCounter=0;
    // for( Plan plan: other.plans){
    //     Settlement& newSet= getSettlement(plan.getSettlement().getName());
    //     plans.emplace_back(planCounter,newSet, (plan.getSelectionPolicyByPtr())->clone(),facilitiesOptions);
    //     planCounter++;
    // }


    for (const FacilityType& facility : other.facilitiesOptions) {
     facilitiesOptions.push_back(facility); // Uses copy constructor
    }

    // Deep copy of plans
    // for (const Plan& plan : other.plans) {
    //     plans.push_back(plan); 
    // } 

    for( Plan plan: other.plans){
        Settlement& newSet= getSettlement(plan.getSettlement().getName());
        Plan newPlan (plan.getPlanID(),newSet, (plan.getSelectionPolicyByPtr())->clone(),plan.getStatus() ,facilitiesOptions,plan.getlifeQualityScore(),plan.getEconomyScore(),plan.getEnvironmentScore()); 
        plans.emplace_back(newPlan);
        for(Facility* facility:plan.getFacilities()){
            newPlan.addToFacilities(facility->clone());
        }

        for(Facility* facility:plan.getUnderConstuction()){
            newPlan.addToUnderConstruction(facility->clone());
        }
    }

    return *this;
}

// move constructor

Simulation::Simulation(Simulation&& other)
    : isRunning(other.isRunning), 
      planCounter(other.planCounter), 
    actionsLog(std::move(other.actionsLog)),
    plans(std::move(other.plans)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)) {
    other.isRunning = false;
    other.planCounter = 0;
}

// move assignment operator

Simulation& Simulation::operator=(Simulation&& other)  {
    if (this == &other) {
        return *this;  // Self-assignment protection
    }

    // Clean up current resources
    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    plans.clear();

    // Move resources from other
    isRunning = other.isRunning;
    planCounter = other.planCounter;

    settlements = std::move(other.settlements);
    actionsLog = std::move(other.actionsLog);
    plans = std::move(other.plans);
    facilitiesOptions = std::move(other.facilitiesOptions);

    other.isRunning = false;
    other.planCounter = 0;

    return *this;
}



