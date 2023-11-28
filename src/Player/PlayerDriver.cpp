#include "Player.h"

void testPlayers() {
  cout << "\n*************************************\nTest "
          "Player\n*************************************\n\n";
  vector<Territory*> t;
  Hand* h = new Hand();
  OrdersList* o = new OrdersList;

  // create 6 territories and push them to the territories vector
  Territory* t1 = new Territory("UK", 1, 2, 0);
  Territory* t2 = new Territory("USA", 2, 3, 0);
  Territory* t3 = new Territory("Russia", 3, 3, 0);
  Territory* t4 = new Territory("Israel", 4, 2, 0);
  Territory* t5 = new Territory("Italy", 5, 3, 0);
  Territory* t6 = new Territory("Bulgaria", 6, 3, 0);
  t.push_back(t1);
  t.push_back(t2);
  t.push_back(t3);
  t.push_back(t4);
  t.push_back(t5);
  t.push_back(t6);

  // init a player
  Player* p = new Player(t, h, o, 1, NULL, new Human());

  cout << "---------> Test 1: Testing player methods (attack/defend/issueOrder) <---------\n\n";
  // show that Player contains method toDefend()
  p->toDefend();

  // show that Player contains method toAttack()
  p->toAttack();

  // show that Player contains method issueOrder()
  Order* newOrder = new Order();
  p->issueOrder(newOrder);

  cout << "---------> Test 2: Testing coppied player methods (attack/defend/issueOrder) <---------\n\n";
  // show that player contains a copy constructor
  Player* pCopy = new Player(*p);

  // show that Player contains method toDefend()
  pCopy->toDefend();

  // show that Player contains method toAttack()
  pCopy->toAttack();

  // show that Player contains method issueOrder()
  Order* newOrderCopy = new Order();
  pCopy->issueOrder(newOrderCopy);

  // print player details
  cout << p;

  // player has a destructor that frees the pointer to hand
  h = NULL;

  cout << "...Deleting territories from PlayerDriver...\n";
  // delete territories pointer
  for (int i = 0; i < t.size(); i++) {
    delete t.at(i);
    t.at(i) = NULL;
  }
  cout << "...Deleting orderslist from PlayerDriver...\n";
  // delete orderlist pointer
  for (int i = 0; i < o->getOL()->size(); i++) {
    delete o->getOL()->at(i);
    o->getOL()->at(i) = NULL;
  }
  // orderlist is already being freed in player destructor
  o = NULL;

  // delete player pointer
  delete p;
  p = NULL;

  // delete playerCopy pointer
  delete pCopy;
  pCopy = NULL;
};
