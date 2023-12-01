#include "../GameEngine/GameEngine.h"
#include "../Map/Map.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../CardsDeck/Cards.h"
#include "../OrdersList/Orders.h"
#include "../Player/Player.h"

void testTournament(int argc, char* argv[]); //A3

// test 1
void testConsole(int argc, char* argv[]);
bool checkConsoleInputTournament(int argc, char* argv[]);
bool checkIfInsideConsoleIsTournamentMode(int argc, char* argv[]);
map<string, vector<string> > collectArgumentsAndValuesFromConsole(int argc, char* argv[]);
bool checkIfAllArgumentsArePresent(map<string, vector<string> > args_and_values);
bool checkIfArgumentsAreValid(map<string, vector<string> > args_and_values);
bool uniqueValues(vector<string> values);

// test 2
void testBoard(int argc, char* argv[]);

// test 3
vector<Player*> createTwoPlayers();

// test 4
void testFullTournament(int argc, char* argv[]);
map<string, map<string, string> > createTournament(map<string, vector<string> > args_and_values);
void printTournamentBoard(map<string, map<string, string> > tournBaord, map<string, vector<string> > args_and_values);
string playGame(map<string, vector<string> > args_and_values, string mapName);
vector <string> getRows(map<string, vector<string> > args_and_values);
vector <string> getColumns(map<string, vector<string> > args_and_values);
vector<Player*> createPlayers(map<string, vector<string> > args_and_values);
PlayerStrategy* createPlayerStratagyObject(string ps);