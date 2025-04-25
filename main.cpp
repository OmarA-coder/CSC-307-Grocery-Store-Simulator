#include <iostream>
#include <limits>
#include "AVLTree.h"
#include "ShoppingCart.h"

//Namespace directives to clean up code
using std::cout;
using std::cin;
using std::string;
using std::endl;

void displayMenu() {
    std::cout << "\n===== GROCERY STORE INVENTORY SYSTEM =====" << endl;
    cout << "1. Add New Item" << endl;
    cout << "2. Find Item by UPC" << endl;
    cout << "3. Update Item" << endl;
    cout << "4. Delete Item" << endl;
    cout << "5. Display All Items" << endl;
    cout <<"6. Shopping Cart" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

void addItem(AVLTree& inventory) {
    string name, upcCode, aisle;
    double price;
    int quantity;
    
    std::cout << "\n--- Add New Item ---" << endl;
    
    cout << "Enter UPC Code: ";
    cin >> upcCode;
    
    // Check if UPC already exists
    if (inventory.search(upcCode) != nullptr) {
        cout << "Error: Item with UPC " << upcCode << " already exists." << endl;
        return;
    }
    
   cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
   cout << "Enter Name: ";
   std::getline(std::cin, name);
    
    cout << "Enter Price: $";
    cin >> price;
    
    cout << "Enter Quantity: ";
    cin >> quantity;
    
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    cout << "Enter Aisle: ";
    std::getline(std::cin, aisle);
    
    GroceryItem newItem(name, upcCode, price, quantity, aisle);
    inventory.insert(newItem);
    
    cout << "Item added successfully!" << std::endl;
}

void findItem(AVLTree& inventory) {
    string upcCode;
    
    cout << "\n--- Find Item ---" << std::endl;
    cout << "Enter UPC Code: ";
    cin >> upcCode;
    
    GroceryItem* item = inventory.search(upcCode);
    
    if (item != nullptr) {
        cout << "\nItem Found:" << endl;
        cout << "UPC Code: " << item->getUpcCode() << endl;
        cout << "Name: " << item->getName() << endl;
        cout << "Price: $" << item->getPrice() << endl;
        cout << "Quantity: " << item->getQuantity() << endl;
        cout << "Aisle: " << item->getAisle() << endl;
    } else {
        cout << "Item with UPC " << upcCode << " not found." << endl;
    }
}

void updateItem(AVLTree& inventory) {
   string upcCode;
    
    cout << "\n--- Update Item ---" << endl;
    cout << "Enter UPC Code of the item to update: ";
    cin >> upcCode;
    
    GroceryItem* item = inventory.search(upcCode);
    
    if (item != nullptr) {
        string name = item->getName();
        double price = item->getPrice();
        int quantity = item->getQuantity();
        string aisle = item->getAisle();
        
        cout << "\nCurrent Item Details:" << endl;
        cout << "UPC Code: " << item->getUpcCode() << endl;
        cout << "Name: " << name << endl;
        cout << "Price: $" << price << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Aisle: " << aisle << endl;
        
        char choice;
        cout << "\nUpdate name (y/n)? ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter new name: ";
            std::getline(std::cin, name);
        } else {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        cout << "Update price (y/n)? ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter new price: $";
            cin >> price;
        }
        
        cout << "Update quantity (y/n)? ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter new quantity: ";
            cin >> quantity;
        }
        
        cout << "Update aisle (y/n)? ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter new aisle: ";
            std::getline(std::cin, aisle);
        } else if (choice != 'y' && choice != 'Y') {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        GroceryItem updatedItem(name, upcCode, price, quantity, aisle);
        inventory.update(upcCode, updatedItem);
        
        cout << "Item updated successfully!" << endl;
    } else {
        cout << "Item with UPC " << upcCode << " not found." << endl;
    }
}

void deleteItem(AVLTree& inventory) {
    string upcCode;
    
    cout << "\n--- Delete Item ---" << endl;
    cout << "Enter UPC Code of the item to delete: ";
    cin >> upcCode;
    
    if (inventory.remove(upcCode)) {
        cout << "Item deleted successfully!" << endl;
    } else {
        cout << "Item with UPC " << upcCode << " not found." << endl;
    }
}



int main() {
    AVLTree inventory;
    int choice;
    
    int cart_choice;
    ShoppingCart cart;

    inventory.insert(GroceryItem("Milk", "100001", 3.99, 50, "Dairy"));
    inventory.insert(GroceryItem("Bread", "100002", 2.49, 30, "Bakery"));
    inventory.insert(GroceryItem("Eggs", "100003", 4.29, 40, "Dairy"));
    inventory.insert(GroceryItem("Apples", "100004", 1.99, 100, "Produce"));
    inventory.insert(GroceryItem("Chicken", "100005", 8.99, 20, "Meat"));
    
    do {
        displayMenu();
        cin >> choice;
        switch(choice) {
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
            case 6: {
                bool stayInCartMenu = true;
                while (stayInCartMenu) {
                    cout << "\n--- Shopping Cart Menu ---" << endl;
                    cout << "1. Add Item to Cart" << endl;
                    cout << "2. Remove Item from Cart" << endl;
                    cout << "3. Display Cart" << endl;
                    cout << "4. Clear Cart" << endl;
                    cout << "0. Exit Cart" << endl;
                    cout << "Enter your choice: ";
                    cin >> cart_choice;
                    if (cart_choice == 0) {
                        cout << "Exiting cart menu." << endl;
                        break;
                    }
                    string upccode;
                    int quantity;
                    switch (cart_choice)
                    {
                    case 1:
                        cout << "Enter UPC Code: ";
                        cin >> upccode;
                        cout << "Enter Quantity you want to add: ";
                        cin >> quantity;
                        cart.addItem(inventory, upccode, quantity);
                        cout << "Item added to cart." << endl;
                        break;
                    case 2:
                        cout << "Enter UPC Code: ";
                        cin >> upccode;
                        cout << "Enter Quantity you want to remove: ";
                        cin >> quantity;
                        cart.removeItem(inventory, upccode, quantity);
                        cout << "Item removed from cart." << endl;
                        break;
                    case 3:
                        cout << "\n--- Shopping Cart ---" << endl;
                        cart.displayCart();
                        break;
                    case 4:
                        cart.clearCart();
                        cout << "Cart cleared." << endl;
                        break;
                    case 0:
                        cout << "Exiting cart menu." << endl;
                        stayInCartMenu = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
                break;
            }
            case 0:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
    
    return 0;
}

