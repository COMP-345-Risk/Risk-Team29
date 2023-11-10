#pragma once
#include <vector>
#include <iostream>

#include "../LoggingObserver/LoggingObserver.h"
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"


using namespace std;

class Player;

class OrderAbstract
{
public:
    virtual void execute() = 0;
};

class Order : public OrderAbstract, public Subject, public ILoggable 
{
public:

    Order();

    Order(const Order* o);

    virtual ~Order(); // need to delete parent when call child destructor 

    virtual Order* clone() const;

    /**
    * Checks if valid for execution, invalid orders can exist
    */
    virtual bool validate();

    bool getValid();

    void setValid(bool v);
    
    /**
    * Checks player state and executes order
    */
    virtual void execute(State *currentState);

    virtual void execute();

    /**
    * Increment countOrderID by 1
    */
    int incrementCount(); 

    int getCount();

    void setOrderID(int id);

    int getOrderID();

    virtual void addDescription();

    virtual string getDescription();

    virtual string getOrderName();

    void setPlayerGivenCardThisTurn(bool b);

    bool getPlayerGivenCardThisTurn();

    bool pOwnsTerr(Player* p, Territory* t);

    bool terrIsAdjP(Territory *t1, Territory* t2);

    bool terrHasOwner(Territory *t);

    bool pIsInExecuteState(Player *p, string orderName);

    virtual string stringToLog();

protected:
    string description;
private:
    static int countOrderID;
    int orderID;
    bool valid;
    friend ostream& operator<<(ostream& out, Order* o);  // overide Stream insertion operator
    static bool pGivenCardThisTurn; 
};

class Deploy : public Order
{
public:
    Deploy();

    Deploy(Player* p, Territory* terrToDeploy, int numReinToDeploy);

    Deploy(const Deploy* d);

    Deploy* clone() const;

    void execute(State *currentState);

    string stringToLog();

    /*
    * Checks if Deploy is valid and Player owns Territory to deploy,
    * then adds reinforcments.
    */
    void execute();

    bool validate();

    bool pHasEnoughRein();

    void addDescription();

    string getDescription();

    string getOrderName();

private:
    Territory* terrToDeploy;
    int numReinToDeploy;
    vector<Territory*> tOwned;
    Player *p;
};

class Advance : public Order
{
public:
    Advance();

    Advance(const Advance* a);

    Advance(Player* p, Territory* terrSource, Territory* terrTarget, int numReinToAdvnce);

    Advance* clone() const;

    void execute(State *currentState);

    void execute();

    bool validate();

    string stringToLog();

    /**
    * Simulates battle between two armies.
    * @return int 1 if source wins, 0 if target wins
    */
    int battle();

    void occupyConqueredTerr();

    string givePLayerNewCard();

    void addDescription();

    string getDescription();

    string getOrderName();

    Territory* getTerrTarget();
    
private:
    Player* p;
    Territory* terrSource;
    Territory* terrTarget;
    int numReinToAdvnce;
};

class Bomb : public Order
{
public:
    Bomb();

    Bomb(const Bomb* b);

    Bomb(Player* p, Territory* terrTarget);

    Bomb* clone() const;

    void execute(State *currentState);

    void execute();

    bool validate();

    bool terrTargetIsAdjP();

    void halfArmyUnits(Territory *t);
    
    void addDescription();
    
    string getDescription();

    string getOrderName();

    string stringToLog();

private:
    Player* p;
    Territory* terrTarget;
};

class Blockade : public Order
{
public:
    Blockade();
   
    Blockade(const Blockade* a);

    Blockade(Player* p, Player* pNeutral, Territory* t);

    Blockade* clone() const;
    
    void execute(State *currentState);

    void execute();

    bool validate();

    void doubleArmyUnits(Territory* t);
    
    void addDescription();
    
    string getDescription();

    string getOrderName();

    string stringToLog();

private:
    Player* p;
    Territory* terrTarget;
    Player* pNeutral;
};

class Airlift : public Order
{
public:
    Airlift();
    
    Airlift(const Airlift* a);

    Airlift(Player* p, Territory* terrSource, Territory* terrTarget, int numArmyUnits);

    Airlift* clone() const;
    
    void execute(State *currentState);

    void execute();

    bool validate();
    
    void addDescription();
    
    string getDescription();

    string getOrderName();
    
    string stringToLog();

private:
    Player* p;
    Territory* terrSource;
    Territory* terrTarget;
    int numArmyUnits;

};

class Negotiate : public Order
{
public:
    Negotiate();
    
    Negotiate(const Negotiate* a);

    Negotiate(Player* pSource, Player* pTarget);

    Negotiate* clone() const;
    
    void execute(State *currentState);

    void execute();

    bool validate();

    void deleteAdvancesAgainstBothPlayerTerritories();
    
    void addDescription();
    
    string getDescription();

    string getOrderName();
    
    string stringToLog();

private:
    Player* pSource;
    Player* pTarget;
};


class OrdersList : public Subject, public ILoggable 
{
public:
    OrdersList();
    
    /**
     * Copy Constructor
     */
    OrdersList(const OrdersList *originalOrderList);

    ~OrdersList();

    virtual void addOrder(Order* o);
    
    string stringToLog();

    /**
    * Moves order to new location in OL
    * @param pos new position 
    * @param id order ID of order to be moved
    */
    bool move(int pos, int id);
    
    /**
     * Will delete and set Order ptr to null
     */
    bool remove(int id);
    
    vector <Order*> * getOL();

    int getIndex(vector<Order*> ol, Order *o);

    void executeAll(State *s); //A1

    void executeAll(); //A2

    void executeNextOrder(State *s);
    
    void deleteOrdersList();
  
private:
    vector<Order*> *OL;

    friend ostream& operator << (ostream& out, OrdersList* ol); // overide Stream insertion operator

};

/** 
* test driver A1
*/
void testOrdersLists();

/**
* test driver A2
*/
void testOrderExecution();