#include "../GameEngine/GameEngine.h"
#include "../Map/Map.h"

void testTournament(int argc, char* argv[]); //A3

void testConsole(int argc, char* argv[]);
void checkConsoleInputTournament(int argc, char* argv[]);
bool checkIfInsideConsoleIsTournamentMode(int argc, char* argv[]);
map<string, vector<string> > collectArgumentsAndValuesFromConsole(int argc, char* argv[]);
bool checkIfAllArgumentsArePresent(map<string, vector<string> > args_and_values);
bool checkIfArgumentsAreValid(map<string, vector<string> > args_and_values);
bool uniqueValues(vector<string> values);

void testBoard(int argc, char* argv[]);