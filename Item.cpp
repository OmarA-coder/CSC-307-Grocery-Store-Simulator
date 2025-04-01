#include <iostream>
#include <vector>
#include <string>
#include "Item.h"
#include <iomanip>  //for setprecsion
#include <fstream>  //for file operations
#include <sstream>  //for stringstream

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
    os << left << setw(20) << item.name
       << setw(15) << item.upcCode
       << setw(10) << fixed << setprecision(2) << item.price
       << setw(10) << item.aisle
       << right << setw(10) << item.quantity;
    return os;
}

// ShopingCart class implementation
void ShopingCart::addItem(const Item& item) {
    cart.push_back(item);
    total += item.getPrice() * item.getQuantity();
    quantity += item.getQuantity();
}

void ShopingCart::removeItem(const string& upcCode) {
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if (it->getUpcCode() == upcCode) {
            total -= it->getPrice() * it->getQuantity();
            quantity -= it->getQuantity();
            cart.erase(it);
            break;
        }
    }
}

double ShopingCart::calculateTotal() {
    return total;
}

void ShopingCart::displayCart() {
    // Set up table headers with consistent width
    cout << "\n=================== Shopping Cart ===================\n";
    cout << left << setw(20) << "Item Name" 
         << setw(15) << "UPC Code" 
         << setw(10) << "Price($)" 
         << setw(10) << "Aisle" 
         << right << setw(10) << "Quantity" 
         << right << setw(15) << "Subtotal($)" << endl;
    cout << string(70, '-') << endl;

    // Display each item as a row
    for (const auto& item : cart) {
        double subtotal = item.getPrice() * item.getQuantity();
        cout << left << setw(20) << item.getName()
             << setw(15) << item.getUpcCode()
             << setw(10) << fixed << setprecision(2) << item.getPrice()
             << setw(10) << item.getAisle()
             << right << setw(10) << item.getQuantity()
             << right << setw(15) << fixed << setprecision(2) << subtotal << endl;
    }

    // Display the total with proper alignment
    cout << string(70, '-') << endl;
    cout << right << setw(55) << "Total: $" 
         << fixed << setprecision(2) << setw(15) << total << endl;
    cout << "===================================================\n";
}

void ShopingCart::clearCart() {
    cart.clear();
    total = 0.0;
    quantity = 0;
}

//Transaction class implementation
void Transaction::addItem(const Item& item) {
    cart.push_back(item);
    total += item.getPrice() * item.getQuantity();
}
void Transaction::removeItem(const string& upcCode) {
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if (it->getUpcCode() == upcCode) {
            total -= it->getPrice() * it->getQuantity();
            cart.erase(it);
            break;
        }
    }
}


void Transaction::finalizeCart(const ShopingCart& cart) {
    // Clear any existing items in the transaction
    this->cart.clear();
    total = 0.0;
    
    // Copy all items from the shopping cart to the transaction
    for (const auto& item : cart.getCart()) {
        this->cart.push_back(item);
        total += item.getPrice() * item.getQuantity();
    }
}
double Transaction::calculateTotal() {
    double sum = 0.0;  // Initialize accumulator correctly
    for (const auto& item : cart) {
        sum += item.getPrice() * item.getQuantity();
    }
    return sum;
}

void Transaction::printReceipt() {
    cout << "\n================ GROCERY STORE RECEIPT ================\n";
    
    //Printing the receipt header
    cout << "Date: " << (date.empty() ? "N/A" : date) << endl;
    cout << "Time: " << (time.empty() ? "N/A" : time) << endl;
    cout << "Cashier: " << (cashier.empty() ? "N/A" : cashier) << endl;

    cout << "Receipt #: " << rand() % 10000 + 10000 << endl;
    cout << "\n";
    cout << left << setw(20) << "Item" 
         << setw(8) << "Qty" 
         << right << setw(10) << "Price" 
         << right << setw(12) << "Subtotal" << endl;
    cout << string(50, '-') << endl;

    // Displaying each item
    for (const auto& item : cart) {
        double subtotal = item.getPrice() * item.getQuantity();
        cout << left << setw(20) << item.getName()
             << setw(8) << item.getQuantity()
             << right << setw(10) << fixed << setprecision(2) << "$" << item.getPrice()
             << right << setw(12) << fixed << setprecision(2) << "$" << subtotal << endl;
    }

    // Displaying the total
    cout << string(50, '-') << endl;
    cout << left << setw(28) << "Total Items:" << cart.size() << endl;
    cout << left << setw(28) << "Total:" << right << "$" << fixed << setprecision(2) << total << endl;
    cout << "\n";
    

    cout << "Payment Method: Cash" << endl;
    cout << "Amount Tendered: $" << fixed << setprecision(2) << (total + 5.0) << endl;
    cout << "Change: $" << fixed << setprecision(2) << 5.0 << endl;
    
    cout << "\nThank you for shopping with us!" << endl;
    cout << "Please come again soon." << endl;
    cout << "===================================================\n";
}
const vector<Item>& ShopingCart::getCart() const {
    return cart;
}
void Transaction::setTime(const string& time) {
    this->time = time;
}

