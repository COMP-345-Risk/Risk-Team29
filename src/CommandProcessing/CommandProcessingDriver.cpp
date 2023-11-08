#include "CommandProcessing.h"

void testCommandProcessor(bool isConsole, string filename = "") {
    CommandProcessor* processor;
    if (isConsole) {
        processor = new CommandProcessor();
    }
    else {
        // call the adapter
        if (filename.empty()) {
            cout << "No file name provided to process commands from";
            return;
        }
        cout << "Reading Commands from " << filename << "\n";
        processor = new FileCommandProcessorAdapter(filename);
    }

    const int testStates = 7;
    State* states[testStates] = {
    new State("start"),
    new State("maploaded"),  // make sure to load invalid map
    new State("maploaded"),
    new State("mapvalidated"),
    new State("playersadded"),
    new State("playersadded"),
    new State("win"),
    };

    // if(isConsole) {
    //     cout << "\n\n---------> Test 1:Check the validity of the commands if the app is run by console <---------\n\n";
    //     for (int i = 0; i < testStates; i++) {
    //         cout << "\nFor console purposes the state right now is at: " + states[i]->getStateName() << "\n";
    //         Command* c = processor->getCommand();
    //         bool isValid = processor->validate(c, states[i]);
    //         cout << "\n................." << (isValid ? "Valid command" : "Invalid command") << ".................\n";
    //     }
    // } else {
    //     cout << "\n\n---------> Test 1:Check the validity of the commands if the app is run by saved file <---------\n\n";
    //     for (int i = 0; i < testStates; i++) {
    //         cout << "\nFor the file adapter the state right now is at: " + states[i]->getStateName() << "\n";
    //         Command* c = processor->getCommand();
    //         bool isValid = processor->validate(c, states[i]);
    //         cout << "\n................." << (isValid ? "Valid command" : "Invalid command") << ".................\n";
    //     }
    // }

    cout << "\n\n---------> Test 2: Test Game transitions <---------\n\n";
    // delete the old processor to be able to initialize the new processor and open the file to complete the second text
    delete processor;
    processor = new FileCommandProcessorAdapter("game.txt");
    map<string, Transition*> transitions = initializeGameTransitionsV2(); 
    State * state = new State("start");  
    cout << "Game should transition from start to maploaded";
    // returns "loadmap file1"
    Command* c = processor->getCommand();
    string stateName = state->getStateName();
    if(processor->validate(c,state)){
        cout << "Valid command " << c->getName() << "\nMoving from " 
        << stateName << " state to " << transitions.find(stateName)->second->getNextState()->getStateName() << " state";
        // save effect
        //todo + to_string(transitions.find(stateName)->getNextState());
        c->saveEffect("Command is valid, now in state " +  stateName + " and is moving to the next state: TODO");
        state = transitions.find(stateName)->second->getNextState();
    }
    else {
        //todo save effect
          cout << "Invalid command " << c->getName() << "\n thats currently is in state "  << stateName << " can't be transitioned";
          c->saveEffect("Invalid command " + c->getName() + "\n thats currently is in state " +  stateName + " can't be transitioned");
    }
    
    // todo add loop
    // observer

}