#pragma once
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <map>

#include "../Map/Map.h"

class Player;

using namespace std;

/************************************************************ State **************************************************************/
class State{
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
class Transition{
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
	Transition(string requiredCommand, State*  finalState);
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

State * getNextState();

private:
    string command;
    State* nextState;
};

/************************************************************ Game Engine **************************************************************/
class GameEngine {
public:
    GameEngine();
  
    GameEngine(Map *map, vector<Player*> players);
    
    ~GameEngine();

    Map* getMap();

    vector<Player*> getPlayers();

    void mainGameLoop();

    void reinforcementPhase();

    void issueOrdersPhase();

    void executeOrdersPhase();


private:
    vector<State*> gameStates;
    map<string, map <string,Transition*> > gameTransitions;
    Map *map;
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

map<string, map<string, Transition*>> initializeGameTransitionsV2();
