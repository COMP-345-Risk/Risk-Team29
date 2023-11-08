#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include "../GameEngine/GameEngine.h"
#include "../LoggingObserver/LoggingObserver.h"

using namespace std;
/************************************************************ Command **************************************************************/
class Command : public Subject, public ILoggable {
public:
    /**
     * Default
     */
    Command();
    /**
     * Param Constructor
     */
    Command(string name);
    /**
     * Copy constrcutor
     */
    Command(const Command& c);
    /**
     * Destructor
     */
    ~Command();

    string getEffect();

    string getName();

    /**
     * Save the effect of the command
    */
    void saveEffect(string eff);

    Command& operator=(const Command& other);

    string stringToLog();

private:
    string name;
    string effect;
    friend ostream& operator<<(ostream& out, Command* o);  // overide Stream insertion operator


};

/************************************************************ CommandProcessor **************************************************************/
class CommandProcessor : public Subject, public ILoggable {
public:
    /**
     * Default
     */
    CommandProcessor();
    /**
     * Copy constrcutor
     */
    CommandProcessor(const CommandProcessor& c);
    /**
     * Destructor
     */
    ~CommandProcessor();

    Command* getCommand();


    CommandProcessor& operator=(const CommandProcessor& other);

    /*
    * Validate if this command can be executed at this state of the game
    */
    bool validate(Command* c, State* currentState);

private:
    vector<Command*> commands;
    friend ostream& operator<<(ostream& out, CommandProcessor* o);  // overide Stream insertion operator
    /**
     * Stores a collection oF Command ojects
     * will read commands from the command line
    */
    virtual string readCommand();

    /**
     * Save into the private collection of commands
    */
    void saveCommand(Command* c);

    /**
     * Override string to log (observer pattern)
    */
    string stringToLog();


};
/************************************************************ FileCommandProcessorAdapter   **************************************************************/

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    /**
    * Default
    */
    FileCommandProcessorAdapter();
    /**
    * Constructor with Params
    */
    FileCommandProcessorAdapter(string fileName);
    /**
    * Copy constrcutor
    */
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& a);
    /**
    * Destructor
    */
    ~FileCommandProcessorAdapter();

    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);

private:
    string fileName;
    // track how we read the file
    ifstream inputFile;
    string line;

    friend ostream& operator<<(ostream& out, FileCommandProcessorAdapter* o);  // overide Stream insertion operator
    /**
     *   Will read commands from the text file
     */
    string readCommand();

};

/************************************************************ FileCommandProcessorAdapter   **************************************************************/
void testCommandProcessor(bool isConsole, string fileName);