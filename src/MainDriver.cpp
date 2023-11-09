#include "Map/Map.h"
#include "Player/Player.h"
#include "OrdersList/Orders.h"
#include "CardsDeck/Cards.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"

int main(int argc, char* argv[])
{
    /*
    * Assignment 1
    */
    //testLoadMaps();
    //testOrdersLists();
    //testPlayers();
    //testCards();
    // testGameStates(); 

    /*
    * Assignment 2
    */
    // testOrderExecution();
    // argv[0] is the name of the program hence the second param will be what we need (flag)
    if (argc > 1) {
        string flag = argv[1];
        bool isCommandLine = flag.compare("-console") == 0;
        testCommandProcessor(isCommandLine, argc == 3 ? argv[2] : "");
        return 0;
    }

    // testLoggingObserver();
}
