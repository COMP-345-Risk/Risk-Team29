#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include "../LoggingObserver/LoggingObserver.h"
#include "../GameEngine/GameEngine.h"

using namespace std;

class Subject;
class ILoggable;
class State;

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
    Command(string name, string param);
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

    string getParam(); // certain commands like "loadmap" and "addplayer" have a second paramater after them

    /**
     * Save the effect of the command
    */
    void saveEffect(string eff);

    Command& operator=(const Command& other);

    string stringToLog();

private:
    string name;
    string param; // some commands may have paramters such as a map name or a players name 
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

    vector<string> split(string line, string delim);

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
/************************************************************ FileLineReader   **************************************************************/
class FileLineReader {
public:
    FileLineReader(string fileName);
    ~FileLineReader();
    FileLineReader(const FileLineReader& other);
    FileLineReader& operator=(const FileLineReader& other);
    string readLineFromFile();

private:
    string fileName;
    // track how we read the file
    ifstream inputFile;
    string line;

    friend ostream& operator<<(ostream& out, FileLineReader* o);  // overide Stream insertion operator

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
    FileCommandProcessorAdapter(FileLineReader * flr);
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
    friend ostream& operator<<(ostream& out, FileCommandProcessorAdapter* o);  // overide Stream insertion operator
    /**
     *   Will read commands from the text file
     */
    FileLineReader* flr;
    string readCommand();

};


/************************************************************ FileCommandProcessorAdapter   **************************************************************/
void testCommandProcessor(bool isConsole, string fileName);