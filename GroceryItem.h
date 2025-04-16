#ifndef GROCERY_ITEM_H
#define GROCERY_ITEM_H

#include <string>

class GroceryItem {
private:
    std::string name;
    std::string upcCode;
    double price;
    int quantity;
    std::string aisle;

public:
    // Constructors
    GroceryItem() : name(""), upcCode(""), price(0.0), quantity(0), aisle("") {}
    
    GroceryItem(const std::string& name, const std::string& upcCode, double price, 
                int quantity, const std::string& aisle)
        : name(name), upcCode(upcCode), price(price), quantity(quantity), aisle(aisle) {}
    
    // Getters
    std::string getName() const { return name; }
    std::string getUpcCode() const { return upcCode; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    std::string getAisle() const { return aisle; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setUpcCode(const std::string& newUpc) { upcCode = newUpc; }
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setAisle(const std::string& newAisle) { aisle = newAisle; }
};

#endif 
