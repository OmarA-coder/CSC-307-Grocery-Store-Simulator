#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <vector>
#include "GroceryItem.h"
#include "AVLTree.h"
#include <iostream>
#include <iomanip>

class ShoppingCart {
private:
    std::vector<GroceryItem> items;
    double totalCost;
    int totalItems;

public:
    ShoppingCart() : totalCost(0.0), totalItems(0) {}
    
    void addItem( AVLTree& inventory,  const std::string& upcCode, int quantity) {
        GroceryItem* item = inventory.search(upcCode);
        if (item == nullptr) {
            std::cout << "Item not found in inventory." << std::endl;
            return;
        }
        if (item -> getQuantity() < quantity) {
            std::cout << "Not enough stock available for"<< item ->getName() << "available."<< std::endl;
            return;
        }
        item -> setQuantity(item -> getQuantity() - quantity);
        inventory.update(upcCode, *item);

        for (GroceryItem& cartItem : items) {
            if (cartItem.getUpcCode() == upcCode) {
                cartItem.setQuantity(cartItem.getQuantity() + quantity);
                totalCost += item->getPrice() * quantity;
                totalItems += quantity;
                return;
            }
        }
        GroceryItem itemToAdd = *item;
        itemToAdd.setQuantity(quantity);
        items.push_back(itemToAdd);
        totalCost += item->getPrice() * quantity;
        totalItems += quantity; 
    }
    
    void removeItem(AVLTree& inventory, const std::string& upcCode, int quantity) {
        GroceryItem* item = inventory.search(upcCode);
        if (item == nullptr) {
            std::cout << "Item not found in inventory." << std::endl;
            return;
        }
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getUpcCode() == upcCode) {
                if (it->getQuantity() < quantity) {
                    std::cout << "Not enough items in cart to remove." << std::endl;
                    return;
                }
                it->setQuantity(it->getQuantity() - quantity);
                totalCost -= item->getPrice() * quantity;
                totalItems -= quantity;
                
                if (it->getQuantity() == 0) {
                    items.erase(it);
                }
                item->setQuantity(item->getQuantity() + quantity);
                inventory.update(upcCode, *item);
                return;
            }
        }
        
        std::cout << "Item not found in cart." << std::endl;
    }

    void displayCart() const {
        std::cout << std::fixed << std::setprecision(2);
        for (const GroceryItem& item : items) {
            std::cout << "UPC: " << item.getUpcCode() 
                      << ", Name: " << item.getName() 
                      << ", Price: $" << item.getPrice() 
                      << ", Quantity: " << item.getQuantity() 
                      << ", Aisle: " << item.getAisle() 
                      << std::endl;
        }
        std::cout << "Total Cost: $" << totalCost << std::endl;
        std::cout << "Total Items: " << totalItems << std::endl;
    }
    void clearCart() {
        items.clear();
        totalCost = 0.0;
        totalItems = 0;
    }
    double getTotalCost() const {
        return totalCost;
    }
    int getTotalItems() const {
        return totalItems;
    }
    
};

#endif 