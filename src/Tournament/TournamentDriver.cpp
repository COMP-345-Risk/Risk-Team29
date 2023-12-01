#include "TournamentDriver.h"

/**
 * @brief runs tournaments based on following console inputs
 * tournament
 * -M <listofmapfiles>  Ex: -M Africa Europe solarSystem
 * -P <listofplayerstrategies>  Ex: -P Aggressive Benevolant Neutral Cheater
 * -G <numberofgames> Ex: -G 2
 * -D <maxnumberofturns> Ex: -D 10
 */
void testTournament(int argc, char* argv[]) {

    cout << "\n\n---------> TEST TOURNAMENT <---------\n\n";

    cout << "\n\n---------> TEST 1: Test Console Input <---------\n\n\n";

    //testConsole(argc, argv);


    cout << "\n\n---------> TEST 2: Tournament Board w/ Empty Shell <---------\n\n\n";

    //testBoard(argc, argv);


    cout << "\n\n---------> TEST 3: Play 1 Game <---------\n\n\n";
/*
    cout << "...Loading Map of Europe...\n";
    MapLoader* loader = new MapLoader();
    Map* map1 = loader->loadMap("Map/MapFolder/Europe.map");
    map1->validate();

    cout << "...Creating player vector with 2 players...\n";
    vector <Player*> players = createTwoPlayers();

    cout << "... Creating Command Proccesser ... \n\n";
    CommandProcessor* cp = new CommandProcessor();

    GameEngine* gs = new GameEngine(map1,players);

    gs->startupPhaseTournament(cp);

    // //set all owners to player 1 to test hasWinner()
    // for(auto t : gs->getMap()->getterritories()){
    //     t.second->setOwnerId(1);
    // }

    string winner = gs->playPhaseTournament(cp, 10);

    cout <<"The winner is: "<< winner <<"\n";
*/

    cout << "\n\n---------> TEST 4: Full Tournament <---------\n\n\n";

    testFullTournament(argc, argv);


}

/**
 * @brief Contains all tests for Console input
 */
void testConsole(int argc, char* argv[]){
    cout << "---- Scenario 1: actual arguments from command line -----\n\n";

    cout << "... Checking arguments in command line ...\n\n";
    cout << "***** Calling checkConsoleInputTournament() *****\n\n";
    checkConsoleInputTournament(argc, argv);
    cout << "\n\n";

    cout << "---- Scenario 2: Repeating Map -----\n\n";

    cout << "...Creating BAD -M with repeating maps \"Europe\" ...\n\n";
    char runCommand[] = "Users/.../ ";
    char tournament[] = "tournament";
    char optionM[] = "-M", Africa[] = "Africa", Europe[] = "Europe", solarSystem[] = "solarSystem", Europe2[] = "Europe";
    char optionP[] = "-P", Aggressive[] = "Aggressive", Benevolant[] = "Benevolant", Neutral[] = "Neutral", Cheater[] = "Cheater";
    char optionG[] = "-G", number2[] = "2";
    char optionD[] = "-D", number10[] = "10";

    char* tArgv[] = {
        runCommand,
        tournament,
        optionM, Africa, Europe, solarSystem, Europe2,
        optionP, Aggressive, Benevolant, Neutral, Cheater,
        optionG, number2,
        optionD, number10,
        nullptr
    };

    cout << "... Printing arguments and values... \n";
    int tArgc = 1;
    while (tArgv[tArgc] != nullptr) {
        cout << tArgv[tArgc] << " ";
        tArgc++;
    }
    cout << "\n\n";

    cout << "***** Calling checkConsoleInputTournament() *****\n\n";
    checkConsoleInputTournament(tArgc, tArgv);
    cout << "\n\n";

    cout << "---- Scenario 3: Wrong player stratagy -----\n\n";

    cout << "...Creating BAD -P with misseplt strategy \"Cheaterrrrr\" ...\n\n";
    char Cheater3[] = "Cheaterrrrr";

    char* t3Argv[] = {
        runCommand,
        tournament,
        optionM, Africa, Europe, solarSystem,
        optionP, Aggressive, Benevolant, Neutral, Cheater3,
        optionG, number2,
        optionD, number10,
        nullptr
    };

    cout << "... Printing arguments and values... \n";
    int t3Argc = 1;
    while (t3Argv[t3Argc] != nullptr) {
        cout << t3Argv[t3Argc] << " ";
        t3Argc++;
    }
    cout << "\n\n";

    cout << "***** Calling checkConsoleInputTournament() *****\n\n";
    checkConsoleInputTournament(t3Argc, t3Argv);
    cout << "\n\n";

    cout << "---- Scenario 4: Wrong number of -G games -----\n\n";

    cout << "...Creating BAD -G with \"20\" Games ...\n\n";
    char number20[] = "20";

    char* t4Argv[] = {
        runCommand,
        tournament,
        optionM, Africa, Europe, solarSystem,
        optionP, Aggressive, Benevolant, Neutral, Cheater,
        optionG, number20,
        optionD, number10,
        nullptr
    };

    cout << "... Printing arguments and values... \n";
    int t4Argc = 1;
    while (t4Argv[t4Argc] != nullptr) {
        cout << t4Argv[t4Argc] << " ";
        t4Argc++;
    }
    cout << "\n\n";

    cout << "***** Calling checkConsoleInputTournament() *****\n\n";
    checkConsoleInputTournament(t4Argc, t4Argv);
    cout << "\n\n";

}

