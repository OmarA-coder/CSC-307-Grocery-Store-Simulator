#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <string>

// Forward declaration of Item class if not already included
class Item {
private:
    int upc;
    std::string name;
    double price;
    int quantity;
    std::string aisle;

public:
    Item() : upc(0), name(""), price(0.0), quantity(0), aisle("") {}
    
    Item(int upc, std::string name, double price, int quantity, std::string aisle) 
        : upc(upc), name(name), price(price), quantity(quantity), aisle(aisle) {}
    
    // Getters
    int getUPC() const { return upc; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    std::string getAisle() const { return aisle; }
    
    // Setters
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
};

// Transaction class to represent a completed purchase
class Transaction {
private:
    int transactionId;
    int customerId; // -1 if guest checkout
    std::string customerName; // "Guest" if guest checkout
    std::vector<std::pair<Item, int>> items; // Item and quantity
    double subtotal;
    double discount;
    double tax;
    double total;
    std::string timestamp;
    
    // Generate timestamp string
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
public:
    Transaction(int id, int custId, std::string custName, 
                const std::vector<std::pair<Item, int>>& cartItems,
                double subt, double disc, double tx, double tot)
        : transactionId(id), customerId(custId), customerName(custName),
          items(cartItems), subtotal(subt), discount(disc), tax(tx), total(tot) {
        timestamp = getCurrentTimestamp();
    }
    
    // Constructor with timestamp for loading from file
    Transaction(int id, int custId, std::string custName, 
                const std::vector<std::pair<Item, int>>& cartItems,
                double subt, double disc, double tx, double tot, 
                const std::string& ts)
        : transactionId(id), customerId(custId), customerName(custName),
          items(cartItems), subtotal(subt), discount(disc), tax(tx), total(tot),
          timestamp(ts) {
    }
    
    // Getters
    int getTransactionId() const { return transactionId; }
    int getCustomerId() const { return customerId; }
    std::string getCustomerName() const { return customerName; }
    const std::vector<std::pair<Item, int>>& getItems() const { return items; }
    double getSubtotal() const { return subtotal; }
    double getDiscount() const { return discount; }
    double getTax() const { return tax; }
    double getTotal() const { return total; }
    std::string getTimestamp() const { return timestamp; }
    
    // Display transaction details
    void displayDetails() const {
        std::cout << "\n===== Transaction #" << transactionId << " =====\n";
        std::cout << "Date/Time: " << timestamp << std::endl;
        std::cout << "Customer: " << customerName;
        if (customerId != -1) {
            std::cout << " (ID: " << customerId << ")";
        }
        std::cout << std::endl << std::endl;
        
        std::cout << "Items purchased:\n";
        for (const auto& pair : items) {
            std::cout << "- " << pair.first.getName() 
                      << " (UPC: " << pair.first.getUPC() << ")"
                      << " x" << pair.second
                      << " @ $" << std::fixed << std::setprecision(2) << pair.first.getPrice()
                      << " = $" << std::fixed << std::setprecision(2) 
                      << (pair.first.getPrice() * pair.second) << std::endl;
        }
        
        std::cout << "\nSubtotal: $" << std::fixed << std::setprecision(2) << subtotal << std::endl;
        if (discount > 0) {
            std::cout << "Discount: -$" << std::fixed << std::setprecision(2) << discount << std::endl;
        }
        std::cout << "Tax: $" << std::fixed << std::setprecision(2) << tax << std::endl;
        std::cout << "Total: $" << std::fixed << std::setprecision(2) << total << std::endl;
        std::cout << "==============================\n";
    }
    
    // Convert transaction to CSV format for storage
    std::string toCSV() const {
        std::stringstream ss;
        ss << transactionId << ","
           << customerId << ","
           << customerName << ","
           << subtotal << ","
           << discount << ","
           << tax << ","
           << total << ","
           << timestamp;
           
        // Add items data (UPC,Quantity,Price;UPC,Quantity,Price;...)
        ss << ",";
        for (size_t i = 0; i < items.size(); ++i) {
            if (i > 0) ss << ";";
            ss << items[i].first.getUPC() << "," 
               << items[i].second << "," 
               << items[i].first.getPrice();
        }
        
        return ss.str();
    }
};

// Transaction Manager class
class TransactionManager {
private:
    std::vector<Transaction> transactions;
    int nextTransactionId;
    std::string transactionLogFile;
    
    // Save transactions to file
    bool saveToFile() {
        std::ofstream file(transactionLogFile, std::ios::app); // Append mode
        if (!file.is_open()) {
            std::cerr << "Error: Could not open transaction log file for writing.\n";
            return false;
        }
        
        // Only write the most recent transaction (the one just added)
        if (!transactions.empty()) {
            file << transactions.back().toCSV() << std::endl;
        }
        
        file.close();
        return true;
    }
    
