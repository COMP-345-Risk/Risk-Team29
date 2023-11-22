#include "GameEngine.h"
#include "../Player/Player.h"
#include "../Map/Map.h"


void testGameStates() {

    cout << "\n************************************\nTesting Game Driver!!! \n************************************\n\n";

    cout << "\n\n---------> Test 1: Checking if states and transitions are initialized properly <---------\n\n\n";

    vector<State*> myStates = initializeGameStates();
    vector<Transition*> myTransitions = initializeGameTransitions();
    cout << "...Getting start state...\n";
    cout << myStates[0]->getStateName() << "\n";
    cout << "state has been printed.\n\n";

    cout << "...Getting commands associated with states...\n";
    for (int i = 0; i < myTransitions.size(); i++) {
        cout << myTransitions[i];
        //cout << myTransitions[i]->getCommand() << " leads to " << myTransitions[i]->getNextState() << ".\n";
    }
    cout << "commands have been printed.\n\n";

    cout << "\n\n---------> Test 2: string command comparsion <---------\n\n\n";

    cout << "...Testing command comparision...\n";
    string testLoadMapCommand = "loadmap";
    for (int i = 0; i < myTransitions.size(); i++) {
        if (testLoadMapCommand.compare(myTransitions[i]->getCommand()) == 0) {
            cout << "testLoadMapCommand equals " << myTransitions[i]->getCommand() << " command\n";
        }
    }

    cout << "...Testing Dummy Command...\n";
    string dummyCommand = "patato";
    bool invalid = false;
    for (int i = 0; i < myTransitions.size(); i++) {
        if (dummyCommand.compare(myTransitions[i]->getCommand()) != 0) {
            invalid = true;
        }
        if (invalid && i == myTransitions.size() - 1) {
            cout << "DummyCommand is NOT Valid\n";
        }
    }

    cout << "\n\n---------> Test 3: Assign CurrentState to newState <---------\n\n\n";

    cout << "...Creating currentState as start ...\n";
    State* currentState = myStates[0];
    cout << "currentState created.\n\n";

    cout << "...Printing Contents of current State...\n";
    cout << "Current state is : " << currentState->getStateName() << "\n\n";

    cout << "...assigning currentState using t1 nextState :maploaded ...\n";
    currentState = myTransitions[0]->getNextState();
    cout << "Current state is : " << currentState->getStateName() << "\n\n";

    cout << "\n\n---------> Test 3: Testing User Input <---------\n\n\n";

    cout << "...checking user input commands against game catalogue to allow or deny switch of states...\n";
    cout << "...creating start state...\n";
    State* cState = myStates[0];
    cout << "start state created\n";
    cout << "...Taking user input...\n";

    bool transComplete;
    string userCommand = "";
    while (userCommand.compare("end") != 0) {
        cout << "Enter one of the follwing commands:\n"
            "----------------------------\n"
            " loadmap\n validate\n addplayer\n assigncountries\n issueorder\n "
            "endissueorders\n execorder\n endexecorders\n win\n play\n end - to exit!\n"
            "----------------------------\n";
        cin >> userCommand;
        cout << "\n";
        transComplete = false;
        for (int i = 0; i < myTransitions.size(); i++) {
            if (userCommand.compare(myTransitions[i]->getCommand()) == 0) { // string command
                cState = myTransitions[i]->getNextState(); // assign oldstate to newState
                userCommand = myTransitions[i]->getCommand();
                transComplete = true;
                break;
            }
        }

        if (transComplete) {
            cout << "Current state is : " << cState->getStateName() << "\n\n";
            if (userCommand.compare("end") == 0)
                cout << "...Exiting testGameStates()...\n\n";
        }
        else {
            cout << "The command entered: " << userCommand << " is invalid. Try Again.\n\n";
        }
    }

    cout << "...Deleting State pointers and setting to null...\n";
    for (int i = 0; i < myStates.size(); i++) {
        delete myStates[i];
        myStates[i] = NULL;
    }
    cout << "Deleted State pointers and set to null\n\n";

    cout << "...Deleting Transition Pointers...\n";
    for (int i = 0; i < myTransitions.size(); i++) {
        delete myTransitions[i];
        myTransitions[i] = NULL;
    }
    cout << "Deleted Transition pointers and set to null\n\n";

}


/************************************************************ Test Driver A2 **************************************************************/

