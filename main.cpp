#include <iostream>
#include <limits>
#include "AVLTree.h"
#include "ShoppingCart.h"
#include "TransactionProcessing.h" 

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
    cout << "6. Shopping Cart" << endl;
    cout << "7. Transaction Reports" << endl;
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

void processCheckout(AVLTree& inventory, ShoppingCart& cart, TransactionManager& transactionManager) {
    if (cart.isEmpty()) {
        cout << "Your cart is empty. Nothing to checkout." << endl;
        return;
    }

    // Display cart contents
    cout << "\n--- Checkout ---" << endl;
    cart.displayCart();
    
    // Calculate totals
    double subtotal = cart.getTotal();
    double tax = subtotal * 0.0825; // 8.25% tax rate
    double total = subtotal + tax;
    
    // Display receipt
    cout << "\n--- Receipt ---" << endl;
    cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << endl;
    cout << "Tax (8.25%): $" << std::fixed << std::setprecision(2) << tax << endl;
    cout << "Total: $" << std::fixed << std::setprecision(2) << total << endl;

    // Record the transaction
    int transactionId = transactionManager.addTransaction(
        -1, // Guest customer ID
        "Guest", // Customer name
        cart.getItems(),
        subtotal,
        0.0, // Discount amount
        tax,
        total
    );

    cout << "Transaction #" << transactionId << " has been recorded." << endl;
    
    // Clear the cart
    cart.clearCart();
    cout << "Thank you for your purchase!" << endl;
}

int main() {
    AVLTree inventory;
    int choice;
    
    int cart_choice;
    ShoppingCart cart;
    TransactionManager transactionManager;

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
                    cout << "5. Checkout" << endl;
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
                    case 5:
                        processCheckout(inventory, cart, transactionManager);
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
            case 7: {
                // Transaction Reports
                std::cout << "\n===== Transaction Reports =====\n";
                std::cout << "1. View all transactions\n";
                std::cout << "2. Generate sales report\n";
                std::cout << "3. Find transaction by ID\n";
                std::cout << "4. View customer transactions\n";
                std::cout << "0. Return to main menu\n";
                std::cout << "Choice: ";
                
                int reportChoice;
                std::cin >> reportChoice;
                
                switch (reportChoice) {
                    case 1:
                        // View all transactions
                        transactionManager.displayAllTransactions();
                        break;
                    case 2:
                        // Generate sales report
                        transactionManager.generateSalesReport();
                        break;
                    case 3: {
                        // Find transaction by ID
                        int transactionId;
                        std::cout << "Enter transaction ID: ";
                        std::cin >> transactionId;
                        
                        const Transaction* transaction = transactionManager.findTransaction(transactionId);
                        if (transaction != nullptr) {
                            transaction->displayDetails();
                        } else {
                            std::cout << "Transaction not found.\n";
                        }
                        break;
                    }
                    case 4: {
                        // View customer transactions
                        int customerId;
                        std::cout << "Enter customer ID: ";
                        std::cin >> customerId;
                        
                        std::vector<const Transaction*> customerTransactions = 
                            transactionManager.getTransactionsByCustomer(customerId);
                        
                        if (customerTransactions.empty()) {
                            std::cout << "No transactions found for this customer.\n";
                        } else {
                            std::cout << "Found " << customerTransactions.size() 
                                    << " transactions for customer ID " << customerId << ":\n";
                            
                            for (const auto* transaction : customerTransactions) {
                                transaction->displayDetails();
                            }
                        }
                        break;
                    }
                    case 0:
                        // Return to main menu
                        break;
                    default:
                        std::cout << "Invalid choice.\n";
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
