#include <algorithm>
#include <iostream>
#include <random>
#include "Cards.h"

/************************************************************ Card ************************************************************/
/**
* 'cardTypes' vector holds all possible card types
*/
const vector<string> Card::cardTypes = {"Bomb", "Reinforcement", "Blockade", "Airlift", "Diplomacy"};
// forward declaration
class Player;

/**
* Default constructor
*/
Card::Card() {
    type = nullptr;
}

/**
* Destructor
*/
Card::~Card() {
    delete type;
    type = nullptr;
}

/**
* Parameterized constructor
*/
Card::Card(const string &cardType) {
    type = nullptr;
    if (isValidCardType(cardType)) {
        type = new string(cardType);
    } else {
        cout << "Invalid card type: " << cardType << "\n";
    }
}

/**
* Deep copy constructor
*/
Card::Card(const Card &other) {
    type = nullptr;
    if (other.type != nullptr) {
        type = new string(*(other.type));
    }
}

/**
* Assignment operator
*/
Card &Card::operator=(const Card &other) {
    if (this != &other) {
        delete type;
        type = nullptr;
        if (other.type != nullptr) {
            type = new string(*(other.type));
        }
    }
    return *this;
}

/**
* Method returns true if card type is valid and false if type is not present in cardTypes vector
*/
bool Card::isValidCardType(const string &value) {
    for (size_t i = 0; i < cardTypes.size(); i++) {
        if (value == cardTypes[i]) {
            return true;
        }
    }
    return false;
}

Order* Card::play(Player* player) {
    if (type->compare("Bomb") == 0) {
        cout << "Which Territory would you like to Bomb?\n";
        vector<Territory*> canAttack = player->toAttack();
        string userInput;
        getline(cin, userInput);
        int targetTerritory = stoi(userInput);
        Order *order = new Bomb(player, canAttack.at(targetTerritory));
        return order;
    } else if (type->compare("Reinforcement") == 0) {
        cout << "Which Territory would you like to add 5 reinforcements to?\n";
        vector<Territory*> canDefend = player->toDefend();
        string userInput;
        getline(cin, userInput);
        int destinationTerritory = stoi(userInput);
        Order *order = new Deploy(player, canDefend.at(destinationTerritory), 5);
        return order;
    } else if (type->compare("Blockade") == 0) {
        cout << "Which Territory would you like to Blockade?\n";
        vector<Territory*> canDefend = player->toDefend();
        string userInput;
        getline(cin, userInput);
        int destinationTerritory = stoi(userInput);
        // todo make neutralPlayer just be no player (can we do this?)
        Order *order = new Blockade(player,player,canDefend.at(destinationTerritory));
        return order;
    } else if (type->compare("Airlift") == 0) {
        cout << "Which Territory would you like to Airlift TO?\n";
        vector<Territory*> canDefend = player->toDefend();
        string userInput;
        getline(cin, userInput);
        int destinationTerritory = stoi(userInput);
        cout << "Which Territory would you like to Airlift FROM?\n";
        player->printTerritories(canDefend);
        getline(cin, userInput);
        int fromTerritory = stoi(userInput);
        cout << "How many armies would you like to Airlift? (max: " << canDefend.at(fromTerritory)->getArmyCount() << "\n";
        getline(cin, userInput);
        int armiesToLift = stoi(userInput);
        // todo make neutralPlayer just be no player (can we do this?)
        Order *order = new Airlift(player, canDefend.at(destinationTerritory), canDefend.at(fromTerritory), armiesToLift);
        return order;
    } else if (type->compare("Diplomacy") == 0) {
        cout << "Choose another player to Negotiate with\n";
        //todo make players static on the GameEngine to be able to access it here.
        Order *order = new Negotiate(player, player);
        return order;
    } else {
        cout << "Invalid card type: " << type << "\n";
        return NULL;
    }
}

/**
* Method outputs card type
*/
string Card::getType() const {
    if (type != nullptr) {
        return *type;
    } else {
        return " ";
    }
}

/**
* Method sets new card type
*/
void Card::setType(const string &newType) {
    if (isValidCardType(newType)) {
        if (type == nullptr) {
            type = new string(newType);
        } else {
            *type = newType;
        }
    } else {
        cout << "Invalid card type!\n";
    }
}

/************************************************************ Deck ************************************************************/
/**
* Default constructor
*/
Deck::Deck() {

}

/**
* Destructor
*/
Deck::~Deck() {

}

/**
* Deep copy constructor
*/
Deck::Deck(const Deck &other) {
    for (size_t i = 0; i < other.deck.size(); ++i) {
        deck.push_back(new Card(*(other.deck[i])));
    }
}

/**
* Assignment operator
*/
Deck &Deck::operator=(const Deck &other) {
    if (this != &other) {
        for (size_t i = 0; i < other.deck.size(); ++i) {
            deck.push_back(new Card(*(other.deck[i])));
        }
    }
    return *this;
}

/**
* Method fills a deck with three cards of each type
*/
void Deck::fillDeck() {
    for (int i = 0; i < 3; i++) {
        for (size_t j = 0; j < cardTypes.size(); j++) {
            Card *newCard = new Card(cardTypes[j]);
            deck.push_back(newCard);
        }
    }
}