/**
 * @brief checks if all arguments are valid based on following criteria:
 * tournament is first arguemnt
 * -M <listofmapfiles> has 1-5 different maps
 * -P <listofplayerstrategies> has 2-4 different stratagies
 * -G <numberofgames> has one value between 1-5
 * -D <maxnumberofturns> has one value between 10-50
 */
bool checkConsoleInputTournament(int argc, char* argv[]) {
    // check if console argument "tournament" is valid
    if (checkIfInsideConsoleIsTournamentMode(argc, argv)) {
        cout << "...Inside Tournament Mode ...\n\n";
    }
    else {
        cerr << "... Console argument \"tournament\" is incorrect or does not exist  ...\n"
            << "... Exiting checkConsoleInputTournament() ...\n\n";
        return false;
    }

    // collect arguments and values into a map data type
    map<string, vector<string> > args_and_values = collectArgumentsAndValuesFromConsole(argc, argv);

    //check all arguments are PRESENT
    if (checkIfAllArgumentsArePresent(args_and_values)) {
        cout << "... All arguments are present ...\n\n";
    }
    else {
        cerr << "... One of the arguments is missing ...\n\n"
            << "... Exiting checkConsoleInputTournament() ...\n\n";
        return false;
    }

    //check if arguments are VALID (-M, -P, -G, or -D)
    if (checkIfArgumentsAreValid(args_and_values)) {
        cout << "... All arguments syntax are valid ...\n\n";
    }
    else {
        cerr << "... Error: one of the arguments syntax is invalid ...\n"
            << "... Exiting checkConsoleInputTournament() ...\n\n";
        return false;
    }

    // check -M has 1-5 different maps
    int numMaps = args_and_values["-M"].size();
    if (!uniqueValues(args_and_values["-M"])) {
        cerr << "... Error: there are repeating maps ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    if (numMaps < 1 || numMaps > 5) { // must be between 1-5
        cerr << "... Error: the number of maps is too large ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    for (const auto& v : args_and_values["-M"]) {
        if (!(v.compare("Africa") == 0 || v.compare("cliff") == 0 || v.compare("Europe") == 0 ||
            v.compare("solarSystem") == 0 || v.compare("World") == 0)) {
            cerr << "... Error: one of the maps are invalid ...\n"
                << "... Exiting checkConsoleInputTournament() ....\n\n";
            return false;
        }
    }

    // check valid player has 2-4 different stratagies
    int numStrat = args_and_values["-P"].size();
    if (!uniqueValues(args_and_values["-P"])) {
        cerr << "... Error: there are repeating player stratagies ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    if (numStrat < 2 || numStrat > 4) { // must be between 1-5
        cerr << "... Error: the number of player stratagies is too large ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    for (const auto& v : args_and_values["-P"]) {
        if (!(v.compare("Aggressive") == 0 || v.compare("Benevolant") == 0 ||
            v.compare("Neutral") == 0 || v.compare("Cheater") == 0)) {
            cerr << "... Error: one of the player stratagies are invalid ...\n"
                << "... Exiting checkConsoleInputTournament() ....\n\n";
            return false;
        }
    }

    // check games has one value between 1-5
    if (args_and_values["-G"].size() > 1) {
        cerr << "... Error: number of games can only have 1 value ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    int numGames = stoi(args_and_values["-G"][0]);
    if (numGames < 1 || numGames > 5) { // must be between 1-5
        cerr << "... Error: the number of games is too large ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }

    // check number turns has one value between 10-50
    if (args_and_values["-D"].size() > 1) {
        cerr << "... Error: number of turns can only have 1 value ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }
    int numTurns = stoi(args_and_values["-D"][0]);
    if (numTurns < 10 || numTurns > 50) { // must be between 1-5
        cerr << "... Error: the number of turns is too large or small ...\n"
            << "... Exiting checkConsoleInputTournament() ....\n\n";
        return false;
    }

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

    return true;
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
 * @brief returns true if all arguments are present -M, -P, -D or -G
 */
bool checkIfAllArgumentsArePresent(map<string, vector<string> > args_and_values) {
    bool argM = false;
    bool argP = false;
    bool argD = false;
    bool argG = false;
    for (const auto& arg : args_and_values) {
        if ((arg.first.compare("-M") == 0))
            argM = true;
        if ((arg.first.compare("-P") == 0))
            argP = true;
        if ((arg.first.compare("-D") == 0))
            argD = true;
        if ((arg.first.compare("-G") == 0))
            argG = true;

    }
    return (argM && argP && argD && argG);
}

/**
 * @brief return true if console arguments are of valid syntax (-M, -P, -D or -G)
 *
 */
bool checkIfArgumentsAreValid(map<string, vector<string> > args_and_values) {

    for (const auto& arg : args_and_values) {
        //check if argument is -M, -P, -D or -G
        if (!(arg.first.compare("-M") == 0 || arg.first.compare("-P") == 0
            || arg.first.compare("-G") == 0 || arg.first.compare("-D") == 0)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief checks if vector values are unqique
 *
 * @param v
 * @return true
 * @return false
 */
bool uniqueValues(vector<string> values) {
    if (values.empty()) { return false; } // return false if empty
    bool unique = true;
    for (int i = 0; i < values.size(); ++i) {
        for (int j = i + 1; j < values.size(); ++j) {
            if (values[i].compare(values[j]) == 0) {
                unique = false;
                break;
            }
        }
    }
    return unique;
}

void testBoard(int argc, char* argv[]) {

    cout << "... Storing argument values in map...\n\n";
    map<string, vector<string> > args_and_values = collectArgumentsAndValuesFromConsole(argc, argv);

    cout << "... Creating tournament board as map of maps...\n\n";
    map<string, map<string, string> > tournBoard;

    vector<string> rows;
    vector<string> columns;

    // fill rows with maps
    for (string row : args_and_values["-M"]) {
        rows.push_back(row);
    }

    // fill columns with game #'s
    string sGame;
    for (int i = 0; i < stoi(args_and_values["-G"][0]); i++) {
        sGame = "Game ";
        columns.push_back(sGame.append(to_string(i + 1)));
    }

    // fill board
    int result = 0;
    string sResult;
    for (auto row : rows) {
        for (auto col : columns) {
            sResult = "Result ";
            tournBoard[row][col] = sResult.append(to_string(result));
            result++;
        }
    }

    // print board
    // print first row
    cout << "| " << setfill(' ') << setw(14) << "| ";
    for (auto col : columns) {
        cout << setfill(' ') << setw(12) << col << "| ";
    }
    cout << "\n";

    //print every other row
    for (const auto& row : rows) {
        cout << "| " << setfill(' ') << setw(12) << row << "| ";
        for (const auto& col : columns) {
            cout << setfill(' ') << std::setw(12) << tournBoard[row][col] << "| ";
        }
        cout << "\n";
    }
}

vector<Player*> createTwoPlayers(){
    vector<Territory*> t1;
    Hand* h1 = new Hand();
    OrdersList* pOL1 = new OrdersList();
    State* pState1 = new State("start");

    Player* p1 = new Player(t1, h1, pOL1, 1, pState1, "Lara", new Human()); // 1 is id

    vector<Territory*> t2;
    Hand* h2 = new Hand();
    OrdersList* pOL2 = new OrdersList();

    State* pState2 = new State("start");

    Player* p2 = new Player(t2, h2, pOL2, 2, pState2, "eyal", new Human()); // 1 is id

    vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);

    return players;
}

void testFullTournament(int argc, char* argv[]) {

    cout << "... Creating a Tournament Reading from Command line ...\n\n";

    cout << "... Checking arguments in command line ...\n\n";
    checkConsoleInputTournament(argc, argv);
   
    cout << "... Storing argument values in map...\n\n";
    map<string, vector<string> > args_and_values = collectArgumentsAndValuesFromConsole(argc, argv);

    cout << "... Create tournament board ...\n\n";
    map<string, map<string, string> > tournBoard = createTournament(args_and_values);

    cout << "... Printing tournament board ...\n\n";
    printTournamentBoard(tournBoard, args_and_values);

}

map<string, map<string, string> > createTournament(map<string, vector<string> > args_and_values) {
    
    map<string, map<string, string> > tournBoard;

    vector<string> rows;
    vector<string> columns;

    // fill rows with maps
    rows = getRows(args_and_values);

    // fill columns with game #'s
    columns = getColumns(args_and_values);

    // fill board
    for (auto mapName : rows) {
        for (auto col : columns) {
            tournBoard[mapName][col] = playGame(args_and_values, mapName);
        }
    }
    
    return tournBoard;
}

void printTournamentBoard(map<string, map<string, string> > tournBoard, map<string, vector<string> >  args_and_values) {

    vector<string> rows = getRows(args_and_values);
    vector<string> columns = getColumns(args_and_values);

    // print first row
    cout << "| " << setfill(' ') << setw(14) << "| ";
    for (auto col : columns) {
        cout << setfill(' ') << setw(12) << col << "| ";
    }
    cout << "\n";

    //print every other row
    for (const auto& row : rows) {
        cout << "| " << setfill(' ') << setw(12) << row << "| ";
        for (const auto& col : columns) {
            cout << setfill(' ') << std::setw(12) << tournBoard[row][col] << "| ";
        }
        cout << "\n";
    }
}

string playGame(map<string, vector<string> > args_and_values, string mapName) {

    cout << "...Loading Map of Europe...\n";
    MapLoader* loader = new MapLoader();
    string source = "Map/MapFolder/";
    source = source.append(mapName);
    source = source.append(".map");
    Map* map = loader->loadMap(source);
    map->validate();

    cout << "...Creating player vector with number of players...\n";
    vector <Player*> players = createPlayers(args_and_values);

    cout << "... Creating Command Proccesser ... \n\n";
    CommandProcessor* cp = new CommandProcessor();

    GameEngine* gs = new GameEngine(map, players);

    gs->startupPhaseTournament(cp);

    // //TODO: Remove this for actual Winner
    // for(auto t : gs->getMap()->getterritories()){
    //     t.second->setOwnerId(1);
    // }

    string winner = gs->playPhaseTournament(cp, 10);

    return winner;
    
}

vector <string> getRows(map<string, vector<string> > args_and_values) {
    vector<string> rows;
    // fill rows with maps
    for (string row : args_and_values["-M"]) {
        rows.push_back(row);
    }
    return rows;
}

vector <string> getColumns(map<string, vector<string> > args_and_values) {
    vector<string> columns;
    // fill columns with game #'s
    string sGame;
    for (int i = 0; i < stoi(args_and_values["-G"][0]); i++) {
        sGame = "Game ";
        columns.push_back(sGame.append(to_string(i + 1)));
    }
    return columns;
}

vector<Player*> createPlayers(map<string, vector<string> > args_and_values){
    
    vector<Player*> players;
    vector<string> playerStragies = args_and_values["-P"];
    int count = 1;
    for (string ps: playerStragies) {
        PlayerStrategy* psTemp = createPlayerStratagyObject(ps);
        players.push_back(new Player(new Hand(), new OrdersList(), count, new State("start"), ps, psTemp));
        count++;
    }

    return players;
}

PlayerStrategy* createPlayerStratagyObject(string ps) {
    PlayerStrategy* psObject;
    if (ps.compare("Aggressive") == 0) {
        psObject = new Aggressive();
    }
    else if (ps.compare("Benevolant") == 0) {
        psObject = new Benevolant();

    }
    else if (ps.compare("Neutral") == 0) {
        psObject = new Neutral();
    }
    else if (ps.compare("Cheater") == 0) {
        psObject = new Cheater();
    }else{
        psObject = new Human();
    }
    return psObject;
}