    // Load all transactions from file
    bool loadFromFile() {
        std::ifstream file(transactionLogFile);
        if (!file.is_open()) {
            // It's not an error if the file doesn't exist yet
            return true;
        }
        
        transactions.clear();
        nextTransactionId = 1000; // Default starting ID
        
        std::string line;
        while (std::getline(file, line)) {
            // Parse transaction CSV data
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;
            
            // Split by commas
            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }
            
            // Need at least the basic transaction data plus items
            if (tokens.size() >= 9) {
                int transId = std::stoi(tokens[0]);
                int custId = std::stoi(tokens[1]);
                std::string custName = tokens[2];
                double subtotal = std::stod(tokens[3]);
                double discount = std::stod(tokens[4]);
                double tax = std::stod(tokens[5]);
                double total = std::stod(tokens[6]);
                std::string timestamp = tokens[7];
                
                // Parse items (UPC,Quantity,Price;UPC,Quantity,Price;...)
                std::vector<std::pair<Item, int>> items;
                std::istringstream itemsStream(tokens[8]);
                std::string itemData;
                
                while (std::getline(itemsStream, itemData, ';')) {
                    std::istringstream itemStream(itemData);
                    std::string itemToken;
                    std::vector<std::string> itemTokens;
                    
                    while (std::getline(itemStream, itemToken, ',')) {
                        itemTokens.push_back(itemToken);
                    }
                    
                    if (itemTokens.size() >= 3) {
                        int upc = std::stoi(itemTokens[0]);
                        int quantity = std::stoi(itemTokens[1]);
                        double price = std::stod(itemTokens[2]);
                        
                        // Create a simplified item (without name and aisle)
                        Item item(upc, "Item #" + std::to_string(upc), price, 0, "");
                        items.push_back(std::make_pair(item, quantity));
                    }
                }
                
                // Create and add the transaction
                Transaction transaction(transId, custId, custName, items, 
                                       subtotal, discount, tax, total, timestamp);
                transactions.push_back(transaction);
                
                // Update next transaction ID if needed
                if (transId >= nextTransactionId) {
                    nextTransactionId = transId + 1;
                }
            }
        }
        
        file.close();
        return true;
    }
    
public:
    TransactionManager(const std::string& logFile = "transactions.csv") 
        : nextTransactionId(1000), transactionLogFile(logFile) {
        loadFromFile();
    }
    
    // Add a new transaction
    int addTransaction(int customerId, const std::string& customerName,
                      const std::vector<std::pair<Item, int>>& items,
                      double subtotal, double discount, double tax, double total) {
        int transactionId = nextTransactionId++;
        
        Transaction transaction(transactionId, customerId, customerName,
                               items, subtotal, discount, tax, total);
        transactions.push_back(transaction);
        
        // Save the transaction to file
        saveToFile();
        
        return transactionId;
    }
    
    // Find a transaction by ID
    const Transaction* findTransaction(int transactionId) const {
        for (const auto& transaction : transactions) {
            if (transaction.getTransactionId() == transactionId) {
                return &transaction;
            }
        }
        return nullptr;
    }
    
    // Get transactions by customer ID
    std::vector<const Transaction*> getTransactionsByCustomer(int customerId) const {
        std::vector<const Transaction*> result;
        for (const auto& transaction : transactions) {
            if (transaction.getCustomerId() == customerId) {
                result.push_back(&transaction);
            }
        }
        return result;
    }
    
    // Get all transactions
    const std::vector<Transaction>& getAllTransactions() const {
        return transactions;
    }
    
    // Get the total revenue
    double getTotalRevenue() const {
        double total = 0.0;
        for (const auto& transaction : transactions) {
            total += transaction.getTotal();
        }
        return total;
    }
    
    // Get the total revenue for a specific day
    double getDailyRevenue(const std::string& date) const {
        double total = 0.0;
        for (const auto& transaction : transactions) {
            // Check if the transaction date matches (format: YYYY-MM-DD HH:MM:SS)
            if (transaction.getTimestamp().substr(0, 10) == date) {
                total += transaction.getTotal();
            }
        }
        return total;
    }
    
    // Get the number of transactions
    size_t getTransactionCount() const {
        return transactions.size();
    }
    
    // Get the average transaction value
    double getAverageTransactionValue() const {
        if (transactions.empty()) {
            return 0.0;
        }
        return getTotalRevenue() / transactions.size();
    }
    
    // Display all transactions
    void displayAllTransactions() const {
        if (transactions.empty()) {
            std::cout << "No transactions found.\n";
            return;
        }
        
        for (const auto& transaction : transactions) {
            transaction.displayDetails();
        }
    }
    
