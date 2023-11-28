
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../OrdersList/Orders.h"


/************************************************************ State **************************************************************/
/**
 * Default Constructor
 */
State::State() {}
/**
 * Destructor
 */
State::~State() {}
/**
 * Constructor with an argument(s)
 */
State::State(string name) {
    stateName = name;
}
/**
 * Assignment Operator
 */
State& State::operator=(const State& other) {
    this->stateName = other.stateName;
    return *this;
}
/**
 * Copy Constructor
 */
State::State(const State& other) {
    this->stateName = other.stateName;
}
/**
 * overide Stream insertion operator
 */
ostream& operator<<(ostream& out, State* s) {
    cout << "📍 You are in " << s->stateName << "\n";
    return out;

}
string State::getStateName() {
    return stateName;
}

void State::setStateName(string sName) { this->stateName = sName; }


/************************************************************ Transition **************************************************************/
/**
 * Default Constructor
 */
Transition::Transition() {}
/**
 * Destructor
 */
Transition::~Transition() {}
/**
 * Constructor with with an argument(s)
 */
Transition::Transition(string requiredCommand, State* finalState) {
    this->command = requiredCommand;
    this->nextState = finalState;
}
/**
 * Assignment Operator
 */
Transition& Transition::operator=(const Transition& other) {
    this->command = other.command;
    this->nextState = other.nextState;
    return *this;
}
/**
 * Copy Constructor
 */
Transition::Transition(const Transition& other) {
    this->command = other.command;
    this->nextState = other.nextState;
}
/**
 * overide Stream insertion operator
 */
ostream& operator<<(ostream& out, Transition* t) {
    cout << "The " << t->command << " leads to the next state of : " << t->nextState->getStateName() << "\n";
    return out;

}
string Transition::getCommand() {
    return command;
}
State* Transition::getNextState() {
    return nextState;
}


/************************************************************ Game Engine **************************************************************/

GameEngine::GameEngine() {
    // set state to start
    currentState = new State("start");
    // define the default transitions
    gameTransitions = initializeGameTransitionsV2();
}


/**
 * Init game engine
*/
GameEngine::GameEngine(Map* map, vector<Player*> players) {
    this->gameStates = initializeGameStates();
    this->currentState = gameStates[0];
    this->gameTransitions = initializeGameTransitionsV2();
    this->loadedMap = loadedMap;
    this->players = players;
}

vector<State*> GameEngine::getGameStates() { return gameStates; }

State* GameEngine::getCurrentState() { return currentState; }

void GameEngine::setCurrentState(State* s) { this->currentState = s; }

map<string, map <string, Transition*> > GameEngine::getGameTransitions() { return gameTransitions; }

Map* GameEngine::getMap() { return this->loadedMap; }

vector<Player*> GameEngine::getPlayers() { return this->players; }

GameEngine::~GameEngine() {
    // todo delete all the game transitions 
}

/**
 * @brief Contains reinforcementPhase(), issueOrdersPhase() and executeOrdersPhase()
 *
 * Currently the Game loop is designed to execute each phase 2 times in a loop.
 *
 */
void GameEngine::mainGameLoop() {

    string inputCommand = "assignreinforcement";
    string previousStateName = "playersadded";
    int count = 0; // will be removed
    while (inputCommand.compare("win") != 0 && count < 6) {
        //TODO: Tried using map "gameTransitions", could not figure it out, will try again in future
        if ((previousStateName.compare("playersadded") == 0 && inputCommand.compare("assignreinforcement") == 0)
            || (previousStateName.compare("executeorders") == 0 && inputCommand.compare("issueorders") == 0)) {
            previousStateName = getGameStates().at(4)->getStateName(); // assignreinforcement
            reinforcementPhase();
            inputCommand = "issueorder";

        }
        else if ((previousStateName.compare("assignreinforcement") == 0 && inputCommand.compare("issueorder") == 0)
            || (previousStateName.compare("issueorders") == 0 && inputCommand.compare("issueorder") == 0)) {
            previousStateName = getGameStates().at(5)->getStateName(); // issueorders
            issueOrdersPhase();
            inputCommand = "issueordersend";

        }
        else if ((previousStateName.compare("issueorders") == 0 && inputCommand.compare("issueordersend") == 0)
            || (previousStateName.compare("executeorders") == 0 && inputCommand.compare("execorder") == 0)) {
            previousStateName = getGameStates().at(6)->getStateName(); // executeorders
            executeOrdersPhase();
            map<int, Territory*> territories = loadedMap->getterritories();
            int ownerId = territories.begin()->second->getOwnerId();
            bool isAllSameOwner = true;
            // check if all territories have the same owner id 
            for (auto const& t : territories) {
                if (t.second->getOwnerId() != ownerId) {
                    // if owner isnt the same for at least one territory, no player has won yet
                    isAllSameOwner = false;
                    break;
                }
            }
            // take the first territory, get the owner and go over the territories to see if they all have the same owner, if yes, end game, else go to reinforcement 
            inputCommand = isAllSameOwner ? "win" : "assignreinforcement";
        }
        else {
            cout << "Invalid choice.\n";
        }
        count++;
    }
}


