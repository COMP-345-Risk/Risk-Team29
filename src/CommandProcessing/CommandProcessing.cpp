#include "CommandProcessing.h"

using namespace std;
/************************************************************ Command **************************************************************/

/**
 * Default
 */
Command::Command() {};

/**
 * Param Constructor
 */
Command::Command(string n) {
    name = n;
}

Command::Command(string n, string p) {
    name = n;
    param = p;
}

/**
 * Copy constrcutor
 */
Command::Command(const Command& c) {
    effect = c.effect;
}

/**
 * Destructor
 */
Command::~Command() {};

string Command::getEffect() {
    return effect;
};

string Command::getName() {
    return name;
};

/**
 * Save the effect of the command
 */
void Command::saveEffect(string eff) {
    effect = eff;
    notify(this);
};

/**
 * Override the string to log
*/
string Command::stringToLog(){ 
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\nCommand with name: " + name + " was saved with the following effect " +  effect + "\n\n------------------------------------------------------------------------------------------\n\n";
}


/**
 * Assignement operator
 */
Command& Command::operator=(const Command& other) {
    effect = other.effect;
    name = other.name;
    return *this;
};

/**
 * certain commands like "loadmap" and "addplayer" have a second paramater after them
 */
string Command::getParam() {
    return param;
}

/**
 * Override the stream operator for Command
 */
ostream& operator<<(ostream& out, Command* c) {
    cout << "....Effect of the " << c->name << " command: " << c->effect
        << "....\n";
    return out;
}

/************************************************************ CommandProcessor ***************************************************************/

/**
 * Default
 */
CommandProcessor::CommandProcessor() {};
/**
 * Copy constrcutor
 */
CommandProcessor::CommandProcessor(const CommandProcessor& c) {
    for (int i = 0; i < c.commands.size(); i++) {
        commands.push_back(new Command(*(commands.at(i))));
    }
};
/**
 * Destructor
 */
CommandProcessor::~CommandProcessor() {
    for (int i = 0; i < commands.size(); i++) {
        Command* command = commands.at(i);
        delete command;
        command = NULL;
    }
};

/**
* Get command from the user/file
* Read and save it after
*/
Command* CommandProcessor::getCommand() {
    string command = readCommand();
    vector<string> line = split(command, " ");
    string commandName = line[0];
    string commandParam = "";
    if(line.size() > 1) {
        commandParam = line[1];
    }
    //separate the user input if there's more than 1 argument
    // save as command 
    // save input
    Command* c = new Command(commandName, commandParam);
    saveCommand(c);
    return c;
};



CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    commands = other.commands;
    return *this;
};

/**
 * Check if the command is valid in the current game state. If the command is
 * not valid, a corresponding error message should be saved in the effect of the
 * command.
 */
bool CommandProcessor::validate(Command* c, State* currentState) {
    string commandName = c->getName();
    string currentStateName = currentState->getStateName();
    // map regex checks if the loadmap command is written properly and if there is a file name afterwards 
    // NOTE: it doesnt check for the syntax of the file name, the file wont be able to open if its an invalid path
    if (commandName == "loadmap" && !(c->getParam().empty())) {
        if (currentStateName.compare("start") == 0 ||
            currentStateName.compare("maploaded") == 0) {
            return true;
        }
    }
    else if (commandName == "validatemap") {
        if (currentStateName.compare("maploaded") == 0) {
            return true;
        }
    }
    else if (commandName == "addplayer" && !c->getParam().empty()) {
        if (currentStateName.compare("mapvalidated") == 0 ||
            currentStateName.compare("playersadded") == 0) {
            return true;
        }
    }
    else if (commandName == "gamestart") {
        if (currentStateName.compare("playersadded") == 0) {
            return true;
        }
    }
    else if (commandName == "replay" || commandName == "quit") {
        if (currentStateName.compare("win") == 0) {
            return true;
        }
    }

    c->saveEffect("Invalid " + commandName + " command. Game is in invalid state: " + currentStateName);
    return false;
};

/**
 * Save into the private collection of commands
 */
void CommandProcessor::saveCommand(Command* c) {
    commands.push_back(c);
    notify(this);
}
/**
 * Override the string to log
*/
string CommandProcessor::stringToLog(){ 
    return "\n\n----------------------------------------- Logger -----------------------------------------\n\n" + commands.back()->getName() + " was saved to the collection of commands\n\n------------------------------------------------------------------------------------------\n\n";
}

/**
 * Stores a collection oF Command ojects
 * will read commands from the command line
 */
string CommandProcessor::readCommand() {
    // Asking for input
    cout << "Please enter one of the commands:\n loadmap <mapfile> \n validatemap \n addplayer <playername> \n gamestart \n replay \n quit \n";
    string userInput;
    getline(cin, userInput);

    cout << "\nYou have entered: " << userInput << "\n\n";
    return userInput;
};

/**
 * TODO DOCS
*/
vector<string> CommandProcessor::split(string line, string delim) {

    vector<string> words;
    int start = 0;
    string word;

    // Continue while a delimiter is found
    while ((start = line.find(delim)) != string::npos) {
        // Remove the delimiter
        word = line.substr(0, start);
        // If word exists, add it to the array of words to return
        if (word != "") {
            words.push_back(word);
        }
        // Remove the stuff I have already found
        line.erase(0, start + delim.length());
    }

    // For the last word (ex: 1̶,̶2̶,̶3) where "1,2," were already covered
    if (!line.empty()) {
        words.push_back(line);
    }

    return words;
}

/**
 * Override the stream operator for CommandProcessor
 */
ostream& operator<<(ostream& out, CommandProcessor* c) {
    out << "The current commands in the processor are: ";
    for (int i = 0; i < c->commands.size(); i++) {
        // Command has already an overriden operator to print the name of the
        // command and its effect
        out << c;
    }
    return out;
}

/************************************************************* FileCommandProcessorAdapter ***************************************************************/

/**
 * Default
 */
 // todo: do i need to call the default con here??!?!
FileCommandProcessorAdapter::FileCommandProcessorAdapter() : CommandProcessor::CommandProcessor() {
};
/**
 * Constructor with Params
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string fn) {
    fileName = fn;
    inputFile.open(fileName);
};

/**
 * Copy constrcutor
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& a) : CommandProcessor::CommandProcessor(a) {
    fileName = a.fileName;
};
/**
 * Destructor
 */
 // TODO: how can i call the base descturo
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    inputFile.close();
};

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    // TODO how to call the base?! 
    fileName = other.fileName;
    return *this;
};

/**
 * Override the stream operator for CommandProcessor
 */
ostream& operator<<(ostream& out, FileCommandProcessorAdapter* c) {
    return out;
};

/**
 *   Will read commands from the text file
 *  TODO: should this also save the command or return the list ?
 */
string FileCommandProcessorAdapter::readCommand() {
    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        cout << "...Error: Failed to open the file...\n";
        return ""; // Exit the program
    }
    else {
        if (getline(inputFile, line) && !inputFile.eof()) {
            cout << "\nThe command read was: " << line << "\n\n";
            return line;

        }
        else {
            cout << "FileCommandProcessorAdapter EOF";
            return "";
        }
    };
}

/************************************************************* FileCommandProcessorAdapter ***************************************************************/
void testCommandProcessor(bool isConsole, string fileName);