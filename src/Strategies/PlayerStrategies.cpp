#include "./PlayerStrategies.h"
#include<unordered_set>

PlayerStrategy::~PlayerStrategy() {};

/**
 * Neutral
 * Computer player that never issues an order, doesnt use any cards
 * It can recive card and if its attacked, strategy is changed to an aggressive player
*/
Neutral::Neutral() {}
Neutral::~Neutral() {};

void Neutral::issueOrder(Player* p) {

};

vector<Territory*> Neutral::toAttack(Player* p) {
    vector<Territory*> territories;
    return territories;
};
vector<Territory*> Neutral::toDefend(Player* p) {
    vector<Territory*> territories;
    return territories;
};


/**
 * Cheater
 * Computer player that conquers all territories that are agecent to its own territories, once per turn.
 * Doesnt use cards but it can recieve them
*/
Cheater::Cheater() {};
Cheater::~Cheater() {};

void Cheater::issueOrder(Player* p) {

};
vector<Territory*> Cheater::toAttack(Player* p) {
    vector<Territory*> territories;
    return territories;
};
vector<Territory*> Cheater::toDefend(Player* p) {
    vector<Territory*> territories;
    return territories;
};


/**
 * Human
 * Requires user interactions to make decisions including deploy and advance orders and can use any cards
 * Currently impelemented from previous phases
*/
Human::Human() {}
Human::~Human() {}

void Human::issueOrder(Player* p) {
  cout << " ********************** Human issue order **********************\n";
  string userInput;
  // Only use Deploy orders while the player has reinforcements
  while (p->getReinforcement() > 0) {
    vector<Territory*> canDeploy = toDefend(p);

    cout << "Choose a Territory to Deploy armies to (Please use the number)\n";
    getline(cin, userInput);
    int territoryId = stoi(userInput);
    cout << "How many armies would you like to send? (Available armies: " << p->getReinforcement() << ")\n";
    getline(cin, userInput);
    int numArmies = stoi(userInput);

    Deploy* deploy = new Deploy(p, canDeploy[territoryId], numArmies);
    p->getOrdersList()->addOrder(deploy);
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
      vector<Territory*> canDefend = toDefend(p);
      vector<Territory*> canHelp; // territories that you can move your armies to
      int destinationTerritory;
      do {
        cout << "Choose a Territory to Deploy armies to (Please use the number) or -1 to cancel this operation\n";
        getline(cin, userInput);
        destinationTerritory = stoi(userInput);
        for (Territory* terr : canDefend.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == p->getID()) {
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
          p->printTerritories(canHelp);
          getline(cin, userInput);
          fromTerritory = stoi(userInput);
          cout << "How many armies would you like to send? (Available armies: "
            << canHelp.at(fromTerritory)->getArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canHelp.at(fromTerritory)->getArmyCount() || numArmies < 0);
        p->getOrdersList()->addOrder(new Advance(p, canDefend.at(destinationTerritory), canHelp.at(fromTerritory), numArmies));
      }
      break;
    }
    case 2:
    {
      // Advance to Attack enemy territories
      vector<Territory*> canAttack = toAttack(p);
      vector<Territory*> canAttackWith;
      int destinationTerritory;
      do {
        cout << "Choose a Territory to Attack (Please use the number) or -1 to cancel this operation\n";
        getline(cin, userInput);
        destinationTerritory = stoi(userInput);
        for (Territory* terr : canAttack.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == p->getID()) {
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
          p->printTerritories(canAttackWith);
          getline(cin, userInput);
          fromTerritory = stoi(userInput);
          cout << "How many armies would you like to send? (Available armies: "
            << canAttackWith.at(fromTerritory)->getArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canAttackWith.at(fromTerritory)->getArmyCount() || numArmies < 0);
        p->getOrdersList()->addOrder(new Advance(p, canAttack.at(destinationTerritory), canAttackWith.at(fromTerritory), numArmies));
      }
      break;
    }
    case 3:
    {
      cout << "Choose a card to play from your hand:\n";
      p->getHand()->printHand();
      getline(cin, userInput);
      int cardNum = stoi(userInput);
      Card* card = p->getHand()->getCard(cardNum);
      Order* order = card->play(p);
      p->getOrdersList()->addOrder(order);
      break;
    }
    case 4:
      userInput = -1;
      break;
    default:
      cout << "That was not an available option, you will have to wait for next round to issue an order\n";
    }
  } while (option != 3);

}
vector<Territory*> Human::toAttack(Player* p) {
  cout << " ********************** Human to attack **********************\n";
  vector<Territory*> canAttack;
  if (p->getTerritories().size() == 0) {
    cout << "...There are no territories to attack...\n";
    return canAttack;
  }

  for (Territory* territory : p->getTerritories()) {
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
vector<Territory*> Human::toDefend(Player* p) {
  cout << " ********************** Human to defend **********************\n";
  vector<Territory*> canDefend = p->getTerritories();
  // if no territories were init
  if (p->getTerritories().size() == 0) {
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
  p->printTerritories(canDefend);
  return canDefend;
}


/**
 * Aggressive
 * Computer player that focuses on attack / deploy or advances armies on its strongest countries and always advances on enemy territories until it cant
 * Will use any card with an aggressive purpose
*/
Aggressive::Aggressive() {

};
Aggressive::~Aggressive() {}
void Aggressive::issueOrder(Player* p) {

};
vector<Territory*> Aggressive::toAttack(Player* p) {
    vector<Territory*> territories;
    return territories;
};
vector<Territory*> Aggressive::toDefend(Player* p) {
    vector<Territory*> territories;
    return territories;
};

/**
 * Benevolant
 * Computer focuses on protecting its weak countries
 * Deploys and advances its armies on its weakest countries, doesnt advance to new territories
 * Can use hard, but not to harm anyone on purpose (ex: negociate)
*/
Benevolant::Benevolant() {};
Benevolant::~Benevolant() {}

void Benevolant::issueOrder(Player* p) {

};
vector<Territory*> Benevolant::toAttack(Player* p) {
    vector<Territory*> territories;
    return territories;
};
vector<Territory*> Benevolant::toDefend(Player* p) {
    vector<Territory*> territories;
    return territories;
};