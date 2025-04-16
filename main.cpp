#include <iostream>
#include <limits>
#include "AVLTree.h"

void displayMenu() {
    std::cout << "\n===== GROCERY STORE INVENTORY SYSTEM =====" << std::endl;
    std::cout << "1. Add New Item" << std::endl;
    std::cout << "2. Find Item by UPC" << std::endl;
    std::cout << "3. Update Item" << std::endl;
    std::cout << "4. Delete Item" << std::endl;
    std::cout << "5. Display All Items" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void addItem(AVLTree& inventory) {
    std::string name, upcCode, aisle;
    double price;
    int quantity;
    
    std::cout << "\n--- Add New Item ---" << std::endl;
    
    std::cout << "Enter UPC Code: ";
    std::cin >> upcCode;
    
    // Check if UPC already exists
    if (inventory.search(upcCode) != nullptr) {
        std::cout << "Error: Item with UPC " << upcCode << " already exists." << std::endl;
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter Name: ";
    std::getline(std::cin, name);
    
    std::cout << "Enter Price: $";
    std::cin >> price;
    
    std::cout << "Enter Quantity: ";
    std::cin >> quantity;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter Aisle: ";
    std::getline(std::cin, aisle);
    
    GroceryItem newItem(name, upcCode, price, quantity, aisle);
    inventory.insert(newItem);
    
    std::cout << "Item added successfully!" << std::endl;
}

void findItem(AVLTree& inventory) {
    std::string upcCode;
    
    std::cout << "\n--- Find Item ---" << std::endl;
    std::cout << "Enter UPC Code: ";
    std::cin >> upcCode;
    
    GroceryItem* item = inventory.search(upcCode);
    
    if (item != nullptr) {
        std::cout << "\nItem Found:" << std::endl;
        std::cout << "UPC Code: " << item->getUpcCode() << std::endl;
        std::cout << "Name: " << item->getName() << std::endl;
        std::cout << "Price: $" << item->getPrice() << std::endl;
        std::cout << "Quantity: " << item->getQuantity() << std::endl;
        std::cout << "Aisle: " << item->getAisle() << std::endl;
    } else {
        std::cout << "Item with UPC " << upcCode << " not found." << std::endl;
    }
}

void updateItem(AVLTree& inventory) {
    std::string upcCode;
    
    std::cout << "\n--- Update Item ---" << std::endl;
    std::cout << "Enter UPC Code of the item to update: ";
    std::cin >> upcCode;
    
    GroceryItem* item = inventory.search(upcCode);
    
    if (item != nullptr) {
        std::string name = item->getName();
        double price = item->getPrice();
        int quantity = item->getQuantity();
        std::string aisle = item->getAisle();
        
        std::cout << "\nCurrent Item Details:" << std::endl;
        std::cout << "UPC Code: " << item->getUpcCode() << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Price: $" << price << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "Aisle: " << aisle << std::endl;
        
        char choice;
        std::cout << "\nUpdate name (y/n)? ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter new name: ";
            std::getline(std::cin, name);
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        std::cout << "Update price (y/n)? ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter new price: $";
            std::cin >> price;
        }
        
        std::cout << "Update quantity (y/n)? ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter new quantity: ";
            std::cin >> quantity;
        }
        
        std::cout << "Update aisle (y/n)? ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter new aisle: ";
            std::getline(std::cin, aisle);
        } else if (choice != 'y' && choice != 'Y') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        GroceryItem updatedItem(name, upcCode, price, quantity, aisle);
        inventory.update(upcCode, updatedItem);
        
        std::cout << "Item updated successfully!" << std::endl;
    } else {
        std::cout << "Item with UPC " << upcCode << " not found." << std::endl;
    }
}

void deleteItem(AVLTree& inventory) {
    std::string upcCode;
    
    std::cout << "\n--- Delete Item ---" << std::endl;
    std::cout << "Enter UPC Code of the item to delete: ";
    std::cin >> upcCode;
    
    if (inventory.remove(upcCode)) {
        std::cout << "Item deleted successfully!" << std::endl;
    } else {
        std::cout << "Item with UPC " << upcCode << " not found." << std::endl;
    }
}

int main() {
    AVLTree inventory;
    int choice;
    

    inventory.insert(GroceryItem("Milk", "100001", 3.99, 50, "Dairy"));
    inventory.insert(GroceryItem("Bread", "100002", 2.49, 30, "Bakery"));
    inventory.insert(GroceryItem("Eggs", "100003", 4.29, 40, "Dairy"));
    inventory.insert(GroceryItem("Apples", "100004", 1.99, 100, "Produce"));
    inventory.insert(GroceryItem("Chicken", "100005", 8.99, 20, "Meat"));
    
    do {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addItem(inventory);
                break;
            case 2:
                findItem(inventory);
                break;
            case 3:
                updateItem(inventory);
                break;
            case 4:
                deleteItem(inventory);
                break;
            case 5:
                inventory.displayAllItems();
                break;
            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
    
    return 0;
}