void Transaction::setDate(const string& date) {
    this->date = date;
}

void Transaction::setCashier(const string& cashier) {
    this->cashier = cashier;
}
string Transaction::getTime() const {
    return time;
}

string Transaction::getDate() const {
    return date;
}

string Transaction::getCashier() const {
    return cashier;
}

void Transaction::clearTransaction() {
    cart.clear();
    total = 0.0;
}
void Transaction::saveTransaction() {
    //Save to a text file
    ofstream outFile("transaction.txt");


}
void Transaction::loadTransaction() {
    //loading from a text_file
    ifstream inFile("transaction.txt");
    if (!inFile) {
        cout << "Error opening file." << endl;
        return;
    }
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string name, upcCode, aisle;
        double price;
        int quantity;
        ss >> name >> upcCode >> price >> aisle >> quantity;
        Item item(name, upcCode, price, aisle, quantity);
        cart.push_back(item); // Add item to the cart
 
        cout << line << endl;
    }
}

int main() {
    // Example usage
    Item item("Apple", "12345", 0.99, "Fruits", 100);
    cout << item << endl;

    item.addQuantity(50);
    cout << "\nAfter adding quantity: " << item.getQuantity() << endl;

    item.removeQuantity(30);
    cout << "After removing quantity: " << item.getQuantity() << endl;

    item.setPrice(1.29);
    cout << "Updated price: $" << item.getPrice() << endl;
    item.setAisle("Produce");
    cout << "Updated aisle: " << item.getAisle() << endl;
    cout << item << endl;

    // Test comparison operators
    Item item2("Banana", "67890", 0.59, "Fruits", 200);
    cout << "Item 1 < Item 2: " << (item < item2) << endl;
    cout << "Item 1 > Item 2: " << (item > item2) << endl;
    cout << "Item 1 == Item 2: " << (item == item2) << endl;
    cout << "Item 1 != Item 2: " << (item != item2) << endl;


    //Test shopping cart
    ShopingCart cart;
    cart.addItem(item);
    cart.addItem(item2);
    cout << "Total cost in cart: $" << cart.calculateTotal() << endl;
    cart.displayCart();

    cart.removeItem("12345");
    cout << "\nAfter removing item from cart:\n";
    cart.displayCart();
    cart.clearCart();

    cout << "Cart after clearing cart: " << endl;
    cout << "Cart cleared.\n";

    //Test transaction
    Transaction transaction;
    transaction.setTime("12:00 PM");
    transaction.setDate("2023-10-01");
    transaction.setCashier("John Doe");
    cout << "Transaction Date: " << transaction.getDate() << endl;
    cout << "Transaction Time: " << transaction.getTime() << endl;
    cout << "Cashier: " << transaction.getCashier() << endl;
    cout << "Total cost in transaction: $" << transaction.calculateTotal() << endl;
    transaction.printReceipt();
    transaction.saveTransaction();
    transaction.loadTransaction();
    cout << "Transaction loaded from file." << endl;
    transaction.clearTransaction();
    cout << "Transaction cleared." << endl;
    return 0;

    //Menu-driven loop, so data can be changed during runtime
    int choice;
    do {
        cout << "\n===================================\n";
        cout << "\nGrocery store Simulator - Menu";
        cout << "\n1. Add item to Cart";
        cout << "\n2. Remove item from Cart (UPC Code)";
        cout << "\n3. Display Shoppng Cart";
        cout << "\n4. Clear Shopping Cart";
        cout << "\n5. Exit";
        cout << "\n===================================";
        cout << "\nEnter your choice: ";
        cin >> choice;
    
    } while (choice != 5);
}