/**
* @brief Players given  number  of  army  units (#  of  territories  owned  divided  by  3,  rounded  down)
* Each player gets a minumum of 3 army per turn.
* If player owns all territories in continent they get a reinforcements bonus.
*
*/
void GameEngine::reinforcementPhase() {
    for (auto player : getPlayers()) {
        int numTerritories = player->getTerritories().size();
        if (numTerritories / 3 <= 3) { // between 0-11 territories owned
            player->addReinforcement(3);
            cout << "Added 3 reinforcments to Player #" << player->getID() << "\n";
        }
        else {
            player->addReinforcement(numTerritories / 3); // 12 and over territories owned (round down)
            cout << "Added " << numTerritories / 3 << " reinforcments to Player #" << player->getID() << "\n";
        }
        addReinforcmentBonus(player);
    }
    cout << "\n";
}

void GameEngine::addReinforcmentBonus(Player* p) {
    for (auto id : continentIDsPlayerOwns(p)) { //get the ids of each continent player owns
        if (continentIDsPlayerOwns(p).size() != 0) {
            int bonusReinforcements = loadedMap->getContinent(id)->getReinforcementBonus();
            p->addReinforcement(bonusReinforcements); // reinforcment bonus
            cout << "Added " << bonusReinforcements << " bonus reinforcments to Player #" << p->getID()
                << " from " << loadedMap->getContinent(id)->getName() << "\n";
        }
    }
}

vector<int> GameEngine::continentIDsPlayerOwns(Player* p) {
    vector<int> cPlayerOwns;
    for (const auto& c : loadedMap->getContinents()) { //check every continent
        if (playerOwnsAllTerritoriesInContinent(c.second->getId(), p)) // use id to see if player owns
            cPlayerOwns.push_back(c.second->getId()); //add to list
    }
    return cPlayerOwns;
}

bool GameEngine::playerOwnsAllTerritoriesInContinent(int cID, Player* p) {
    for (auto t : loadedMap->getContinentTerritories(cID)) {
        if (t->getOwnerId() != p->getID()) {
            return false; // if one of territory owners is not player return false
        }
    }
    return true;
}

/**
 * @brief The player issues deploy orders on its own territories that are in the list returned by toDefend(). As long
 * as  the  player  has  army  units  in their  reinforcement  pool.
 */
void GameEngine::issueOrdersPhase() {
    OrdersList* ordersToExecute = new OrdersList();
    string userInput;

    for (auto p : players) {
        p->issueOrder(nullptr); // quick fix due to lack of time, will simply not use the param - no time to refactor
    }
}

bool GameEngine::hasMoreReinforcementsPlayers() {
    for (auto p : players) {
        if (p->getReinforcement() > 0)
            return true;
    }
    return false;
}

Order* GameEngine::getPlayerInputOrder(Player* p) {
    Order* o;
    cout << "Which order would you like to make, please type the number of on the followig options?\n";
    cout << "1)Deploy \n2)Advance \n3)Card \n";
    int input;
    do {
        cin >> input;
        switch (input) {
        case 1:
            std::cout << "You chose option 1\n";
            break;
        case 2:
            std::cout << "You chose option 2\n";
            break;
        case 3:
            std::cout << "You chose option 3\n";
            break;
        default:
            std::cout << " ❌ Not a valid choice, please trye again\n";
            break;
        }
    } while (input != 1 || input != 2 || input != 3);

    return o;

}


