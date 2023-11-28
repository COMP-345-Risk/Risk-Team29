#include "Player.h"
#include<unordered_set>

/************************************************************ Player **************************************************************/
/**
 * Constructor with with an argument list
 */
Player::Player(vector<Territory*> t, Hand* h, OrdersList* o, int id, State* s) {
  territories = t;
  hand = h;
  orderList = o;
  this->id = id;
  state = s;
  reinforcements = 0;

  // if seed is set to 1, the generator is reinitialized to its initial value
  // and produces the same values as before any call to rand or srand
  srand((unsigned)time(NULL));
}

void Player::setName(string n) {
  name = n;
}

string Player::getName() {
  return name;
}
/**
 *  Helper method to list attack/defended territories
 */
void Player::printTerritories(vector<Territory*> territories) {
  for (int i = 0; i < territories.size(); i++) {
    cout << i << ")" << territories.at(i);
  }
}

/**
 * Copy Constructor
 */
Player::Player(const Player& p) {
  cout << "...Player Copy constructor was called...\n";
  // its a vector, hence we don't create a new pointer to the territories
  for (int i = 0; i < p.territories.size(); i++) {
    Territory* currentT = p.territories.at(i);
    territories.push_back(currentT);
  }

  // create a new hand
  hand = new Hand(*p.hand);

  // create a new orderlist
  orderList = new OrdersList(*p.orderList);

  // create a new state
  if (p.state == NULL) {
    state = NULL;
  }
  else {
    state = new State(*p.state);
  }

  // copy reinforcements
  reinforcements = p.reinforcements;

  // its not a pointer, so we just increase the ID 1
  id = p.id + 1;
};

/**
 * Destructor
 */
Player::~Player() {
  cout << "...Player destructor was called...\n";
  for (int i = 0; i < territories.size(); i++) {
    territories.at(i) = NULL;
  };

  delete hand;
  hand = NULL;

  delete orderList;
  orderList = NULL;

  delete state;
  state = NULL;
}

/**
 * Default Constructor
 */
Player::Player() {
  hand = new Hand();
  reinforcements = 0;
};

/**
 * Param Constructor
 */
Player::Player(int i) : Player() { id = i; };

int Player::getID() { return this->id; }

int Player::getReinforcement() { return this->reinforcements; }

void Player::setReinforcement(int r) { this->reinforcements = r; }

void Player::addReinforcement(int r) { this->reinforcements = this->reinforcements + r; }

void Player::subtractReinforcemnts(int r) { this->reinforcements = this->reinforcements - r; }

vector<Territory*> Player::getTerritories() { return this->territories; }

void Player::addTerritory(Territory* t) {
  t->setOwnerId(this->id);
  this->territories.push_back(t);
}

void Player::removeTerritory(Territory* t) {
  auto it = find(territories.begin(), territories.end(), t);
  territories.erase(it);
}

/**
 * @brief deletes territory and sets
 */
void Player::eraseTerritory(Territory* t) {
  if (territories.size() == 0) {
    cout << "Error: Territroy list is empty\n";
    return;
  }
  if (!ownsTerritory(t)) {
    cout << "Error: Player does not own Territory\n";
    return;
  }
  t->setOwnerId(0); // resets value that map points too
  t->setArmyCount(0); // resets value that map points too
  auto it = find(territories.begin(), territories.end(), t);
  territories.erase(it);
}

OrdersList* Player::getOrdersList() { return this->orderList; }

State* Player::getState() { return this->state; }

bool Player::ownsTerritory(Territory* t) { return t->getOwnerId() == this->id; }

Hand* Player::getHand() { return this->hand; }

/**
 * Returns a list of territories that are assigned to the user which they
 * would like to defend from other players
 */
vector<Territory*> Player::toDefend() {
  vector<Territory*> canDefend = territories;
  // if no territories were init
  if (territories.size() == 0) {
    cout << "...There are no territories to defend...\n";
    return canDefend;
  }

  // for(Territory *territory: territories) {
  //   vector<Territory*> adjacencyList = territory->getAdjacencyList();
  //   for(Territory *adjacent: adjacencyList) {
  //     // an adjacent territory is owned by another player, this means they can attack this territory
  //     if(adjacent->getOwnerId() != id) {
  //       canDefend.push_back(territory);
  //     }
  //   }
  // }
  cout << "\nTerritories to defend:\n -------------------\n";
  std::sort(canDefend.begin(), canDefend.end(), [](Territory* t1, Territory* t2) {
    // Territories with fewer armies should be defended in priority
    return t1->getArmyCount() < t2->getArmyCount();
    });
  printTerritories(canDefend);
  return canDefend;
}

/**
 * Returns a list of territories that the user could attack
 */
vector<Territory*> Player::toAttack() {
  vector<Territory*> canAttack;
  if (territories.size() == 0) {
    cout << "...There are no territories to attack...\n";
    return canAttack;
  }

  for (Territory* territory : territories) {
    for (Territory* attackOption : territory->getAdjacencyList()) {
      canAttack.push_back(attackOption);
    }
  }
  unordered_set<Territory*> uniqueSet(canAttack.begin(), canAttack.end());
  vector<Territory*> uniqueCanAttack(uniqueSet.begin(), uniqueSet.end());

  std::sort(canAttack.begin(), canAttack.end(), [](Territory* t1, Territory* t2) {
    // Easier to conquer a territory with fewer armies to defend itself, these should be the priority
    return t1->getArmyCount() < t2->getArmyCount();
    });

  return uniqueCanAttack;
}

