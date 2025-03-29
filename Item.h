#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

using namespace std;

class Item {
private:
    string name;       // Item name (e.g., "Banana")
    string upcCode;    // UPC code (unique identifier)
    double price;      // Price of the item
    string aisle;      // Which aisle the item belongs to
    int quantity;      // Available quantity in inventory
    
public:
    // Default constructor
    Item();
    
    // Parameterized constructor
    Item(string name, string upcCode, double price, string aisle, int quantity);
    
    // Copy constructor
    Item(const Item& other);
    
    // Getters
    string getName() const;
    string getUpcCode() const;
    double getPrice() const;
    string getAisle() const;
    int getQuantity() const;
    
    // Setters
    void setName(const string& name);
    void setUpcCode(const string& upcCode);
    void setPrice(double price);
    void setAisle(const string& aisle);
    void setQuantity(int quantity);
    
    // Modify quantity (for adding to or removing from inventory)
    void addQuantity(int amount);
    void removeQuantity(int amount);
    
    // Comparison operators for AVL tree (using UPC code as unique key)
    bool operator<(const Item& other) const;
    bool operator>(const Item& other) const;
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const;
    
    // Assignment operator
    Item& operator=(const Item& other);
    
    // Output stream operator for displaying item information
    friend ostream& operator<<(ostream& os, const Item& item);
};

#endif // ITEM_H
