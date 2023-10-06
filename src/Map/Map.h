#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <map>

using namespace std;

/************************************************************ Continent ************************************************************/

class Continent {
private:
    int id;
    string name;

public:

    /// <summary>
    /// Default Constructor
    /// </summary>
    Continent();

    /// <summary>
    /// Param Constructor
    /// </summary>
    Continent(string, int);

    string getName() const;

    int getId() const;
};

/************************************************************ Territory ************************************************************/


class Territory {
private:
    int ownerId;
    int armyCount;
    int id;
    int continentId;
    string name;
    vector<Territory*> adjacencyList;
public:
    /// <summary>
    /// Default constructor
    /// </summary>
    Territory();

    /// <summary>
    /// Param constructor
    /// </summary>
    Territory(string name, int id, int continentId, int armyCount);

    // Getter for name
    string getName() const;

    // Getter for value
    int getId() const;

    // Getter for contientId
    int getContinentId() const;

    // Getter for adjacencyList
    vector<Territory*> getAdjacencyList();

    /// <summary>
    /// imagine we have A,B,C which are all objects of territory
    /// if we call A.addAdjacentTerritory(B) then:
    /// 1. We will keep track of B from A
    /// 2. Keep track of A from B
    /// hence we keep track of adjecent territories
    /// </summary>
    void addAdjacentTerritory(Territory*);
};
/************************************************************ Map ************************************************************/

class Map {
private:
    map<int, Territory*> territories;
    map<int, Continent*> continents;

public:
    /// <summary>
    /// Default Constructor
    /// </summary>
    Map();

    /// <summary>
    /// Copy constructor
    /// </summary>
    Map(const Map&);

    /// <summary>
    /// Param constructor
    /// </summary>
    Map(map<int, Continent*> continents, map<int, Territory*> territories);


    /// <summary>
    /// Decconstructor
    /// </summary>
    ~Map();

    void addContinent(Continent*);

    void addTerritory(Territory*);

    /// <summary>
    /// Print the summary of map
    /// </summary>
    void printMapSummary();

    /// <summary>
    /// Validate the map structure
    /// </summary>
    bool validate();
};



/************************************************************ MapDriver ************************************************************/

void testLoadMaps();

/************************************************************ MapLoader ************************************************************/


class MapLoader {
public:
    Map* loadMap(string);
    /// <summary>
    /// Parse the .map file the map is in, by seperating each line with a delimiter 
    /// The method will return a vector of words
    /// For ex: 123,232,353 will be [123,232,353] so we can manupulate each item in the vector as such v.at(i)
    /// </summary>
    vector<string> split(string s, string delim);
};