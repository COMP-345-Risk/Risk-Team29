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
  // Neutral Player never issues any orders
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
  // deploy some reinforcements
  int reinforcements = p->getReinforcement();
  vector<Territory*> canDeploy = toDefend(p);
  if(reinforcements > 0) {
    if(canDeploy.size() > 0) {
      // we know at 0 is our strongest and the cheater should reinforce it's strongest
      Deploy* deploy = new Deploy(p, canDeploy.at(0), reinforcements);
      canDeploy.at(0)->addToPendingArmyCount(reinforcements);
      p->getOrdersList()->addOrder(deploy);
    }
  }
  // This player doesn't actually issue advance orders, it simply conquers other territories adjacent to itself
  vector<Territory*> canAttack = p->toAttack();
  for(Territory* territory : canAttack) {
    territory->setOwnerId(p->getID());
  }
};

vector<Territory*> Cheater::toAttack(Player* p) {
  vector<Territory*> ownedTerritories = p->getTerritories();
  vector<Territory*> canAttack;
  if (ownedTerritories.size() == 0) {
    cout << "...You don't own any territories you can attack with...\n";
    return canAttack;
  }

  for (Territory* territory : ownedTerritories) {
    for (Territory* attackOption : territory->getAdjacencyList()) {
      if(attackOption->getOwnerId() != p->getID()) {
        canAttack.push_back(attackOption);
      }
    }
  }
  return canAttack;
};
vector<Territory*> Cheater::toDefend(Player* p) {
  return p->getTerritories();
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
  cout << "Player " << p->getName() << " has " << p->getReinforcement() << " reinforcemenets \n";
  int reinforcements = p->getReinforcement();
  while (reinforcements > 0) {
    vector<Territory*> canDeploy = toDefend(p);
    cout << "Player " << p->getName() << ":\n";
    cout << "Choose a Territory to Deploy armies to (Please use the number)\n";
    getline(cin, userInput);
    int territoryId = stoi(userInput);
    cout << "How many armies would you like to send? (Available armies: " << reinforcements << ")\n";
    getline(cin, userInput);

    int numArmies = stoi(userInput);

    Deploy* deploy = new Deploy(p, canDeploy.at(territoryId), numArmies);
    canDeploy.at(territoryId)->addToPendingArmyCount(numArmies);
    reinforcements -= numArmies; // remove the used reinforcement 
    p->getOrdersList()->addOrder(deploy);
  }
  int option;
  do {
    cout << "\nWould you like to Advance armies to defend your territories, attack territories, play a card, or stop your turn?\n";
    cout << "1. Advance to Defend my territories\n2. Advance to Attack others territories\n3. Play a card\n4. Stop\n";
    getline(cin, userInput);
    option = stoi(userInput);
    switch (option) {
    case 1: {
      // Advance to Defend my territories
      vector<Territory*> canDefend = toDefend(p);
      vector<Territory*> canHelp; // territories that you can move your armies to
      int destinationTerritory;
      do {
        cout << "Choose a Territory to Deploy armies to (Please use the number) or -1 to cancel this operation\n";
        getline(cin, userInput);
        if(userInput.compare("-1") == 0) {
          break;
        }
        destinationTerritory = stoi(userInput);
        vector<Territory*> canHelpList; // territories that you can move your armies to
        for (Territory* terr : canDefend.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == p->getID()) {
            canHelpList.push_back(terr);
          }
        }
        unordered_set<Territory*> uniqueSet(canHelpList.begin(), canHelpList.end());
        vector<Territory*> uniqueCanDefend(uniqueSet.begin(), uniqueSet.end());
        for(Territory* terr : uniqueCanDefend) {
          canHelp.push_back(terr);
        }

        if (canHelp.size() == 0) {
          cout << "You don't own any nearby territories to send armies to " << canDefend.at(destinationTerritory)->getName() << "(ID:" << canDefend.at(destinationTerritory)->getId() << ")" << "\n";
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
            << canHelp.at(fromTerritory)->getTotalArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canHelp.at(fromTerritory)->getTotalArmyCount() || numArmies < 0);
        p->getOrdersList()->addOrder(new Advance(p, canHelp.at(fromTerritory), canDefend.at(destinationTerritory), numArmies));
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
        if(userInput.compare("-1") == 0) {
          break;
        }
        destinationTerritory = stoi(userInput);
        vector<Territory*> canAttackWithList;
        for (Territory* terr : canAttack.at(destinationTerritory)->getAdjacencyList()) {
          if (terr->getOwnerId() == p->getID()) {
            canAttackWithList.push_back(terr);
          }
        }
        unordered_set<Territory*> uniqueSet(canAttackWithList.begin(), canAttackWithList.end());
        vector<Territory*> uniqueCanAttack(uniqueSet.begin(), uniqueSet.end());
        for(Territory* terr : uniqueCanAttack) {
          canAttackWith.push_back(terr);
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
            << canAttackWith.at(fromTerritory)->getTotalArmyCount() << ")\n";
          getline(cin, userInput);
          numArmies = stoi(userInput);
        } while (numArmies > canAttackWith.at(fromTerritory)->getTotalArmyCount() || numArmies < 0);
        p->getOrdersList()->addOrder(new Advance(p, canAttackWith.at(fromTerritory), canAttack.at(destinationTerritory), numArmies));
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
      userInput = "-1";
      // reset all pending armies
      for(Territory* terr: p->getTerritories()) {
        terr->resetPendingArmies();
      } 
      break;
    default:
      cout << "That was not an available option, you will have to wait for next round to issue an order\n";
    }
  } while (option != 4);

}
vector<Territory*> Human::toAttack(Player* p) {
  cout << " ********************** Human to attack **********************\n";
  vector<Territory*> ownedTerritories = p->getTerritories();
  vector<Territory*> canAttack;
  if (ownedTerritories.size() == 0) {
    cout << "...You don't own any territories you can attack with...\n";
    return canAttack;
  }

  for (Territory* territory : ownedTerritories) {
    for (Territory* attackOption : territory->getAdjacencyList()) {
      if(attackOption->getOwnerId() != p->getID()) {
        canAttack.push_back(attackOption);
      }
    }
  }
  unordered_set<Territory*> uniqueSet(canAttack.begin(), canAttack.end());
  vector<Territory*> uniqueCanAttack;
  for(Territory* terr : uniqueSet) {
    uniqueCanAttack.push_back(terr);
  }

  std::sort(uniqueCanAttack.begin(), uniqueCanAttack.end(), [](Territory* t1, Territory* t2) {
    // Easier to conquer a territory with fewer armies to defend itself, these should be the priority
    return t1->getTotalArmyCount() < t2->getTotalArmyCount();
  });

  p->printTerritories(uniqueCanAttack);
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
    return t1->getTotalArmyCount() < t2->getTotalArmyCount();
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

  // deploy some reinforcements
  int reinforcements = p->getReinforcement();
  vector<Territory*> canDeploy = toDefend(p);
  if(reinforcements > 0) {
    if(canDeploy.size() > 0) {
      // we know at 0 is our strongest
      Deploy* deploy = new Deploy(p, canDeploy.at(0), reinforcements);
      canDeploy.at(0)->addToPendingArmyCount(reinforcements);
      p->getOrdersList()->addOrder(deploy);
    }
  }

  // Advance Part 1 - Reinforce
  // if there are armies in territories around our strongest, we will move them to strongest
  for(Territory* adjacentToStrongest : canDeploy.at(0)->getAdjacencyList()) {
    if(adjacentToStrongest->getOwnerId() == p->getID() && adjacentToStrongest->getTotalArmyCount() > 0) {
      p->getOrdersList()->addOrder(new Advance(p, adjacentToStrongest, canDeploy.at(0), adjacentToStrongest->getTotalArmyCount()));
    }
  }

  // Play Cards
  // Makes sense for Aggressive player to play cards before issueing advance orders
  auto deck = p->getHand()->getDeck();
  if (deck.size() > 0) {
    for(Card* card: deck) {
      string type = card->getType();
      if (type.compare("Bomb") == 0) {
      vector<Territory*> canAttack = p->toAttack();
      Territory* strongestAdjacent = canAttack.at(0);
      // best to bomb the enemy territory with the most armies
      for(Territory* territory : canAttack) {
        if(territory->getArmyCount() > strongestAdjacent->getArmyCount()) {
          strongestAdjacent = territory;
        }
      }
      p->getOrdersList()->addOrder(new Bomb(p, strongestAdjacent));
      } else if (type.compare("Reinforcement") == 0) {
          p->getOrdersList()->addOrder(new Deploy(p, canDeploy.at(0), 5));
      }
    }
  }
  // Advance Part2 - Attack
  // territories that are adjacent to strongest & not owned by us
  vector<Territory*> canAttack = toAttack(p);
  int totalArmies = canDeploy.at(0)->getTotalArmyCount();
  for(Territory* attack: canAttack) {
    if(totalArmies == 0) {
      break;
    }
    int armiesToSend;
    if(totalArmies > attack->getArmyCount() + 3) {
      armiesToSend = attack->getArmyCount() + 3;
    } else {
      armiesToSend = totalArmies;
    }
    totalArmies -= armiesToSend;
    p->getOrdersList()->addOrder(new Advance(p, canDeploy.at(0), attack, armiesToSend));
  }

  for(Territory * territory: canDeploy) {
    territory->resetPendingArmies();
  }
};

vector<Territory*> Aggressive::toAttack(Player* p) {
  // we need to find the strongest to attack 
  vector<Territory*> canDefend = toDefend(p);
  vector<Territory*> canAttack;
  if(canDefend.size() > 0) {
    Territory * strongest = canDefend.at(0);
    // look at the territories adjacent to the strongest that are not owned by you
    for(Territory* adjacentToStrongest :strongest->getAdjacencyList()) {
      if(adjacentToStrongest->getOwnerId() != p->getID()) {
        canAttack.push_back(adjacentToStrongest);
      }
    }
  }
  
  return canAttack;
};
vector<Territory*> Aggressive::toDefend(Player* p) {
  // return the territory with the most reinforcements first
  vector<Territory*> canDefend = p->getTerritories();

  std::sort(canDefend.begin(), canDefend.end(), [](Territory* t1, Territory* t2) {
    // Territories with most armies should be defended in priority
    return t1->getArmyCount() > t2->getArmyCount();
  });

  // return the sorted territories by army count
  return canDefend;
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
  // deploy some reinforcements
  int reinforcements = p->getReinforcement();
  vector<Territory*> canDeploy = toDefend(p);
  if(reinforcements > 0) {
    if(canDeploy.size() > 0) {
      // we know at 0 is our weakest
      Deploy* deploy = new Deploy(p, canDeploy.at(0), reinforcements);
      canDeploy.at(0)->addToPendingArmyCount(reinforcements);
      p->getOrdersList()->addOrder(deploy);
    }
  }

  // Advance - Reinforce
  vector<Territory*> reinforce;
  // if there are armies in territories around our weakest, we will move them to weakest
  for(Territory* adjacentToWeakest : canDeploy.at(0)->getAdjacencyList()) {
    if(adjacentToWeakest->getOwnerId() == p->getID() && adjacentToWeakest->getTotalArmyCount() > 0) {
        reinforce.push_back(adjacentToWeakest);
    }
  }
  for(Territory* territory : reinforce) {
    p->getOrdersList()->addOrder(new Advance(p, territory, canDeploy.at(0), territory->getTotalArmyCount()/2));
  }

  // Play Cards
  // Should only play Reinforce orders to do no harm
  auto deck = p->getHand()->getDeck();
  if (deck.size() > 0) {
    for(Card* card: deck) {
      string type = card->getType();
      if (type.compare("Airlift") == 0) {
        if(reinforce.size() > 1) {
        Order *order = new Airlift(p, reinforce.at(0), canDeploy.at(0), reinforce.at(0)->getArmyCount()/2);

        }
      } else if (type.compare("Reinforcement") == 0) {
          p->getOrdersList()->addOrder(new Deploy(p, canDeploy.at(0), 5));
      } 
    }
  }

  for(Territory * territory: canDeploy) {
    territory->resetPendingArmies();
  }
};
vector<Territory*> Benevolant::toAttack(Player* p) {
  // Doesn't need an implementation as the Benevolent player does not harm others
  vector<Territory*> territories;
  return territories;
};
vector<Territory*> Benevolant::toDefend(Player* p) {
  // return the territory with the least reinforcements first
  vector<Territory*> canDefend = p->getTerritories();

  std::sort(canDefend.begin(), canDefend.end(), [](Territory* t1, Territory* t2) {
    // Territories with least armies should be defended in priority
    return t1->getArmyCount() < t2->getArmyCount();
  });

  // return the sorted territories by army count
  return canDefend;
};