    // Generate a sales report
    void generateSalesReport() const {
        if (transactions.empty()) {
            std::cout << "No transaction data available for report.\n";
            return;
        }
        
        std::cout << "\n===== Sales Report =====\n";
        std::cout << "Total Transactions: " << getTransactionCount() << std::endl;
        std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) << getTotalRevenue() << std::endl;
        std::cout << "Average Transaction Value: $" << std::fixed << std::setprecision(2) << getAverageTransactionValue() << std::endl;
        
        // Get today's date in YYYY-MM-DD format
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
        std::string today = ss.str();
        
        std::cout << "Today's Revenue: $" << std::fixed << std::setprecision(2) << getDailyRevenue(today) << std::endl;
        
        // Product popularity report
        std::map<int, std::pair<std::string, int>> productSales; // UPC -> (Name, Quantity)
        for (const auto& transaction : transactions) {
            for (const auto& item : transaction.getItems()) {
                if (productSales.find(item.first.getUPC()) == productSales.end()) {
                    productSales[item.first.getUPC()] = std::make_pair(item.first.getName(), 0);
                }
                productSales[item.first.getUPC()].second += item.second;
            }
        }
        
        std::cout << "\nTop Selling Products:\n";
        
        // Convert map to vector for sorting
        std::vector<std::pair<int, std::pair<std::string, int>>> productVector(
            productSales.begin(), productSales.end());
        
        // Sort by quantity sold (descending)
        std::sort(productVector.begin(), productVector.end(),
            [](const auto& a, const auto& b) {
                return a.second.second > b.second.second;
            });
        
        // Display top 5 products (or fewer if less than 5 exist)
        int count = 0;
        for (const auto& product : productVector) {
            std::cout << product.second.first << " (UPC: " << product.first << "): "
                      << product.second.second << " units sold\n";
            
            if (++count >= 5) break;
        }
        
        std::cout << "========================\n";
    }
    
    // Function to demonstrate how to use transaction processing in the main program
    static void demonstrateTransactionProcessing() {
        std::cout << "\n===== Transaction Processing Demo =====\n";
        std::cout << "This function demonstrates how to integrate transaction processing\n";
        std::cout << "into your grocery store simulator. Follow these steps:\n\n";
        
        std::cout << "1. In main.cpp, create a TransactionManager instance:\n";
        std::cout << "   TransactionManager transactionManager;\n\n";
        
        std::cout << "2. Add a new option to your main menu:\n";
        std::cout << "   case 7: // Transaction Reports\n";
        std::cout << "     // Show transaction report menu\n";
        std::cout << "     break;\n\n";
        
        std::cout << "3. In your checkout process, record the transaction:\n";
        std::cout << "   int transactionId = transactionManager.addTransaction(\n";
        std::cout << "     customerId, customerName, cart.getItems(),\n";
        std::cout << "     subtotal, discount, tax, total);\n\n";
        
        std::cout << "4. Create a Transaction Report menu:\n";
        std::cout << "   - View all transactions\n";
        std::cout << "   - Generate sales report\n";
        std::cout << "   - Find transaction by ID\n";
        std::cout << "   - View customer transactions\n\n";
        
        std::cout << "For full implementation details, see the comments in the code.\n";
        std::cout << "======================================\n";
    }
};

// Demo function - you can call this from main to test the transaction system
void transactionProcessingDemo() {
    // Create a transaction manager
    TransactionManager manager;
    
    // Show the integration instructions
    TransactionManager::demonstrateTransactionProcessing();
    
    // Create some sample items
    Item milk(1, "Milk", 3.99, 50, "Dairy");
    Item bread(2, "Bread", 2.49, 30, "Bakery");
    Item eggs(3, "Eggs", 4.99, 40, "Dairy");
    
    // Create a sample cart
    std::vector<std::pair<Item, int>> cart;
    cart.push_back(std::make_pair(milk, 2));   // 2 milks
    cart.push_back(std::make_pair(bread, 1));  // 1 bread
    cart.push_back(std::make_pair(eggs, 1));   // 1 egg carton
    
    // Calculate subtotal
    double subtotal = 0.0;
    for (const auto& pair : cart) {
        subtotal += pair.first.getPrice() * pair.second;
    }
    
    // Calculate tax and total
    double tax = subtotal * 0.0825; // 8.25% tax
    double total = subtotal + tax;
    
    // Add the transaction
    int transactionId = manager.addTransaction(
        -1, // Guest checkout (no customer ID)
        "Guest Customer",
        cart,
        subtotal,
        0.0, // No discount
        tax,
        total
    );
    
    std::cout << "Created transaction #" << transactionId << "\n";
    
    // Display the transaction
    const Transaction* transaction = manager.findTransaction(transactionId);
    if (transaction != nullptr) {
        transaction->displayDetails();
    }
    
    // Generate a sales report
    manager.generateSalesReport();
}

// You can include the following main function for testing just this file,
// or comment it out when integrating with your project
/*
int main() {
    std::cout << "Transaction Processing System - Test\n";
    transactionProcessingDemo();
    return 0;
}
*/
