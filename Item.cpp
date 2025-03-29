#include "Item.h"
#include <iomanip>  //for setprecsion

using namespace std;

// Default constructor
Item::Item() : name(""), upcCode(""), price(0.0), aisle(""), quantity(0) {
}

// Parameterized constructor
Item::Item(string name, string upcCode, double price, string aisle, int quantity) 
    : name(name), upcCode(upcCode), price(price), aisle(aisle), quantity(quantity) {
}

// Copy constructor
Item::Item(const Item& other) 
    : name(other.name), upcCode(other.upcCode), price(other.price), 
      aisle(other.aisle), quantity(other.quantity) {
}

// Getters
string Item::getName() const {
    return name;
}

string Item::getUpcCode() const {
    return upcCode;
}

double Item::getPrice() const {
    return price;
}

string Item::getAisle() const {
    return aisle;
}

int Item::getQuantity() const {
    return quantity;
}

// Setters
void Item::setName(const string& name) {
    this->name = name;
}

void Item::setUpcCode(const string& upcCode) {
    this->upcCode = upcCode;
}

void Item::setPrice(double price) {
    this->price = price;
}

void Item::setAisle(const string& aisle) {
    this->aisle = aisle;
}

void Item::setQuantity(int quantity) {
    this->quantity = quantity;
}

// Modify quantity
void Item::addQuantity(int amount) {
    if (amount > 0) {
        quantity += amount;
    }
}

void Item::removeQuantity(int amount) {
    if (amount > 0 && amount <= quantity) {
        quantity -= amount;
    }
}

// Comparison operators
bool Item::operator<(const Item& other) const {
    return upcCode < other.upcCode;
}

bool Item::operator>(const Item& other) const {
    return upcCode > other.upcCode;
}

bool Item::operator==(const Item& other) const {
    return upcCode == other.upcCode;
}

bool Item::operator!=(const Item& other) const {
    return upcCode != other.upcCode;
}

// Assignment operator
Item& Item::operator=(const Item& other) {
    if (this != &other) {
        name = other.name;
        upcCode = other.upcCode;
        price = other.price;
        aisle = other.aisle;
        quantity = other.quantity;
    }
    return *this;
}

// Output stream operator
ostream& operator<<(ostream& os, const Item& item) {
    os << "Item: " << item.name << "\n"
       << "UPC: " << item.upcCode << "\n"
       << "Price: $" << fixed << setprecision(2) << item.price << "\n"
       << "Aisle: " << item.aisle << "\n"
       << "Quantity: " << item.quantity;
    return os;
}
