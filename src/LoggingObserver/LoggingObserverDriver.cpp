#include "LoggingObserver.h"
#include "../OrdersList/Orders.h"
#include "../Player/Player.h"
#include "../CardsDeck/Cards.h"
#include "../CommandProcessing/CommandProcessing.h"


/************************************************************ LoggingDriver ************************************************************/

void testLoggingObserver() {
    // create the log observer
    LogObserver* logger = new LogObserver();

    // create test components (territory, hand, state, orderlist)
    vector <Territory*> territoryList;
    vector <Territory*> territoryList2;
    Territory* territory1 = new Territory("UK", 5, 3, 11);
    Territory* territory2 = new Territory("Bulgaria", 6, 3, 6);
    territoryList.push_back(territory2);
    territoryList2.push_back(territory1);

    State* executeordersState = new State("executeorders");
    State* executeordersState2 = new State("executeorders");

    Hand* h = new Hand();
    Hand* h2 = new Hand();

    OrdersList* o = new OrdersList;
    OrdersList* o2 = new OrdersList;

    Player* p = new Player(territoryList, h, o, 1, executeordersState);
    Player* p2 = new Player(territoryList2, h2, o2, 2, executeordersState2);

    p->addTerritory(territory1);
    p2->addTerritory(territory2);

    const int numberOfOrders = 6;

    Order* orderArray[numberOfOrders] = {
      new Deploy(p, territory2, 3), // Init Bulgaria with 6 and DEPLOY 3 = 9 total
      new Airlift(p, territory1, territory2, 2), // UK has 11, Bulgaria has 9, airlift 2 = UK has 9 & Bulgaria has 11
      new Blockade(p, p2, territory1), // UK army doubles from 9 to 18
      new Bomb(p, territory1), // half to 9
      new Advance(p, territory1, territory2, 9), // ?
      new Negotiate(p, p2)
    };

    /**
     * Set the order to valid by default
     * Attach logger to each order
     * Execute the order - expect the console and the file to be filled with logs
     */
    for (int i = 0; i < numberOfOrders; i++) {
        Order* order = orderArray[i];
        order->setValid(true);
        order->attach(logger);
        order->execute();
    }

    // test add the order to the player's orderlist 
    Deploy* d = new Deploy(p, territory2, 3);
    // attach modal to view 
    o->attach(logger);
    p->issueOrder(d); // should call Order::addOrder and notify the observer

    // test the save command 
    CommandProcessor * processor = new CommandProcessor();
    processor->attach(logger);
    Command* c = processor->getCommand();

    // test command effect
    c->attach(logger);
    c->saveEffect("new effect");


    // avoid dangeling pointers
    for (int i = 0; i < numberOfOrders; i++) {
        Order* order = orderArray[i];
        delete order;
        order = NULL;
    }

    delete territory2;
    territory2 = NULL;

    delete territory1;
    territory1 = NULL;

    delete p; // will delete all items that were passed to players constructor such as territories, hand, orderlist and state
    p = NULL;

    delete p2; // will delete all items that were passed to players constructor such as territories, hand, orderlist and state
    p2 = NULL;

    // command will be deleted inside of the processor 
    delete processor;
    processor = NULL;

    delete logger;
    logger = NULL;

}

// TODO: when merged - call `notify(this)` inside of each of those methods 
// CommandProcessor::saveCommand() - DONE
// Order::execute() --> DONE
// OrderList::addOrder() --> DONE
// Command::saveEffect() - DONE  - but effect doesnt print 
// GameEngine::transition() - TODO

//todo: When merged - Extend the following Classes from Subject, public ILoggable and override the stringToLogMethod
// Order - DONE 
// OrderList -DONE
// CommandProcessor - DONE
// Command - DONE
// GameEngine 
