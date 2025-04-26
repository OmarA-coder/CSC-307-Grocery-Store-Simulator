// TransactionProcessing.cpp - Implementation of transaction processing system
// Part of CSC 307 Grocery Store Simulator project

#include "TransactionProcessing.h"
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <algorithm>

// Helper function to get current timestamp
std::string Transaction::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Constructor for new transactions
Transaction::Transaction(int id, int custId, std::string custName, 
            const std::vector<std::pair<GroceryItem, int>>& cartItems,
            double subt, double disc, double tx, double tot)
    : transactionId(id), customerId(custId), customerName(custName),
      items(cartItems), subtotal(subt), discount(disc), tax(tx), total(tot) {
    timestamp = getCurrentTimestamp();
}

// Constructor for loading transactions from file
Transaction::Transaction(int id, int custId, std::string custName, 
            const std::vector<std::pair<GroceryItem, int>>& cartItems,
            double subt, double disc, double tx, double tot, 
            const std::string& ts)
    : transactionId(id), customerId(custId), customerName(custName),
      items(cartItems), subtotal(subt), discount(disc), tax(tx), total(tot),
      timestamp(ts) {
}

// Getters
int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getCustomerId() const { return customerId; }
std::string Transaction::getCustomerName() const { return customerName; }
const std::vector<std::pair<GroceryItem, int>>& Transaction::getItems() const { return items; }
double Transaction::getSubtotal() const { return subtotal; }
double Transaction::getDiscount() const { return discount; }
double Transaction::getTax() const { return tax; }
double Transaction::getTotal() const { return total; }
std::string Transaction::getTimestamp() const { return timestamp; }

// Display transaction details
void Transaction::displayDetails() const {
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
                  << " (UPC: " << pair.first.getUpcCode() << ")"
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
std::string Transaction::toCSV() const {
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
        ss << items[i].first.getUpcCode() << "," 
           << items[i].second << "," 
           << items[i].first.getPrice();
    }
    
    return ss.str();
}

// TransactionManager constructor
TransactionManager::TransactionManager(const std::string& logFile) 
    : nextTransactionId(1000), transactionLogFile(logFile) {
    loadFromFile();
}

// Save transactions to file
bool TransactionManager::saveToFile() {
    std::ofstream file(transactionLogFile, std::ios::app); // Append mode
    if (!file.is_open()) {
        std::cerr << "Error: Could not open transaction log file for writing.\n";
        return false;
    }
    
    // Only write the most recent transaction
    if (!transactions.empty()) {
        file << transactions.back().toCSV() << std::endl;
    }
    
    file.close();
    return true;
}

// Load all transactions from file
bool TransactionManager::loadFromFile() {
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
            std::vector<std::pair<GroceryItem, int>> items;
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
                    GroceryItem item(upc, "Item #" + std::to_string(upc), price, 0, "");
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

// Add a new transaction
int TransactionManager::addTransaction(int customerId, const std::string& customerName,
                  const std::vector<std::pair<GroceryItem, int>>& items,
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
const Transaction* TransactionManager::findTransaction(int transactionId) const {
    for (const auto& transaction : transactions) {
        if (transaction.getTransactionId() == transactionId) {
            return &transaction;
        }
    }
    return nullptr;
}

// Get transactions by customer ID
std::vector<const Transaction*> TransactionManager::getTransactionsByCustomer(int customerId) const {
    std::vector<const Transaction*> result;
    for (const auto& transaction : transactions) {
        if (transaction.getCustomerId() == customerId) {
            result.push_back(&transaction);
        }
    }
    return result;
}

// Get all transactions
const std::vector<Transaction>& TransactionManager::getAllTransactions() const {
    return transactions;
}

// Get the total revenue
double TransactionManager::getTotalRevenue() const {
    double total = 0.0;
    for (const auto& transaction : transactions) {
        total += transaction.getTotal();
    }
    return total;
}

// Get the total revenue for a specific day
double TransactionManager::getDailyRevenue(const std::string& date) const {
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
size_t TransactionManager::getTransactionCount() const {
    return transactions.size();
}

// Get the average transaction value
double TransactionManager::getAverageTransactionValue() const {
    if (transactions.empty()) {
        return 0.0;
    }
    return getTotalRevenue() / transactions.size();
}

// Display all transactions
void TransactionManager::displayAllTransactions() const {
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
        return;
    }
    
    for (const auto& transaction : transactions) {
        transaction.displayDetails();
    }
}

// Generate a sales report
void TransactionManager::generateSalesReport() const {
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
            if (productSales.find(item.first.getUpcCode()) == productSales.end()) {
                productSales[item.first.getUpcCode()] = std::make_pair(item.first.getName(), 0);
            }
            productSales[item.first.getUpcCode()].second += item.second;
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
