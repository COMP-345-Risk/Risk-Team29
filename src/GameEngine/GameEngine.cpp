
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"


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
    cout << "The " << t->command << " leads to the next state of : " << t->nextState << "\n";
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
    this->gameTransitions = initializeGameTransitionsV2();
    this->map = map;
    this->players = players;
}

Map* GameEngine::getMap() { return this->map; }

vector<Player*> GameEngine::getPlayers() { return this->players; }

GameEngine::~GameEngine() {
    // todo delete all the game transitions 
}


ostream& operator << (ostream& out, GameEngine* ge)
{

    out << "********** Printing Players **********";
    for (auto p : ge->getPlayers()) {
        out << p;
    }
    out << "********** Printing Map **********"
        << ge->getMap();

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
        map = loader->loadMap("Map/MapFolder/" + c->getParam());
        if (map == NULL) {
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
        if (map->validate()) {
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
        for (auto const& territory : map->getterritories()) {
            players[currentPlayer]->addTerritory(territory.second);
            territory.second->setOwnerId(players[currentPlayer]->getID());
            // round robin the players/territories
            currentPlayer = (currentPlayer + 1) % (parsedNumOfPlayers - 1);
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
            cout << "\n 💪 Reinforcement is set to: " << players.at(i)->getReinforcement() << "\n";
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
    cout << "\n 👍 You're all set up! use the gamestart command to start playing!\n";
    transition(c->getName());

}

/**
 * Transition if the command is proper (will be a key in the second map)
*/
void GameEngine::transition(string command) {
    currentState = gameTransitions[currentState->getStateName()][command]->getNextState();
    cout << currentState;
    notify(this);
}

/**
 * Override the stringToLog method to print about the order
*/
string GameEngine::stringToLog() {
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nChange of current state to: " + currentState->getStateName() + "\n\n------------------------------------------------------------------------------------------\n\n";
};
