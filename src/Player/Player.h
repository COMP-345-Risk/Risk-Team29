#pragma once
#include <time.h>

#include <iostream>
#include <string>
#include <vector>

#include "../CardsDeck/Cards.h"
#include "../GameEngine/GameEngine.h"
#include "../Map/Map.h"
#include "../OrdersList/Orders.h"
#include "../Strategies/PlayerStrategies.h"

using namespace std;

// forward declaration
class Hand;
class PlayerStrategy;
/************************************************************ Player **************************************************************/

class Player {
 private:
  int id;
  int reinforcements;
  vector<Territory*> territories;
  Hand* hand;
  OrdersList* orderList;
  State* state;
  string name;
  PlayerStrategy * ps;

  friend ostream& operator<<(ostream& out, Player* o);  // overide Stream insertion operator

 public:
  /**
   * Constructor with with an argument list
   */
  Player(vector<Territory*>, Hand*, OrdersList*, int id, State*, PlayerStrategy * ps);
  
  Player(vector<Territory*>, Hand*, OrdersList*, int id, State*, string name, PlayerStrategy* ps);

  Player(Hand*, OrdersList*, int id, State*, string name, PlayerStrategy* ps);

  Player(vector<Territory*>, Hand*, OrdersList*, int id, State*);

  /**
   * Default Constructor
   */
  Player();

  /**
   * Param con
   */
  Player(int id);
  
  /**
  * Param con 
  */
  Player(int id, PlayerStrategy * ps);

  /**
   * Copy Constructor
   */
  Player(const Player&);

  /**
   * Destructor
   */
  ~Player();

  int getID();

  int getReinforcement();

  void setReinforcement(int r);

  void addReinforcement(int r);

  void subtractReinforcemnts(int r);

  vector<Territory*> getTerritories();

  void addTerritory(Territory* t);

  void removeTerritory(Territory* t);

  void eraseTerritory(Territory* t);

  OrdersList* getOrdersList();

  State* getState();

  bool ownsTerritory(Territory *t);

  Hand* getHand();

  string getName();

  void setState(string s);

  void setStrategy(PlayerStrategy* ps);

  PlayerStrategy* getStrategy();

  void setName(string n);

  /**
   * Returns a random list of territories that are assigned to the user which
   * they would like to defend
   */
  vector<Territory*> toDefend();

  /**
   * Returns a random list of territories that the user would like to attack
   */
  vector<Territory*> toAttack();

  /**
   * Take in an order and add it into the OrderList
   */
  OrdersList* issueOrder(Order* o);

  /**
   * Helper method to print the list of territories to attack/defended
   */
  void printTerritories(vector<Territory*> territories);

  //void executeNextOrderAndRemove();
};

/************************************************************ PlayerDriver **************************************************************/
void testPlayers();