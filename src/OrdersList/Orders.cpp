#include <iostream>
#include <cstdlib>

#include "Orders.h"
#include "../Player/Player.h"

using namespace std;

int Order::countOrderID = 0;        //start counter for orders at 0
bool Order::pGivenCardThisTurn = false;

Order::Order() {
    orderID = incrementCount();
    addDescription();
    setValid(false);
};

Order::Order(const Order* o) {
    orderID = incrementCount();
    addDescription();
    setValid(false);
    cout <<"hi grom order copy constructor\n";
};

//TODO:
Order::~Order(){
    cout<< "Order ID#: " << getOrderID() << " is deleted.\n\n";
}

Order* Order::clone() const {
    return new Order(this);
}

/**
* Checks if valid for execution, invalid orders can exist
*/
bool Order::validate(){ return false;}

bool Order::getValid() {
    return this->valid;
};

void Order::setValid(bool v) {
    this->valid = v;
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Order::execute(State* currentState) {
    if (currentState->getStateName().compare("executeorders") == 0 && getValid() == 1) {
        cout << "Executing order #" << getOrderID() << " ...\n";
    }else cout << "Can NOT execute order #" << getOrderID() << " ...\n";
};

void Order::execute(){};
/**
 * Override the stringToLog method to print about the order
*/
string Order::stringToLog() {
    return "\n----------------------------------------- Logger -----------------------------------------\n Order ID: " + to_string(orderID) + "\n Order descroption: " + getDescription() + "\n------------------------------------------------------------------------------------------\n";
};

int Order::incrementCount() {
    return ++countOrderID;
};

int Order::getCount() {
    return countOrderID;
};


void Order::setOrderID(int id) {
    orderID = id;
}

int Order::getOrderID() {
    return orderID;
}

void Order::addDescription() {
    this->description = "(Order Class)";
}

string Order::getDescription() {
    return this->description;
}

string Order::getOrderName(){return "Order";}

void Order::setPlayerGivenCardThisTurn(bool b){pGivenCardThisTurn = b;}

bool Order::getPlayerGivenCardThisTurn() { return pGivenCardThisTurn; }

bool Order::pOwnsTerr(Player* p, Territory* t){
    return (t->getOwnerId() == p->getID());
}

bool Order::terrIsAdjP(Territory *t1, Territory *t2){
    for (auto adj : t1->getAdjacencyList()) {
        // true: if valid and territory name matches name in players list territories
        if (adj->getName().compare(t2->getName()) == 0) {
            return true;
        }
    }
    return false;
}

bool Order::terrHasOwner(Territory* t){
    if(t->getOwnerId()<=0)
        return false;
    return true;
}

bool Order::pIsInExecuteState(Player *p, string orderName){
    if (!(p->getState()->getStateName().compare("executeorders") == 0)) {
        cout << "Can NOT execute " << orderName  
            << " | Player #" << p->getID() << " is not in Execute Orders State\n";
        return false;
    }
    return true;
}


/**
* @brief Stream insertion operator
* Will ouptut to console everytime "cout <<" is used on Order Object
* like toString but directly from cout<<
*/
ostream& operator << (ostream& out, Order* o)
{
    /*
    * Lambda expresion: If true return "true", b/c c++ will return 1
    */
    auto printBoolValue = [](bool b) { if (b) return "true"; else return "false"; };

    out << o->getOrderName()
        << " | ID: #" << o->getOrderID()
        << " | Valid: " << printBoolValue(o->getValid()) << "\n\n";
    return out;
}

/************************************************************ Deploy **************************************************************/
Deploy::Deploy() {
    this->addDescription();
}

Deploy::Deploy(Player* p, Territory* terrToDeploy, int numReinToDeploy) {
    this->p = p;
    this->terrToDeploy = terrToDeploy;
    this->numReinToDeploy = numReinToDeploy;
    setValid(validate());
}

Deploy::Deploy(const Deploy *d) {
    setOrderID(getCount());
    this->addDescription();
}

Deploy* Deploy::clone() const {
    return new Deploy(this);
}

/**
 * Override the stringToLog method to print about the order
*/
string Deploy::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nDeploy executed | Player #" + to_string(p->getID()) + " now has " + to_string(terrToDeploy->getArmyCount()) + " reinforcments in Territory " + this->terrToDeploy->getName() + "\n\n------------------------------------------------------------------------------------------\n\n";
};

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Deploy::execute(State* current) {
    if (current->getStateName().compare("executeorders") == 0 && validate() == 1){
        cout << "Executing (Deploy) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Deploy) order #" << getOrderID() << " ...\n";
};

/**
 * @brief Checks if Deploy is valid and Player owns Territory to deploy,
 * then adds reinforcments.
 * 
 */
void Deploy::execute() {
    if (!pIsInExecuteState(p, getOrderName())) { return; } // player is NOT in execute order state
    if(getValid()){
        p->subtractReinforcemnts(numReinToDeploy);
        terrToDeploy->addToArmyCount(numReinToDeploy);
        cout << "Deploy executed | Player #" << p->getID() << " now has "
            << terrToDeploy->getArmyCount() << " reinforcments in Territory "
            << this->terrToDeploy->getName() <<"\n";
            notify(this);


    }
    else if(this->numReinToDeploy==0){
        cout << "Can NOT execute Deploy " << this->terrToDeploy->getName()
             << " | Deploy has 0 reinforcments\n";
    }else if(!pOwnsTerr(p,terrToDeploy)){
        cout << "Can NOT execute Deploy " << this->terrToDeploy->getName()
            << " | Player #" << p->getID() << " does NOT own territory\n";
    }
    else if (!pHasEnoughRein()) { //player has enough reinforcemnts
        cout << "Can NOT execute Deploy " << this->terrToDeploy->getName()
             << " | Player #" << p->getID() << " does NOT enough have reinforcments\n";
    }
}

/**
 * @brief return true and Deploy is valid and Player owns Territory to deploy
 */
bool Deploy::validate(){
    if (this->numReinToDeploy == 0)
        return false;
    if (!pHasEnoughRein()) { //player has enough reinforcemnts
        return false;
    }
    if(pOwnsTerr(p, terrToDeploy))
        return true;
    return false;
}

bool Deploy::pHasEnoughRein(){ return (p->getReinforcement() - numReinToDeploy >= 0);}

void Deploy::addDescription() {
    this->description = "(Deploy) Move a certain number of army units from the current player's \n"
        "            reinforcement pool to one of the current player's territories.";
}

string Deploy::getDescription() {
    return this->description;
}

string Deploy::getOrderName() { return "Deploy"; }

/************************************************************ Advance **************************************************************/
Advance::Advance() {
    this->addDescription();
}

Advance::Advance(const Advance* a) {
    setOrderID(getCount());
    this->addDescription();
}

Advance::Advance(Player* p, Territory* terrSource, Territory* terrTarget, int numReinToAdvnce) {
    this->p = p;
    this->terrSource = terrSource;
    this->terrTarget = terrTarget;
    this->numReinToAdvnce = numReinToAdvnce;
    setValid(validate());
    setPlayerGivenCardThisTurn(false);
}

Advance* Advance::clone() const {
    return new Advance(this);
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Advance::execute(State* current) {
    if (current->getStateName().compare("executeorders")==0 && getValid() == 1){
        cout << "Executing (Advance) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Advance) order #" << getOrderID() << " ...\n";
}

/**
 * @brief of player owns source territory and target is enemy territory
 * will attack target territory. If source wins, raiming forces will occupy target
 * and ownership changed to attacking plaver. If source loses (army units depleted 
 * to zero) attacking player keeps ownership of source. If player owns source will transfer
 * army units from source to target.
 * 
 */
void Advance::execute() {
    if(!pIsInExecuteState(p,getOrderName())){ return; } // player is NOT in execute order state
    if(this->getValid()){ // advance is valid
        notify(this);
        if(pOwnsTerr(p,this->terrTarget)){ // player owns target
           if(terrSource->getArmyCount() < numReinToAdvnce){ //check source has enough reinforcments to give to target
               cout << "Can NOT execute Advance " << this->terrSource->getName()
                    << " | The number of Army units to Advance is greater than Source Terriotory has. \n";
                return ; //exit function
           }
            terrSource->subFromArmy(numReinToAdvnce);
            terrTarget->addToArmyCount(numReinToAdvnce);
            cout << "Advance executed (player owns both Territories) | "
                <<terrSource->getName() << " has army of " << terrSource->getArmyCount() <<", "
                << terrTarget->getName() <<" has army of " << this->terrTarget->getArmyCount() << "\n";
        
        } else if (!pOwnsTerr(p, this->terrTarget)) { // player does not own target
            int winner = battle();
            cout << "...The battle has ended printing both Terriories after battle...\n"
                << terrSource
                << terrTarget;
            if(winner == 1){
                occupyConqueredTerr();
                cout << "Advance executed (player attacks target) | player " <<p->getID() 
                    << " has occupied " << this->terrTarget->getName() << " with " 
                    << this->terrSource->getArmyCount() << " army ";
                //TODO: each player can not receive more than one card per turn, maybe check if any more advnces in OL per person
                if(!Order::getPlayerGivenCardThisTurn()){ //if not given card this turn  
                    string newCard = givePLayerNewCard();
                    cout << "| Player given " << newCard <<" card";
                    Order::setPlayerGivenCardThisTurn(true);
                }else{
                    cout << "| Player already given card. ";
                }
                cout<<"\n";
            }
            if (winner == 0){
                cout << "Advance executed (player attacks target) | player " << p->getID()
                    << " has NOT occupied " << this->terrTarget->getName() << " and has 0 army in "
                    << this->terrSource->getName() <<"\n";


                //NOTE: leave owners as is, can only occupy after successful Advance attacker
            } 
        }
    }
    else {
        cout << "Can NOT execute Advance " << this->terrSource->getName()
        << " | Player does not own source \n";
    }
}

/**
 * Override the stringToLog method to print about the order
*/
string Advance::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nAdvance executed (player attacks target) | player " + to_string(p->getID()) +  " has NOT occupied " + this->terrTarget->getName() + " and has 0 army in " + this->terrSource->getName() + "\n\n------------------------------------------------------------------------------------------\n\n";
};

/**
 * @brief Valid if Territory is adjacant and player owns Source Territory
 */
bool Advance::validate() {
    if(terrIsAdjP(this->terrSource,this->terrTarget) && pOwnsTerr(p,this->terrSource))
        return true;
    return false;
}

/**
* Simulates battle between Source and Targer Territories, source attacks first. 
* RoundRobin style Source and Target will have a 60% adn 70% chance
* of winning an attack or defend, and the resulting army will lose 1
* of their army forces until one territory reaches 0 armies. 
* @return int 1 if source wins (target army = 0), 0 target wins (source army = 0)
*          if 2, error;
*/
int Advance::battle(){
    cout <<"... Enetering battle ...\n";
    srand((unsigned)time(NULL)); // set seed for random number
    while(this->terrSource->getArmyCount()>0 && this->terrTarget->getArmyCount()>0){ // stop when one Territory has 0 army
        int random = (rand() % 10) + 1; // random number between 1-10
        if(random <= 6) // source attack succesful
            this->terrTarget->subFromArmy(1); // -1 army from target  
        
        random = (rand() % 10) + 1; // call agan for target defence
        if(random <= 7) // defend succesffy 
            this->terrSource->subFromArmy(1); // -1 army from source
    }
    if(this->terrTarget->getArmyCount() == 0)
        return 1;
    if (this->terrSource->getArmyCount() == 0)
        return 0;
    return 2;
}

/**
* Move all army units from Source Territory to Target Territory, 
* and declare attacking Player ownership of Target Territory. 
*/
void Advance::occupyConqueredTerr(){
    this->terrTarget->setArmyCount(this->terrSource->getArmyCount()); // move source army into target Territory
    p->addTerritory(terrTarget);// ownership transfer from target to source
}

string Advance::givePLayerNewCard() {
    srand((unsigned)time(NULL)); // set seed for random number
    int r = (rand() % 4) + 1; // random number between 1-5
    
    string cardName;
    if(r == 1)
        cardName = "Bomb";
    if(r == 2)
        cardName = "Blockade";
    if(r == 3)
        cardName = "Airlift";
    if(r == 4)
        cardName = "Deplomacy";
    
    Card* c = new Card(cardName);
    p->getHand()->addCard(c);
    return cardName;
}

void Advance::addDescription() {
    this->description = "(Advance) Move a certain number of army units from one territory \n"
            "            (source territory) to another territory (target territory).";
}

string Advance::getDescription() {
    return this->description;
}

string Advance::getOrderName() { return "Advance"; }

Territory* Advance::getTerrTarget(){ return this->terrTarget;}

/************************************************************ Bomb **************************************************************/
Bomb::Bomb() {
    this->addDescription();
}

Bomb::Bomb(const Bomb* b) {
    setOrderID(getCount());
    this->addDescription();
}

Bomb::Bomb(Player* p, Territory* terrTarget){
    this->p = p;
    this->terrTarget = terrTarget;
    setValid(validate());
}

Bomb* Bomb::clone() const{
    return new Bomb(this);
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Bomb::execute(State* current) {
    if (current->getStateName().compare("executeorders")==0 && getValid() == 1){
        cout << "Executing (Bomb) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Bomb) order #" << getOrderID() << " ...\n";
}

/**
 * @brief if target territory is owner by another player, the army units
 * stationed will be cut down to half. If odd number of army units, division 
 * is rounded up.  
 * 
 */
void Bomb::execute(){
    if (!pIsInExecuteState(p, getOrderName())) { return; } // player is NOT in execute order state
    if(getValid()){
        halfArmyUnits(this->terrTarget);
        cout << "Bomb executed | "
            << terrTarget->getName() << " army is havled, and is now " 
            << terrTarget->getArmyCount() << "\n";
        notify(this);
    }
    if(!terrHasOwner(terrTarget)){
        cout << "Bomb can NOT be executed | "
            << terrTarget->getName()
            << " does not have an owner \n";
    }
    else if(p->ownsTerritory(this->terrTarget)){
        cout << "Bomb can NOT be executed | "
            << "player owns " <<terrTarget->getName() <<"\n";
    }
    else if(!terrTargetIsAdjP()) {
        cout << "Bomb can NOT be executed | "
            << "player has no adjacent territories to" << terrTarget->getName()
            << "\n";
    }
}

/**
 * Override the stringToLog method to print about the order
*/
string Bomb::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\n Bomb executed | " + terrTarget->getName() + " army is havled, and is now " + to_string(terrTarget->getArmyCount()) + "\n\n------------------------------------------------------------------------------------------\n\n";
};

bool Bomb::validate(){
    if(p->ownsTerritory(this->terrTarget))
        return false;
    if(!terrTargetIsAdjP())
        return false;
    if(!terrHasOwner(terrTarget))
        return false;
    return true;
}

bool Bomb::terrTargetIsAdjP(){
    for (auto t : p->getTerritories()){
        for (auto adj: t->getAdjacencyList()){
            if (adj->getName().compare(this->terrTarget->getName()) == 0)
                return true;
        }
    }
    return false;
}

void Bomb::halfArmyUnits(Territory *t){
    int armySize = t->getArmyCount();
    if(armySize%2 == 0){
        armySize = armySize / 2;
    }else{
        armySize = (armySize / 2) + 1; //round upp
    }
    t->setArmyCount(armySize);
}

void Bomb::addDescription() {
    this->description = "(Bomb) Destroy half of the army units located on a target territory. \n"
        "            This order can only be issued if a player has the bomb card in their hand.";
}

string Bomb::getDescription() {
    return this->description;
}

string Bomb::getOrderName() { return "Bomb"; }

/************************************************************ Blockade **************************************************************/
Blockade::Blockade() {
    this->addDescription();
}

Blockade::Blockade(const Blockade* a) {
    setOrderID(getCount());
    this->addDescription();
}
/**
 * @brief Needs Neutral player! Can not be zero, pick random number 999
 * 
 * @param p 
 * @param pNeutral 
 * @param terrTarget 
 */
Blockade::Blockade(Player* p, Player* pNeutral, Territory* terrTarget){
    this->p = p;
    this->terrTarget = terrTarget;
    this->pNeutral = pNeutral;
    setValid(validate());
}

Blockade* Blockade::clone() const{
    return new Blockade(this);
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Blockade::execute(State* current) {
    if (current->getStateName().compare("executeorders")==0 && getValid() == 1){
        cout << "Executing (Blockade) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Blockade) order #" << getOrderID() << " ...\n";
};

/**
 * @brief if player owns target territory, will double forces 
 * and give to neutral player
 * 
 */
void Blockade::execute() {
    if (!pIsInExecuteState(p, getOrderName())) { return; } // player is NOT in execute order state
    if(getValid()){
        doubleArmyUnits(this->terrTarget);
        p->removeTerritory(this->terrTarget);
        pNeutral->addTerritory(this->terrTarget);
        cout << "Blockade executed | "
            << terrTarget->getName() << " army is doubles, and is now "
            << terrTarget->getArmyCount()<<" | Territory is now Neutral" << "\n";
            notify(this);

    }else{
        cout << "Blockade can NOT be executed | "
            << "player does not own " << terrTarget->getName() << "\n";
    }
    
}

/**
 * Override the stringToLog method to print about the order
*/
string Blockade::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nBlockade executed | " + terrTarget->getName() + " army is doubles, and is now " + to_string(terrTarget->getArmyCount()) + " | Territory is now Neutral" + "\n\n------------------------------------------------------------------------------------------\n\n";
};

bool Blockade::validate(){ 
    if (p->ownsTerritory(this->terrTarget))
        return true;
    return false;
}

void Blockade::doubleArmyUnits(Territory* t) {
    int armySize = t->getArmyCount();
    t->setArmyCount(armySize * 2);
}

void Blockade::addDescription() {
    this->description = "(Blockade) Triple the number of army units on a target territory and make it a neutral territory. \n"
        "            This order can only be issued if a player has the blockade card in their hand.";
}

string Blockade::getDescription() {
    return this->description;
}

string Blockade::getOrderName() { return "Blockade"; }

/************************************************************ Airlift **************************************************************/
Airlift::Airlift() {
    this->addDescription();
}

Airlift::Airlift(const Airlift* a) {
    setOrderID(getCount());
    this->addDescription();
}

Airlift::Airlift(Player* p, Territory* terrSource, Territory* terrTarget, int numArmyUnits) {
    this->p = p;
    this->terrTarget = terrTarget;
    this->terrSource = terrSource;
    this->numArmyUnits = numArmyUnits;
    setValid(validate());
}


Airlift* Airlift::clone() const{
    return new Airlift(this);
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Airlift::execute(State* current) {
    if (current->getStateName().compare("executeorders")==0 && getValid() == 1){
        cout << "Executing (Airlift) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Airlift) order #" << getOrderID() << " ...\n";
};

/**
 * @brief if player ownns both territories will transfer specified
 * amount of army units from source to target
 * 
 */
void Airlift::execute() {
    if (!pIsInExecuteState(p, getOrderName())) { return; } // player is NOT in execute order state
    if(getValid()){
        terrSource->subFromArmy(numArmyUnits);
        terrTarget->addToArmyCount(numArmyUnits);
        cout << "Airlift executed | "
            << terrSource->getName() << " has " << terrSource->getArmyCount() << " army units" 
            << " | " << terrTarget->getName() << " has " << terrTarget->getArmyCount() << " army units" << "\n";
        notify(this);

    }
    else if (!p->ownsTerritory(terrSource)) {
        cout << "Ariflift can NOT be executed | "
            << "player does not own " << terrSource->getName() << "\n";
    }
    else if (!p->ownsTerritory(terrTarget)) {
        cout << "Ariflift can NOT be executed | "
            << "player does not own " << terrTarget->getName() << "\n";
    }
    else if (terrSource->getArmyCount() < numArmyUnits) {
        cout << "Ariflift can NOT be executed | "
            << "player source " << terrSource->getName() 
            << " does not have enough army units"<< "\n";
    }
}

/**
 * Override the stringToLog method to print about the order
*/
string Airlift::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nAirlift executed | " +  terrSource->getName() + " has " + to_string(terrSource->getArmyCount()) + " army units | " + terrTarget->getName() + " has " + to_string(terrTarget->getArmyCount()) + " army units" + "\n\n------------------------------------------------------------------------------------------\n\n";
};

bool Airlift::validate() {
    if(!p->ownsTerritory(terrSource))
        return false;
    if (!p->ownsTerritory(terrTarget))
        return false;
    if(terrSource->getArmyCount() < numArmyUnits)
        return false;
    return true;
}

void Airlift::addDescription() {
    this->description = "(Airlift) Advance a certain number of army units from one from one territory (source territory) to another territory \n"
        "            (target territory). This order can only be issued if a player has the airlift card in their hand.";
}

string Airlift::getDescription() {
    return this->description;
}

string Airlift::getOrderName() { return "Airlift"; }

/************************************************************ Negotiate **************************************************************/
Negotiate::Negotiate() {
    this->addDescription();
}

Negotiate::Negotiate(const Negotiate* a) {
    setOrderID(getCount());
    this->addDescription();
}

Negotiate::Negotiate(Player* pSource, Player* pTarget){
    this->pSource = pSource;
    this->pTarget = pTarget;
    setValid(validate());
}

Negotiate* Negotiate::clone() const{
    return new Negotiate(this);
}

/**
* Checks player state and executes order
* @param currentState player's current state
*/
void Negotiate::execute(State* current) {
    if (current->getStateName().compare("executeorders")==0 && getValid() == 1){
        cout << "Executing (Negotiate) order #" << getOrderID() << " ...\n";
    } else cout << "Can NOT execute (Negotiate) order #" << getOrderID() << " ...\n";
}

/**
 * @brief if source player and target player are diffrent, they are forbidden
 * from attacking eachother this turn;
 * 
 */
void Negotiate::execute() {
    if(getValid()){
        deleteAdvancesAgainstBothPlayerTerritories();
        cout << "Negotiate executed | "
            << "Player " << pSource->getID() << " will not be able to attack "
            << "Player " << pTarget->getID() << " and vice versa for the entire turn\n";
        notify(this);


    }else{
        cout << "Negotiate can NOT be executed | "
            << "player source " << pSource->getID()
            << " is the same as player target" << "\n";
    }
}

/**
 * Override the stringToLog method to print about the order
*/
string Negotiate::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nNegotiate executed |  Player " + to_string(pSource->getID()) + " will not be able to attack & Player " + to_string(pTarget->getID()) + " and vice versa for the entire turn\n\n------------------------------------------------------------------------------------------\n\n";
};

bool Negotiate::validate(){
    if(pSource->getID() == pTarget->getID())
        return false;
    return true;
}

void Negotiate::deleteAdvancesAgainstBothPlayerTerritories(){
    // will delete Advance orders from source player attacking target order
    for(auto pSourceOrder : *pSource->getOrdersList()->getOL()){ // loop through pSource orders list
        if(pSourceOrder->getOrderName().compare("Advance")==0){ // get Advance orders
            // checks if source player's Advance order is attacking a territory that the target players own
            // static cast to access methods from Advance Class
            if (pTarget->ownsTerritory(static_cast<Advance*>(pSourceOrder)->getTerrTarget())){
                pSource->getOrdersList()->remove(pSourceOrder->getOrderID()); // will remove Advance from ol and delete order ptr
            }
        }
    }

    // reapting, but delete Advance orders from target player attacking source order
    for (auto pTargetOrder : *pTarget->getOrdersList()->getOL()) { // loop through pTarget orders list
        if (pTargetOrder->getOrderName().compare("Advance") == 0) { // get Advance orders
            // checks if target player's Advance order is attacking a territory that the source players own
            // static cast to access methods from Advance Class
            if (pSource->ownsTerritory(static_cast<Advance*>(pTargetOrder)->getTerrTarget()))
                pTarget->getOrdersList()->remove(pTargetOrder->getOrderID()); // will remove Advance from ol and delete order ptr
        }
    }
}



void Negotiate::addDescription() {
    this->description = "(Negotiate) prevent attacks between the current player and another target player until the end of \n"
        "            the turn. This order can only be issued if a player has the diplomacy card in their hand.";
}

string Negotiate::getDescription() {
    return this->description;
}

string Negotiate::getOrderName() { return "Negotiate"; }

/************************************************************ OrdersList **************************************************************/
OrdersList::OrdersList(){
    OL = new vector<Order*>;
}

/**
 * Copy Constructor 
 */
OrdersList::OrdersList(const OrdersList *originalOrderList){
    OL = new vector<Order*>;
    for(int i = 0 ;  i < originalOrderList->OL->size(); i++){
        OL->push_back(originalOrderList->OL->at(i)->clone());
    }

    // for(const auto o : originalOrderList->OL){
    //     OL->push_back(o->clone());
    // }
}; 

OrdersList::~OrdersList(){
    cout <<"...Deleting Orders in OrdersList...\n\n";
    for (auto o : *OL) {
        delete o;   // deallocate memory
        o = NULL;   // prevent dangling pointer error
    }
    cout <<"OrdersList deleted.\n\n";
}

void OrdersList::addOrder(Order *o){
    OL->push_back(o);
    notify(this);
}
/**
 * Overload the stringtoLog method to log the orderList 
*/
string OrdersList::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\n OrderList: a new order was added with id " +  to_string(OL->back()->getOrderID())  + "\n\n------------------------------------------------------------------------------------------\n\n";
};

/**
* Moves order to new location in OL
* @param pos new position
* @param id order ID of order to be moved
*/
bool OrdersList::move(int pos, int id){
    int index;
    // post out of bounds
    int size = OL->size();
    if (pos == 0 || pos < 0 || pos > size) {
        cout << "From move(): position " << pos << " does not exist\n";
        return false;
    }
    // pos = 1, send to front of list
    if (pos == 1) {
        // 0 1 2 3 4 5
        // x y z a b c
        for (auto o : *OL) {
            if (o->getOrderID() == id) {
                index = getIndex(*OL,o);
                OL->erase(OL->begin()+index); // remove Order from List
                OL->insert(OL->begin(),o); // place in front
                return true;
            }
        }
    }
    // pos = last
    if (pos == size) {
        for (auto o : *OL) {
            if (o->getOrderID() == id) {
                index = getIndex(*OL, o);
                OL->erase(OL->begin() + index); // remove Order from List
                OL->push_back(o); // place in back
                return true;
            }
        }
    }
    // first position is >=2
    for (auto o : *OL) {
        if (o->getOrderID() == id) {
            index = getIndex(*OL, o);
            OL->erase(OL->begin() + index); // remove Order from List
            OL->insert(OL->begin()+(pos-1),o); // use insert from list std
            return true;
        }
    }
    return false;
}

bool OrdersList::remove(int id) { 
    int index;
    for (auto o : *OL) {
    if (o->getOrderID() == id) {
            index = getIndex(*OL,o);
            OL->erase(OL->begin()+index); 
            delete o;
            o = NULL;
            return true;
        }
    }
    cout<< "From remove(): Order#" << id << " does not exist\n";
    return false;
}

vector <Order*> * OrdersList::getOL(){
    return OL;
}

int OrdersList::getIndex(vector<Order*> ol, Order *o)
{
    auto it = find(ol.begin(), ol.end(), o);

    if (it != ol.end()) // if element is found 
    {
        int index = it - ol.begin();
        return index;
    }
    else {
        cout << "-1\n";
        return -1;
    }
}

void OrdersList::executeAll(State* s) {
    for (auto o : *getOL()) {
        o->execute(s);
    }
    cout << "\n";
}

void OrdersList::executeAll() {
    for (auto o : *getOL()) {
        if(o->getOrderID() != 0){ // check if one of orders has been deleted 
            cout << "Order #" << o->getOrderID() << "\n";
            o->execute();
        }    
    }
    cout << "\n\n";
}

void OrdersList::executeNextOrder(State* s) {
    auto it = *getOL()->begin();
    it->execute();
    cout << "\n";
}

void OrdersList::deleteOrdersList(){
    for (auto o : *getOL()) {
        delete o;   // deallocate memory
        o = NULL;   // prevent dangling pointer error
    }
    getOL()->clear();
}

ostream& operator << (ostream& out, OrdersList* ol) {
    vector<Order*>::iterator it;
    vector<Order*> OL = *ol->getOL();
    cout << "The OrdersList contains \n------------------------\n";
    int pos = 0;
    for (it = OL.begin(); it != OL.end(); it++)
    {
        out << "pos: " << ++pos
            << " | Order: " << *it;
    }
    return out;
}





