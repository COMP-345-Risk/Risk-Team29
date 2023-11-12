#pragma once
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <map>
#include "../CommandProcessing/CommandProcessing.h"
#include "../Map/Map.h"


// to avoid circular dependencies
class Player;
class FileCommandProcessorAdapter;
class CommandProcessor;
class Order;


using namespace std;

/************************************************************ State **************************************************************/
class State {
public:
    /**
     * Default Constructor
     */
    State();
    /**
     * Destructor
     */
    ~State();
    /**
     * Constructor with an argument(s)
     */
    State(string name);
    /**
     * Assignment Operator
     */
    State& operator=(const State& other);
    /**
     * Copy Constructor
     */
    State(const State& other);
    /**
     * overide Stream insertion operator
     */
    friend ostream& operator<<(ostream& out, State* s);

    string getStateName();

    void setStateName(string sName);

private:
    string stateName;
};

/************************************************************ Transition **************************************************************/
class Transition {
public:
    /**
     * Default Constructor
     */
    Transition();
    /**
     * Destructor
     */
    ~Transition();
    /**
     * Constructor with with an argument(s)
     */
    Transition(string requiredCommand, State* finalState);
    /**
     * Assignment Operator
     */
    Transition& operator=(const Transition& other);
    /**
     * Copy Constructor
     */
    Transition(const Transition& other);
    /**
     * overide Stream insertion operator
     */
    friend ostream& operator<<(ostream& out, Transition* t);

    string getCommand();

    State* getNextState();

private:
    string command;
    State* nextState;
};

/************************************************************ Game Engine **************************************************************/
class GameEngine : public Subject, public ILoggable  {
public:
    GameEngine();

    GameEngine(Map* map, vector<Player*> players);

    ~GameEngine();

    vector<State*> getGameStates();

    State* getCurrentState();

    void setCurrentState(State* s);

    map<string, map <string, Transition*> > getGameTransitions();

    Map* getMap();

    vector<Player*> getPlayers();

    GameEngine& operator=(const GameEngine& other);

    /**
     * @brief Contains reinforcementPhase(), issueOrdersPhase() and executeOrdersPhase()
     * 
     */
    void mainGameLoop();

    /**
     * @brief Players given  number  of  army  units (#  of  territories  owned  divided  by  3,  rounded  down)
     * If player owns all territories in continent they get a control bonus. Each player gets a minumum of 3 army per turn.
     * 
     */
    void reinforcementPhase();

    void addReinforcmentBonus(Player* p);

    vector<int> continentIDsPlayerOwns(Player* p);

    bool playerOwnsAllTerritoriesInContinent(int cID, Player *p);

    void issueOrdersPhase();

    bool hasMoreReinforcementsPlayers();

    Order* getPlayerInputOrder(Player *p);

    void executeOrdersPhase();

    void startupPhase(CommandProcessor * command);
    
    void transition(string command);

    string stringToLog();


private:
    vector<State*> gameStates; // all the game states
    State* currentState; // current state of the player
    map<string, map<string, Transition*>> gameTransitions;
    Map* map;
    vector<Player*> players;
    friend ostream& operator<<(ostream& out, GameEngine* ge);  // overide Stream insertion operator
};


/************************************************************ GameEngineDriver **************************************************************/
/**
 * This method initializes the game states and then stores them in a Vector
 */
vector<State*> initializeGameStates();
/**
 * This method initializes the game transitions and then stores them in a Vector
 */
vector<Transition*> initializeGameTransitions();


/************************************************************ GameEngineDriver **************************************************************/
void testGameStates(); //A1

void testMainGameLoop(); //A2
void testGameStates();
void testStartupPhase();

map<string, map<string, Transition*> > initializeGameTransitionsV2();