/**
 * Take in an order and add it into the OrderList
 */
OrdersList* Player::issueOrder(Order* o) {
  string userInput;
  // Only use Deploy orders while the player has reinforcements
  while (reinforcements > 0) {
    vector<Territory*> canDeploy = toDefend();

    cout << "Choose a Territory to Deploy armies to (Please use the number)\n";
    getline(cin, userInput);
    int territoryId = stoi(userInput);
    cout << "How many armies would you like to send? (Available armies: " << reinforcements << ")\n";
    getline(cin, userInput);
    int numArmies = stoi(userInput);

    Deploy* deploy = new Deploy(this, canDeploy[territoryId], numArmies);
    orderList->addOrder(deploy);
  }
  int option;
  do {
    cout << "\nWould you like to Advance armies to defend your territories, attack territories, play a card, or stop your turn?\n";
    cout << "1. Advance to Defend my territories\n2. Advance to Attack others territories\n3. Play a card\n4. Stop\n";
    getline(cin, userInput);
    int option = stoi(userInput);
    switch (option) {
    case 1: {
      // Advance to Defend my territories
      vector<Territory*> canDefend = toDefend();
      vector<Territory*> canHelp;
      int destinationTerritory;
      do {
        cout << "Choose a Territory to Deploy armies to (Please use the number) or -1 to cancel this operation\n";
        getline(cin, userInput);
        destinationTerritory = stoi(userInput);
        for (Territory* terr : canDefend.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == id) {
            canHelp.push_back(terr);
          }
        }
        if (canHelp.size() == 0) {
          cout << "You don't own any nearby territories to send armies to " << canDefend.at(destinationTerritory)->getName() << "\n";
        }
      } while (canHelp.size() == 0 || destinationTerritory == -1);
      if (canHelp.size() > 0) {
        int numArmies;
        int fromTerritory;
        do {
          cout << "From what territory would you like to send reinforcements from?\n";
          printTerritories(canHelp);
          getline(cin, userInput);
          fromTerritory = stoi(userInput);
          cout << "How many armies would you like to send? (Available armies: "
            << canHelp.at(fromTerritory)->getArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canHelp.at(fromTerritory)->getArmyCount() || numArmies < 0);
        orderList->addOrder(new Advance(this, canDefend.at(destinationTerritory), canHelp.at(fromTerritory), numArmies));
      }
      break;
    }
    case 2:
    {
      // Advance to Attack enemy territories
      vector<Territory*> canAttack = toAttack();
      vector<Territory*> canAttackWith;
      int destinationTerritory;
      do {
        cout << "Choose a Territory to Attack (Please use the number) or -1 to cancel this operation\n";
        getline(cin, userInput);
        destinationTerritory = stoi(userInput);
        for (Territory* terr : canAttack.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == id) {
            canAttackWith.push_back(terr);
          }
        }
        if (canAttackWith.size() == 0) {
          cout << "You don't own any nearby territories to attack to " << canAttack.at(destinationTerritory)->getName() << "\n";
        }
      } while (canAttackWith.size() == 0 || destinationTerritory == -1);
      if (canAttackWith.size() > 0) {
        int numArmies;
        int fromTerritory;
        do {
          cout << "From what territory would you like to send reinforcements from?\n";
          printTerritories(canAttackWith);
          getline(cin, userInput);
          fromTerritory = stoi(userInput);
          cout << "How many armies would you like to send? (Available armies: "
            << canAttackWith.at(fromTerritory)->getArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canAttackWith.at(fromTerritory)->getArmyCount() || numArmies < 0);
        orderList->addOrder(new Advance(this, canAttack.at(destinationTerritory), canAttackWith.at(fromTerritory), numArmies));
      }
      break;
    }
    case 3:
    {
      cout << "Choose a card to play from your hand:\n";
      hand->printHand();
      getline(cin, userInput);
      int cardNum = stoi(userInput);
      Card* card = hand->getCard(cardNum);
      card->play(this);
      break;
    }
    case 4:
      userInput = -1;
      break;
    default:
      cout << "That was not an available option, you will have to wait for next round to issue an order\n";
    }
  } while (option != 3);

  // Play a Card

  orderList->addOrder(o);
  cout << "...Pushed a new order to orderList...\n";
  cout << "New order id: " << o->getOrderID() << "\n\n";
  return orderList;
}

/**
 * Override the stream operator for Player
 */
ostream& operator<<(ostream& out, Player* p) {
  out << "Printing info about player ID: " << p->id
    << "\n********************************\n\n";
  int reinforcementCount = p->getReinforcement();
  int territoryCount = p->territories.size();
  if (reinforcementCount > 0) {
    out << "Reinforcements: " << reinforcementCount << "\n\n";
  }
  out << "My name is: " << p->name << "\n\n";
  if (territoryCount > 0) {
    out << " Info about territories: \n ------------------------\n";
    p->printTerritories(p->territories);
  };

  //  if(!(p->orderList == NULL)){
  //   out << p->orderList;
  // };
  out << p->hand;
  // !! dont need state in player
  // out << "\nCurrent player's state: "
  //     << (p->state == NULL ? "no state yet" : p->state->getStateName())
  //     << "\n\n";
  return out;
}
