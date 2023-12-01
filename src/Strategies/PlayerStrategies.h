#pragma once
#include <iostream>

#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"



// forward declaration
class Player;
/**
 * PlayerStrategy - abstract class that need to have overriden methods
 */
class PlayerStrategy {
    public:
    virtual void issueOrder(Player *p) = 0;
    virtual vector<Territory*> toAttack(Player *p) = 0;
    virtual vector<Territory*> toDefend(Player *p) = 0;
    virtual ~PlayerStrategy();

    //todo override stream insertion and copy con
};

/**
 * Neutral 
 * Computer player that never issues an order, doesnt use any cards
 * It can recive card and if its attacked, strategy is changed to an aggressive player
*/
class Neutral : public PlayerStrategy {
    public:
    void issueOrder(Player *p);
    vector<Territory*> toAttack(Player *p);
    vector<Territory*> toDefend(Player *p);
    Neutral();
    ~Neutral();
};


/**
 * Cheater 
 * Computer player that conquers all territories that are agecent to its own territories, once per turn. 
 * Doesnt use cards but it can recieve them
*/
class Cheater : public PlayerStrategy {
    public:
    void issueOrder(Player *p);
    vector<Territory*> toAttack(Player *p);
    vector<Territory*> toDefend(Player *p);
    Cheater();
    ~Cheater();
};

/**
 * Human 
 * Requires user interactions to make decisions including deploy and advance orders and can use any cards 
 * Currently impelemented from previous phases
*/
class Human : public PlayerStrategy {
    public:
    void issueOrder(Player *p);
    vector<Territory*> toAttack(Player *p);
    vector<Territory*> toDefend(Player *p);
    Human();
    ~Human();
};

/**
 * Aggressive 
 * Computer player that focuses on attack / deploy or advances armies on its strongest countries and always advances on enemy territories until it cant
 * Will use any card with an aggressive purpose
*/
class Aggressive : public PlayerStrategy {
    public:
    void issueOrder(Player *p);
    vector<Territory*> toAttack(Player *p);
    vector<Territory*> toDefend(Player *p);
    Aggressive();
    ~Aggressive();
};


/**
 * Benevolant
 * Computer focuses on protecting its weak countries 
 * Deploys and advances its armies on its weakest countries, doesnt advance to new territories
 * Can use hard, but not to harm anyone on purpose (ex: negociate)
*/
class Benevolant : public PlayerStrategy {
    public:
    void issueOrder(Player *p);
    vector<Territory*> toAttack(Player *p);
    vector<Territory*> toDefend(Player *p);
    Benevolant();
    ~Benevolant();
};
 


/************************************************************ PlayerStrategiesDriver **************************************************************/
void testPlayerStrategies();