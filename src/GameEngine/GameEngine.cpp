#include "GameEngine.h"

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
        cout << "You are in " << s->stateName << "\n";
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
        cout << "The " << t->command << " leads to the next state of : " << t->nextState << "\n";
        return out;

    }
    string Transition::getCommand(){
        return command;
    }
    State * Transition::getNextState(){
        return nextState;
    }
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

        gameStates.push_back(startState);
        gameStates.push_back(maploadedState);
        gameStates.push_back(mapvalidatedState);
        gameStates.push_back(playersaddedState);
        gameStates.push_back(assignreinforcementState);
        gameStates.push_back(issueordersState);
        gameStates.push_back(executeordersState);
        gameStates.push_back(winState);
        gameStates.push_back(endState);

        return gameStates;
    }
/**
 * This method initializes the game transitions and then stores them in a Vector
 */
    vector<Transition*> initializeGameTransitions(){
        vector<State*> states = initializeGameStates();
        vector<Transition*> gameTransitions;

        Transition* t1 = new Transition("loadmap", states[1]);
        Transition* t2 = new Transition("loadmap", states[1]);
        Transition* t3 = new Transition("validate", states[2]);
        Transition* t4 = new Transition("addplayer", states[3]);
        Transition* t5 = new Transition("addplayer", states[3]);
        Transition* t6 = new Transition("assigncountries", states[4]);
        Transition* t7 = new Transition("issueorder", states[5]);
        Transition* t8 = new Transition("issueorder", states[5]);
        Transition* t9 = new Transition("endissueorders", states[6]);
        Transition* t10 = new Transition("execorder", states[6]);
        Transition* t11 = new Transition( "endexecorders", states[4]);
        Transition* t12 = new Transition("win", states[7]);
        Transition* t13 = new Transition("play", states[0]);
        Transition* t14 = new Transition( "end", states[8]);

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
    map<string, Transition*> initializeGameTransitionsV2(){
        vector<State*> states = initializeGameStates();
        map<string, Transition*> transitions;

        transitions["start"] = new Transition("loadmap", states[1]); 
        transitions["loadmap"] = new Transition("loadmap", states[1]); 
        transitions["validate"] = new Transition("validate", states[1]);
        transitions["addplayer"] = new Transition("addplayer", states[3]);
        transitions["assigncountries"] = new Transition("assigncountries", states[4]);
        transitions["issueorder"] = new Transition("issueorder", states[5]);
        transitions["endissueorders"] = new Transition("endissueorders", states[6]);
        transitions["execorder"] = new Transition("execorder", states[6]);
        transitions["endexecorders"] = new Transition( "endexecorders", states[4]);
        // todo, win should be replay or quit
        transitions["win"] = new Transition("win", states[7]);
        transitions["play"] = new Transition("play", states[0]);
        transitions["end"] = new Transition( "end", states[8]);

        return transitions;
    }