void testStartupPhase() {
    GameEngine * g = new GameEngine();
    FileLineReader * flr = new FileLineReader("setup.txt");
    CommandProcessor* processor = new FileCommandProcessorAdapter(flr);
    g->startupPhase(processor);
}
void testMainGameLoop(){

    // Create Map > Create 2 players > create gameEngine
    // GameLopp()

    cout << "********** Preliminary Phase: Create Map and Players *********\n\n"
        
         << "---------> Load Map: Europe <---------\nx\n";

    cout << "...Loading Map of Europe...\n";
    MapLoader* loader = new MapLoader();
    Map* map1 = loader->loadMap("Map/MapFolder/Europe.map");

    cout << "...Validating contents...\n";
    map1->validate();


    cout << "\n\n---------> Create Players vector 2 players <---------\n\n\n";

    vector<Territory*> t1;
    Hand* h1 = new Hand();
    OrdersList* pOL1 = new OrdersList();

    cout << "...Creating state executeorders (must be in this state to execute orders)...\n";
    State* pState1 = new State("executeorders");

    cout << "...Creating player 1 with t adjeceny list, Hand, and order list..\n";
    Player* p1 = new Player(t1, h1, pOL1, 1, pState1); // 1 is id

    vector<Territory*> t2;
    Hand* h2 = new Hand();
    OrdersList* pOL2 = new OrdersList();

    cout << "...Creating state executeorders (must be in this state to execute orders)...\n";
    State* pState2 = new State("executeorders");

    cout << "...Creating player 2 with t adjeceny list, Hand, and order list..\n";
    Player* p2 = new Player(t2, h2, pOL2, 2, pState2); // 1 is id

    cout << "...Creating player 1 with t adjeceny list, Hand, and order list..\n";
    vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);

    cout << "\n\n---------> Test reinforcementPhase() <---------\n\n\n";

    cout << "...Creating Game Engine with two players ..\n\n";
    GameEngine *ge = new GameEngine(map1, players);

    cout << "...Printing Western Europe Contient...\n";
    Continent* westernEurope = ge->getMap()->getContinent("Western Europe");
    cout << westernEurope;

    cout << "...Printing continents Western Europe owns...\n";
    for (auto t : ge->getMap()->getContinentTerritories("Western Europe"))
        cout << t;

    cout << "...Giving player 1 all territories in Western Europe ..\n\n";
    for (auto t : ge->getMap()->getContinentTerritories("Western Europe")) //bonus 5
        players[0]->addTerritory(t); 

    cout << "...Western Europe has a reinforcement bonus of " << westernEurope->getReinforcementBonus() << " ..\n\n";

    cout << "...Player 1 has " << ge->getPlayers()[0]->getReinforcement() << " reinforcements..\n\n";

    cout << "...Player 2 has " << ge->getPlayers()[1]->getReinforcement() << " reinforcements..\n\n";

    cout << "...Executing reinforcremntPhase()..\n\n";
    ge->reinforcementPhase();

    cout << "...Player 1 has " << ge->getPlayers()[0]->getReinforcement() << " reinforcements..\n\n";

    cout << "...Player 2 has " << ge->getPlayers()[1]->getReinforcement() << " reinforcements..\n\n";

    cout << "...Giving player 1 all territories in Balkans ..\n\n";
    for (auto t : ge->getMap()->getContinentTerritories("Balkans")) //bonus 5
        players[0]->addTerritory(t);

    cout << "...Player 1 owns 2 continents " << ge->getPlayers()[0]->getTerritories().size() << " territories ..\n\n";

    cout << "...Executing reinforcremntPhase()..\n\n";
    ge->reinforcementPhase();


    cout << "\n\n---------> Test issueOrdersPhase() <---------\n\n\n";


}


/******************************************** Test Tournament A3 **************************************************/


void testTournament(int argc, char* argv[]) {

    cout << "\n\n---------> TEST TOURNAMENT <---------\n\n";
    cout << "\n\n---------> TEST 1: Testing Console Input <---------\n\n\n";

    // check if console argument "tournament" is valid
    if(checkIfInsideConsoleIsTournamentMode(argc, argv)){
        cout <<"...Inside Tournament Mode ...\n\n";
        
    }else{
        cerr <<"... Console argument \"tournament\" is incorrect or does not exist  ...\n"
            <<"... Exiting testTournmant() ...\n\n";
        return;
    }

    // collect arguments and values into a map data type
    map<string, vector<string> > args_and_values 
        = collectArgumentsAndValuesFromConsole(argc, argv);

    //check if arguments are valid (-M, -P, -G, or -D)
    if(checkIfArgumentsAreValid(args_and_values)){
        cout <<"... All arguments syntax are valid ...\n\n"; 
    }else{
        cerr << "... Error: one of the arguments syntax is invalid ...\n"
            <<"... Exiting testTournament() ...\n\n";
        return;
    }


    //check valid maps

    // Display arguments and values
    // NOTE: map organizes keys alphabetically
    cout << "... Listing the arguments and their values ...\n";
    for (const auto& arg : args_and_values) {
        cout << arg.first << " : ";
        for (const auto& v : arg.second) {
            cout << v << " ";
        }
        cout << "\n";
    }

}

/**
 * @brief check if first console argument is tournament
 * 
 */
bool checkIfInsideConsoleIsTournamentMode(int argc, char* argv[]) {
    if (argc > 1) {
        // confirm console is tournment
        string tournament = "tournament";
        if (tournament.compare(argv[1]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief store arguements and respective values inside map data type
 * 
 * @param argc 
 * @param argv 
 * @return map<string, vector<string> > 
 */
map<string, vector<string> > collectArgumentsAndValuesFromConsole(int argc, char* argv[]) {
    map<string, vector<string> > args_and_values;

    // start at arg[2] because confirmed arg[1] is tournament
    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];

        if (arg.size() > 0 && arg[0] == '-') { // check NOT empty and equal to "-"
            vector<string> values;

            ++i; // move one position to get letter arg Ex: M, P, D or G

            // Collect all consecutive values until the next argument or end of input
            while (i < argc && argv[i][0] != '-') {
                values.push_back(argv[i]);
                ++i;
            }
            
            args_and_values.insert(pair<string, vector<string> >(arg, values));

            // Decrement to account for the next argument with "-"
            --i;
        }
    }
    return args_and_values;
}

/**
 * @brief return true if console arguments are of valid syntax (-M, -P, -D or -G)
 * 
 */
bool checkIfArgumentsAreValid(map<string, vector<string> > args_and_values){
    
    for (const auto& arg : args_and_values) {
        //check if argument is -M, -P, -D or -G
        if (!(arg.first.compare("-M") == 0 || arg.first.compare("-P") == 0 
                || arg.first.compare("-G") == 0 || arg.first.compare("-D") == 0) ) {
            return false;
        }
    }
    return true;
}