void GameEngine::executeOrdersPhase() {
    // vector of orders and push all the deploy order + execute them
    // go over each player
    for (Player* player : players) {
        OrdersList* ol = player->getOrdersList();
        for (Order* o : *ol->getOL()) {
            // if order type is deploy
            if (o->getOrderName().compare("Deploy") == 0) {
                //execute them right away
                o->execute();
                ol->remove(o->getOrderID());
            }
        }
    }
    // round robin over the players and call execute on their next order until there is no more order to execute 
      // vector of orders and push all the deploy order + execute them
    int currentPlayer = 0;
    vector<int> playersWhoFinishedOrders;
    while (playersWhoFinishedOrders.size() != players.size()) {
        vector<Order*>* orders = players.at(currentPlayer)->getOrdersList()->getOL();
        if (orders->size() > 0) {
            orders->at(0)->execute();
            players.at(currentPlayer)->getOrdersList()->remove(orders->at(0)->getOrderID());
        }
        else {
            // if no orders left for the player and it hasnt been already pushed
            int playerID = players.at(currentPlayer)->getID();

            // player isnt found
            if (std::find(playersWhoFinishedOrders.begin(), playersWhoFinishedOrders.end(), playerID) == playersWhoFinishedOrders.end()) {
                playersWhoFinishedOrders.push_back(playerID);
            }
        }
        // round robin the players/territories
        currentPlayer = (currentPlayer + 1) % (players.size());
    }


}

ostream& operator << (ostream& out, GameEngine* ge)
{

    out << "********** Printing current state **********\n"
        << ge->getCurrentState();

    out << "********** Printing game states **********\n";
    int count = 0;
    for (auto s : ge->getGameStates()) {
        out << count++ << " " << s->getStateName() << "\n";
    }
    out << "********** Printing Transitions **********\n";
    for (const auto& pair1 : ge->getGameTransitions()) {
        for (const auto& pair2 : pair1.second) {
            out << "previousState: " << pair1.first << " | " << pair2.second;
        }
    }
    out << "********** Printing Players **********\n";
    for (auto p : ge->getPlayers()) {
        out << p;
    }
    out << "********** Printing Map **********\n";
    ge->getMap()->printMapSummary();

    return out;
}


/************************************************************ Helper Functions **************************************************************/

/**
 * This method initializes the game states and then stores them in a Vector
 */
vector<State*> initializeGameStates() {
    vector<State*> gameStates;

    State* startState = new State("start");
    State* maploadedState = new State("maploaded");
    State* mapvalidatedState = new State("mapvalidated");
    State* playersaddedState = new State("playersadded");
    State* assignreinforcementState = new State("assignreinforcement");
    State* issueordersState = new State("issueorders");
    State* executeordersState = new State("executeorders");
    State* winState = new State("win");
    State* endState = new State("end");

    gameStates.push_back(startState); // 0
    gameStates.push_back(maploadedState); // 1
    gameStates.push_back(mapvalidatedState); // 2
    gameStates.push_back(playersaddedState); // 3
    gameStates.push_back(assignreinforcementState); // 4
    gameStates.push_back(issueordersState); // 5
    gameStates.push_back(executeordersState); // 6
    gameStates.push_back(winState); // 7
    gameStates.push_back(endState); // 8

    return gameStates;
}
/**
 * This method initializes the game transitions and then stores them in a Vector
 */
vector<Transition*> initializeGameTransitions() {
    vector<State*> states = initializeGameStates();
    vector<Transition*> gameTransitions;

    Transition* t1 = new Transition("loadmap", states[1]); // mapLoadedState
    Transition* t2 = new Transition("loadmap", states[1]); // mapLoadedState
    Transition* t3 = new Transition("validate", states[2]); // mapvalidatedState
    Transition* t4 = new Transition("addplayer", states[3]); // playersaddedState
    Transition* t5 = new Transition("addplayer", states[3]); // playersaddedState
    Transition* t6 = new Transition("assigncountries", states[4]); // assignreinforcementState
    Transition* t7 = new Transition("issueorder", states[5]); // issueordersState
    Transition* t8 = new Transition("issueorder", states[5]); // issueordersState
    Transition* t9 = new Transition("endissueorders", states[6]); // executeordersState
    Transition* t10 = new Transition("execorder", states[6]); // executeordersState
    Transition* t11 = new Transition("endexecorders", states[4]); // assignreinforcementState
    Transition* t12 = new Transition("win", states[7]); // winState
    Transition* t13 = new Transition("play", states[0]); // startState
    Transition* t14 = new Transition("end", states[8]); // endState

    gameTransitions.push_back(t1);
    gameTransitions.push_back(t2);
    gameTransitions.push_back(t3);
    gameTransitions.push_back(t4);
    gameTransitions.push_back(t5);
    gameTransitions.push_back(t6);
    gameTransitions.push_back(t7);
    gameTransitions.push_back(t8);
    gameTransitions.push_back(t9);
    gameTransitions.push_back(t10);
    gameTransitions.push_back(t11);
    gameTransitions.push_back(t12);
    gameTransitions.push_back(t13);
    gameTransitions.push_back(t14);

    return gameTransitions;
}

