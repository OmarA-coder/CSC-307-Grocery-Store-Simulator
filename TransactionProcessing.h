// TransactionProcessing.h - Header file for transaction processing system
// Part of CSC 307 Grocery Store Simulator project

#ifndef TRANSACTION_PROCESSING_H
#define TRANSACTION_PROCESSING_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "GroceryItem.h" 

// Transaction class to track customer purchases
class Transaction {
private:
    int transactionId;
    int customerId; // -1 for guest checkout
    std::string customerName;
    std::vector<std::pair<GroceryItem, int>> items; // Pairs of item and quantity
    double subtotal;
    double discount;
    double tax;
    double total;
    std::string timestamp;
    
    std::string getCurrentTimestamp();
    
public:
    // Constructor for new transactions
    Transaction(int id, int custId, std::string custName, 
                const std::vector<std::pair<GroceryItem, int>>& cartItems,
                double subt, double disc, double tx, double tot);
    
    // Constructor for loading transactions from file
    Transaction(int id, int custId, std::string custName, 
                const std::vector<std::pair<GroceryItem, int>>& cartItems,
                double subt, double disc, double tx, double tot, 
                const std::string& ts);
    
    // Getters
    int getTransactionId() const;
    int getCustomerId() const;
    std::string getCustomerName() const;
    const std::vector<std::pair<GroceryItem, int>>& getItems() const;
    double getSubtotal() const;
    double getDiscount() const;
    double getTax() const;
    double getTotal() const;
    std::string getTimestamp() const;
    
    // Display and convert functions
    void displayDetails() const;
    std::string toCSV() const;
};

// TransactionManager to handle all transaction operations
class TransactionManager {
private:
    std::vector<Transaction> transactions;
    int nextTransactionId;
    std::string transactionLogFile;
    
    // File I/O operations
    bool saveToFile();
    bool loadFromFile();
    
public:
    // Constructor
    TransactionManager(const std::string& logFile = "transactions.csv");
    
    // Add a new transaction
    int addTransaction(int customerId, const std::string& customerName,
                      const std::vector<std::pair<GroceryItem, int>>& items,
                      double subtotal, double discount, double tax, double total);
    
    // Find transactions
    const Transaction* findTransaction(int transactionId) const;
    std::vector<const Transaction*> getTransactionsByCustomer(int customerId) const;
    const std::vector<Transaction>& getAllTransactions() const;
    
    // Analytics functions
    double getTotalRevenue() const;
    double getDailyRevenue(const std::string& date) const;
    size_t getTransactionCount() const;
    double getAverageTransactionValue() const;
    
    // Reporting functions
    void displayAllTransactions() const;
    void generateSalesReport() const;
};

#endif // TRANSACTION_PROCESSING_H
