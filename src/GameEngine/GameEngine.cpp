#include "GameEngine.h"
#include "../Player/Player.h"


/************************************************************ State **************************************************************/
/**
 * Default Constructor
 */
    State::State(){}
/**
 * Destructor
 */
    State::~State(){}
/**
 * Constructor with an argument(s)
 */
    State::State(string name){ 
        stateName = name;
    }
/**
 * Assignment Operator
 */
    State& State::operator=(const State& other){
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
    ostream& operator<<(ostream& out, State* s){
        
        out << "State: " << (s->stateName) << "\n";
        return out;
    }

    string State::getStateName(){
        return stateName;
    }

    void State::setStateName(string sName){this->stateName=sName;}



/************************************************************ Transition **************************************************************/
/**
 * Default Constructor
 */
    Transition::Transition(){}
/**
 * Destructor
 */
    Transition::~Transition() {}
/**
 * Constructor with with an argument(s)
 */
Transition::Transition(string requiredCommand, State* finalState){
    this->command = requiredCommand;
    this->nextState = finalState;
}
/**
 * Assignment Operator
 */
    Transition& Transition::operator=(const Transition& other){
        this->command = other.command;
        this->nextState = other.nextState;
        return *this;
    }
/**
 * Copy Constructor
 */
    Transition::Transition(const Transition& other){
        this->command = other.command;
        this->nextState = other.nextState;
    }
/**
 * overide Stream insertion operator
 */
    ostream& operator<<(ostream& out, Transition* t){
        out << "Tramsition | command: " << t->command 
            << " | next" << t->nextState <<"\n";
        return out;

        cout << "The " << t->command << " leads to the next state of : " << t->nextState << "\n";
        return out;

    }
    string Transition::getCommand(){
        return command;
    }
    State * Transition::getNextState(){
        return nextState;
    }


/************************************************************ Game Engine **************************************************************/

GameEngine::GameEngine(){ }

GameEngine::GameEngine(Map *map, vector<Player*> players){
    this->gameStates = initializeGameStates();
    this->currentState = getGameStates().at(0);
    this->gameTransitions = initializeGameTransitionsV2();
    this->map = map;
    this->players = players;
}

vector<State*> GameEngine::getGameStates(){return gameStates;}

State* GameEngine::getCurrentState() { return currentState; }

void GameEngine::setCurrentState(State* s){this->currentState = s;}

map<string, map <string, Transition*> > GameEngine::getGameTransitions(){return gameTransitions;}

Map* GameEngine::getMap(){ return this->map; }

vector<Player*> GameEngine::getPlayers() { return this->players; }

GameEngine::~GameEngine(){}

/**
 * @brief Contains reinforcementPhase(), issueOrdersPhase() and executeOrdersPhase()
 * 
 * Currently the Game loop is designed to execute each phase 2 times in a loop. 
 *
 */
void GameEngine::mainGameLoop(){
    
    string inputCommand = "assignreinforcement";
    string previousStateName = "playersadded";
    int count = 0;
    while(inputCommand.compare("win")!=0 && count < 6){
        
        //TODO: Tried using map "gameTransitions", could not figure it out, will try again in future
        if ( (previousStateName.compare("playersadded") == 0 && inputCommand.compare("assignreinforcement") == 0)
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
            inputCommand = "issueorders";
        }
        else {
            cout << "Invalid choice.\n" ;
        }
        count++;
    }
}

/**
    * @brief Players given  number  of  army  units (#  of  territories  owned  divided  by  3,  rounded  down)
    * If player owns all territories in continent they get a control bonus. Each player gets a minumum of 3 army per turn.
    *
    */
void GameEngine::reinforcementPhase() {
    cout << "Hi from GameEnginereinforcementPhase\n";
    

}

void GameEngine::issueOrdersPhase(){
    cout << "Hi from issueOrdersPhase\n";
}

void GameEngine::executeOrdersPhase() {
    cout << "Hi from executeOrdersPhase\n";
}

ostream& operator << (ostream& out, GameEngine* ge)
{

    out << "********** Printing current state **********\n"
        << ge->getCurrentState();

    out << "********** Printing game states **********\n";
        int count =0;
        for(auto s: ge->getGameStates()){
            out << count++ << " "<< s;
        }
    out << "********** Printing Transitions **********\n";
    for (const auto& pair1 : ge->getGameTransitions()) {
        for (const auto& pair2 : pair1.second ) {
                out << "previousState: " <<pair1.first<<" | " << pair2.second;
        }
    }        
    out << "********** Printing Players **********\n";
    for(auto p : ge->getPlayers()){
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
    vector<State*> initializeGameStates(){
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
    vector<Transition*> initializeGameTransitions(){
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

    // loadmap and transition will store the next state for loadmap i.e: `maploaded`
    // todo: EYAL THIS IS TEMP, THIS SHOULD BE IN THE GAMEENGINE PARAMS, I WILL ADD IT SOON
    map<string, map<string, Transition*> > initializeGameTransitionsV2(){
        vector<State*> states = initializeGameStates();
        map<string, map<string, Transition*> > transitions;

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