/**
 * Initialize all possible transitions in a double map
 * There is a map of all possible "current states" --> which each hold a map of possible commandName that the user can enter
 * that are valid.
 * In return, the value of each [currentState][commandName] is a transition object that returns the "next" state
 */
map<string, map<string, Transition*>> initializeGameTransitionsV2() {
    vector<State*> states = initializeGameStates();
    map<string, map<string, Transition*>> transitions;
    //transition[currentState][commandName] => nextTransition
    transitions["start"]["loadmap"] = new Transition("loadmap", states[1]); // maploaded
    transitions["maploaded"]["loadmap"] = new Transition("loadmap", states[1]); // maploaded
    transitions["maploaded"]["validatemap"] = new Transition("validatemap", states[2]); // mapvalidated
    transitions["mapvalidated"]["addplayer"] = new Transition("addplayer", states[3]); // playersadded
    transitions["playersadded"]["addplayer"] = new Transition("addplayer", states[3]); // playersadded
    transitions["playersadded"]["gamestart"] = new Transition("gamestart", states[4]); // assignreinforcement
    transitions["assignreinforcement"]["issueorder"] = new Transition("issueorder", states[5]); // issueorders
    transitions["issueorders"]["issueorder"] = new Transition("issueorder", states[5]); // issueorders
    transitions["issueorders"]["issueordersend"] = new Transition("issueordersend", states[6]); // executeorders
    transitions["executeorders"]["execorder"] = new Transition("execorder", states[6]); // executeorders
    transitions["executeorders"]["endexecorders"] = new Transition("endexecorders", states[4]); // assignreinforcement
    transitions["executeorders"]["win"] = new Transition("win", states[7]); // win
    transitions["win"]["quit"] = new Transition("quit", states[8]); // end
    transitions["win"]["replay"] = new Transition("replay", states[0]); // start


    return transitions;
}