/**
* Method outputs a list of deck cards with their respective number and type
 *
*/
void Deck::printDeck() {
    cout << "The Deck contains: \n-------------------\n";
    for (size_t i = 0; i < deck.size(); i++) {
        const Card *pointer = deck[i];
        cout << "Card #" << i << " : " << pointer->getType() << "\n";
    }
}

/**
* Method outputs size of deck
*/
void Deck::getDeckSize() {
    cout << "The Deck contains " << deck.size() << " cards.\n";
}

/**
* Method randomizes card order in deck
*/
void Deck::shuffleDeck() {
    random_device random;
    default_random_engine seed(random());
    shuffle(deck.begin(), deck.end(), seed);
}

/**
* Method randomizes deck, draws a card from it, adds said card to hand through a temporary vector,
 * and removes card from deck of origin
*/
Card *Deck::draw(Deck &transfer) {
    shuffleDeck();

    if (deck.empty()) {
        cout << "Cannot draw card; The Deck is empty!\n";
        return nullptr;
    }

    Card *drawnCard = deck.back();

    deck.pop_back();

    transfer.addCard(drawnCard);

    return drawnCard;
}

/**
* Method adds card from deck to temporary vector
*/
void Deck::addCard(Card *transferCard) {

    transfer.push_back(transferCard);
}

/**
* Method returns a card to the deck of origin
*/
void Deck::returnCard(Card *returningCard) {

    deck.push_back(returningCard);
}

/************************************************************ Hand ************************************************************/
/**
* Default constructor
*/
Hand::Hand() {

}

/**
* Destructor
*/
Hand::~Hand() {
    cout << "...Hand destructor was called..." << endl;
}

/**
* Deep copy constructor
*/
Hand::Hand(const Hand &other) {
    for (size_t i = 0; i < other.hand.size(); ++i) {
        hand.push_back(new Card(*(other.hand[i])));
    }
}

/**
* Assignment operator
*/
Hand &Hand::operator=(const Hand &other) {
    if (this != &other) {
        for (size_t i = 0; i < other.hand.size(); ++i) {
            hand.push_back(new Card(*(other.hand[i])));
        }
    }
    return *this;
}

/**
* Method outputs a list of cards in a player's hand
*/
void Hand::printHand() {
    cout << "Your Hand contains: \n-------------------\n";
    for (size_t i = 0; i < hand.size(); i++) {
        const Card *pointer = hand[i];
        cout << "Card #" << i << " : " << pointer->getType() << "\n";
    }
}

/**
* Method outputs size of player's hand
*/
void Hand::getHandSize() {
    cout << "Your Hand contains " << hand.size() << " cards.\n";
}

/**
* Method adds a card to player's hand
*/
void Hand::addCard(Card *newHandCard) {
    hand.push_back(newHandCard);
}

Card* Hand::getCard(int index) {
    return hand.at(index);
}

/**
* Play method takes card type and deck of origin as arguments
 * Declares a list of orders
 * Valid orders include: deploy, advance, bomb, blockade, airlift, and negotiate
 * Once a card is player, we create the order, remove the card from hand and return it back to the deck
*/
void Hand::play(string &playedCardType, Deck *returningDeck) {
    OrdersList *OL = new OrdersList(); // declare a list of orders
    auto it = find_if(hand.begin(), hand.end(), [&playedCardType](const Card *card) {
        return card->getType() == playedCardType;
    });

    cout << "You played the " << "\"" << playedCardType << "\" card.\n";

    if (it != hand.end()) {
        if (playedCardType == "Bomb") {
            auto *order = new Bomb();
            OL->addOrder(order);
            cout << "\n" << OL;
        } else if (playedCardType == "Reinforcement") {
            auto *order = new Deploy();
            OL->addOrder(order);
            cout << "\n" << OL;
        } else if (playedCardType == "Blockade") {
            auto *order = new Blockade();
            OL->addOrder(order);
            cout << "\n" << OL;
        } else if (playedCardType == "Airlift") {
            auto *order = new Airlift();
            OL->addOrder(order);
            cout << "\n" << OL;
        } else if (playedCardType == "Diplomacy") {
            auto *order = new Negotiate();
            OL->addOrder(order);
            cout << "\n" << OL;
        } else {
            cout << "Invalid card type: " << playedCardType << "\n";
        }
        returningDeck->returnCard(*it);
        hand.erase(it);
    } else {
        cout << "Error: Card of type " << "\"" << playedCardType << "\"" << " not found in hand!\n";
    }
}

/**
 * Override the stream operator for Card
 */
ostream& operator << (ostream& out, Hand* h) {
    out << "The Hand contains \n------------------------\n";
    int handSize = h->hand.size();
    if(handSize == 0 ) {
        out << "Hand is empty\n";
    }
    else {
        for(int i = 0 ; i < h->hand.size(); i++){
            out << "🃏 Card type: " << h->hand.at(i)->getType() << "\n";
        }
    }

    return out;
}

