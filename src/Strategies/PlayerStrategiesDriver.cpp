#include "./PlayerStrategies.h"

void testPlayerStrategies() {
    // call the startup phase
    GameEngine * ge = new GameEngine();
    // CommandProcessor* processor = new CommandProcessor();
     FileLineReader * flr = new FileLineReader("setup.txt");
    CommandProcessor* processor = new FileCommandProcessorAdapter(flr);
    ge->startupPhase(processor);
    // call the main game loop
    ge->mainGameLoop();
    delete ge;
};