void GameEngine::startupPhase(CommandProcessor* processor) {
    cout << "\n\n---------> Welcome to ＲＩＳＫ 🎲 <---------\n\n";
    currentState = new State("start");
    cout << currentState;

    cout << "....Here is list of maps to select from, please type the number of the map you want!....\n";
    cout << "1) 🗺️ Africa.map\n2) 🗺️ solarSystem.map\n3) 🗺️ Europe.map\n4) 🗺️ cliff.map\n5) 🗺️ World.map\n6) 🗺️ cliffError.map\n7) 🗺️ solarSystemErrorContinent.map\n\n";


    Command* c = processor->getCommand();
    gameTransitions = initializeGameTransitionsV2();

    // ---------> Step 1: Load the map <---------
    bool isValid = processor->validate(c, currentState);
    // if its a valid command (part of the possible commands a user can have) and in the proper state
    if (isValid && gameTransitions[currentState->getStateName()].count(c->getName()) > 0) {
        // the only valid command in this state would be loadmap
        MapLoader* loader = new MapLoader();
        loadedMap = loader->loadMap("Map/MapFolder/" + c->getParam());
        if (loadedMap == NULL) {
            cout << "❌ Not a valid map configuration or invalid file name";
            return;
        }
        // should now be in maploaded state
        transition(c->getName());
    }
    else {
        // invalid command
        cout << "❌ Not a valid command given the current state";
        return;
    }
    // ---------> Step 2: Validate the map <---------
    cout << "🗺️ Please validate your selected map by typing `validatemap`\n";
    c = processor->getCommand();
    isValid = processor->validate(c, currentState);
    if (isValid && gameTransitions[currentState->getStateName()].count(c->getName()) > 0) {
        // validate the map
        if (loadedMap->validate()) {
            // sets the new state to map validated as its the next state following
            transition(c->getName());
        }
    }
    else {
        // invalid command
        cout << "❌ Not a valid command given the current state";
        return;
    }

    // ---------> Step 3: Add players <---------
    string numberOfPlayers = "";
    cout << "🤼 Please enter the amount of players you would like to have: \n";
    getline(cin, numberOfPlayers);
    int parsedNumOfPlayers = stoi(numberOfPlayers);
    if (parsedNumOfPlayers < 2) {
        cout << "❗ The number of players can't be less than 2, setting number of players to 2!\n";
        parsedNumOfPlayers = 2;
    }
    if (parsedNumOfPlayers > 6) {
        cout << "❗ The number of players cant be more than 6, setting number of players to 6!\n";
        parsedNumOfPlayers = 6;
    }
    cout << "\n 🧑 Please add the players by calling `addplayer <playername>\n";
    for (int i = 0; i < parsedNumOfPlayers; i++) {
        c = processor->getCommand();
        isValid = processor->validate(c, currentState);
        //validate the command the user entered, if invalid will break the game
        if (isValid && gameTransitions[currentState->getStateName()].count(c->getName()) > 0) {
            Player* p = new Player(i);
            p->setName(c->getParam());
            players.push_back(p);
            transition(c->getName()); // will go in a loop for playersadded
        }
        else {
            // the command is not `addplayer` or it will not have a param after it for the players name
            cout << "❌ Not a valid command given the current state";
            return;
        }
    }
    // print to the user the state of the names
    for (int i = 0; i < parsedNumOfPlayers; i++) {
        cout << "\n" << players.at(i);
    }


    // ---------> Step 4: SETUP THE GAME START <---------
    cout << "\n 👍 You're all set! use the gamestart command to start playing!\n";
    c = processor->getCommand();
    isValid = processor->validate(c, currentState);
    //validate the command the user entered, if invalid will break the game
    if (isValid && gameTransitions[currentState->getStateName()].count(c->getName()) > 0) {
        cout << "\n... ⚙️ Setting Up ⚙️ ...\n";
        // SubStep 1: Distribute all the territories
        int currentPlayer = 0;
        for (auto const& territory : loadedMap->getterritories()) {
            players[currentPlayer]->addTerritory(territory.second);
            territory.second->setOwnerId(players[currentPlayer]->getID());
            // round robin the players/territories
            currentPlayer = (currentPlayer + 1) % (parsedNumOfPlayers);
        }
        cout << "\n .... ✅ Territories Distributed .... \n";

        // SubStep 2: Randomly shuffle the order of players
        // plant a seed
        vector<Player*> newOrder;
        srand((unsigned)time(NULL));
        for (int i = players.size() - 1; i >= 0; i--) {
            int random = rand() % i + 1; // random number between 0-i
            newOrder.push_back(players.at(i));
            players.erase(players.begin() + i);
        }
        players = newOrder;

        cout << "\n .... ✅ Players are shuffled .... \n";
        cout << "\n .... ✅ Players order is determined like so: .... \n";
        for (int i = 0; i < players.size(); i++) {
            Player* p = players.at(i);
            cout << i + 1 << ") " << p->getName() << "\n";
        }


        // SubStep 3: give 50 army units to the players
        for (Player* p : players) {
            p->setReinforcement(50);
        }
        // print to the user the state of the names
        for (int i = 0; i < parsedNumOfPlayers; i++) {
            cout << "\n💪 Reinforcement is set to: " << players.at(i)->getReinforcement() << "\n";
        }
        cout << "\n .... ✅ Initial armies are setup .... \n";

        // SubStep 4:  draw 2 inital cards per player
        Deck newDeck;
        newDeck.fillDeck(); // fills up with 15 cards
        for (Player* p : players) {
            for (int i = 0; i < 2; ++i) {
                newDeck.draw(*p->getHand());
            }
        };

        // print to the user the state of the names
        for (int i = 0; i < parsedNumOfPlayers; i++) {
            cout << "\n" << players.at(i);
        }

        cout << "\n .... ✅ Hand distributed .... \n";
        // transition to the game satrt
    }
    cout << "\n👍 You're all set up! use the gamestart command to start playing!\n";
    transition(c->getName());
    cout << "\n\n---------> Finish start up phase 🎲 <---------\n\n";

}

/**
 * Transition if the command is proper (will be a key in the second map)
*/
void GameEngine::transition(string command) {
    currentState = gameTransitions[currentState->getStateName()][command]->getNextState();
    cout << currentState;
    notify(this);
}


GameEngine& GameEngine::operator=(const GameEngine& other) {
    this->loadedMap = other.loadedMap;
    this->currentState = other.currentState;
    this->gameStates = other.gameStates;
    this->gameTransitions = other.gameTransitions;
    this->players = other.players;
    return *this;
}

/**
 * Override the stringToLog method to print about the order
*/
string GameEngine::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nChange of current state to: " + currentState->getStateName() + "\n\n------------------------------------------------------------------------------------------\n\n";
};
