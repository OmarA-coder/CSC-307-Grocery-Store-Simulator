#ifndef AVLTREE_H
#define AVLTREE_H


#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include "GroceryItem.h"

class AVLTree {
private:
    struct Node {
        GroceryItem item;
        Node* left;
        Node* right;
        int height;
        
        Node(const GroceryItem& i) : item(i), left(nullptr), right(nullptr), height(1) {}
    };
    
    Node* root;
    
    // Helper functions
    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }
    
    int balanceFactor(Node* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }
    
    void updateHeight(Node* node) {
        if (node == nullptr) return;
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
    
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        // Perform rotation
        x->right = y;
        y->left = T2;
        
        // Update heights
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        // Perform rotation
        y->left = x;
        x->right = T2;
        
        // Update heights
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    Node* insertNode(Node* node, const GroceryItem& item) {
        // Standard BST insertion
        if (node == nullptr)
            return new Node(item);
            
        if (item.getUpcCode() < node->item.getUpcCode())
            node->left = insertNode(node->left, item);
        else if (item.getUpcCode() > node->item.getUpcCode())
            node->right = insertNode(node->right, item);
        else
            return node; // Duplicate UPC codes not allowed
            
        // Update height of current node
        updateHeight(node);
        
        // Get balance factor
        int balance = balanceFactor(node);
        
        // Left Left Case
        if (balance > 1 && item.getUpcCode() < node->left->item.getUpcCode())
            return rightRotate(node);
            
        // Right Right Case
        if (balance < -1 && item.getUpcCode() > node->right->item.getUpcCode())
            return leftRotate(node);
            
        // Left Right Case
        if (balance > 1 && item.getUpcCode() > node->left->item.getUpcCode()) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && item.getUpcCode() < node->right->item.getUpcCode()) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    Node* findMinValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }
    
    Node* deleteNode(Node* root, const std::string& upcCode) {
        // Standard BST delete
        if (root == nullptr)
            return root;
            
        if (upcCode < root->item.getUpcCode())
            root->left = deleteNode(root->left, upcCode);
        else if (upcCode > root->item.getUpcCode())
            root->right = deleteNode(root->right, upcCode);
        else {
            // Node with only one child or no child
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            
            // Node with two children
            Node* temp = findMinValueNode(root->right);
            root->item = temp->item;
            root->right = deleteNode(root->right, temp->item.getUpcCode());
        }
        
        // If the tree had only one node
        if (root == nullptr)
            return root;
            
        // Update height
        updateHeight(root);
        
        // Get balance factor
        int balance = balanceFactor(root);
        
        // Left Left Case
        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);
            
        // Left Right Case
        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        
        // Right Right Case
        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);
            
        // Right Left Case
        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        
        return root;
    }
    
    Node* searchNode(Node* root, const std::string& upcCode) const {
        if (root == nullptr || root->item.getUpcCode() == upcCode)
            return root;
            
        if (upcCode < root->item.getUpcCode())
            return searchNode(root->left, upcCode);
            
        return searchNode(root->right, upcCode);
    }

    Node* updateNode(Node* root, const std::string& upcCode, const GroceryItem& newItem) {
        if (root == nullptr) return nullptr;
        
        if (upcCode < root->item.getUpcCode()) {
            root->left = updateNode(root->left, upcCode, newItem);
        } else if (upcCode > root->item.getUpcCode()) {
            root->right = updateNode(root->right, upcCode, newItem);
        } else {
            root->item = newItem;
        }
        
        return root;
    }

    void displayAllItemsHelper(Node* node) const {
        if (node == nullptr) return;
        
        displayAllItemsHelper(node->left);
        std::cout << "UPC: " << node->item.getUpcCode() 
                  << ", Name: " << node->item.getName() 
                  << ", Price: $" << node->item.getPrice() 
                  << ", Quantity: " << node->item.getQuantity()
                  << ", Aisle: " << node->item.getAisle() << std::endl;
        displayAllItemsHelper(node->right);
    }
    
    void inOrderTraversal(Node* node) const {
        if (node == nullptr) return;
        
        inOrderTraversal(node->left);
        std::cout << "UPC: " << node->item.getUpcCode() 
                  << ", Name: " << node->item.getName() 
                  << ", Price: $" << node->item.getPrice() 
                  << ", Quantity: " << node->item.getQuantity()
                  << ", Aisle: " << node->item.getAisle() << std::endl;
        inOrderTraversal(node->right);
    }
    
    void clearTree(Node* node) {
        if (node == nullptr) return;
        
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    
    ~AVLTree() {
        clearTree(root);
    }
    
    // Create operation - Insert a grocery item
    void insert(const GroceryItem& item) {
        root = insertNode(root, item);
    }
    
    // Read operation - Search for an item by UPC code
    GroceryItem* search(const std::string& upcCode) {
        Node* result = searchNode(root, upcCode);
        if (result == nullptr) {
            return nullptr;
        }
        return &(result->item);
    }
    
    // Update operation - Update an existing item
    bool update( const std::string& upcCode, const GroceryItem& newItem) {
        if(search(upcCode) == nullptr) {
            return false;
        }
        root = updateNode(root, upcCode, newItem);
        return true;
    }
    
    // Delete operation - Remove an item by UPC code
    bool remove(const std::string& upcCode) {
        if (searchNode(root, upcCode) == nullptr) {
            return false;  // Item not found
        }
        root = deleteNode(root, upcCode);
        return true;
    }
    
    // Display all items in order (by UPC code)
    void displayAllItems() const {
        if (root == nullptr) {
            std::cout << "Inventory is empty." << std::endl;
            return;
        }
    
        std::cout << "-------- GROCERY INVENTORY --------" << std::endl;
        displayAllItemsHelper(root);
        std::cout << "----------------------------------" << std::endl;
    }
    
    // Check if the tree is empty
    bool isEmpty() const {
        return root == nullptr;
    }
    // Load inventory from a file
    void loadFromFile(const std::string& filename) {
        std:: ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        std::string line;
        int lineNum = 0;
        while (std::getline(file, line)){
            lineNum++;
            if (line.empty()) continue; // Skip empty lines
            std::istringstream iss(line);
            std::string name, upcCode, price, quantity, aisle;
            double priceValue;
            int quantityValue;
            
            if (std::getline(iss, upcCode, ',') &&    // Read Field 1 into upcCode_val
                std::getline(iss, name, ',') &&       // Read Field 2 into name_val
                std::getline(iss, price, ',') &&      // Read Field 3 into price_str
                std::getline(iss, quantity, ',') &&   // Read Field 4 into quantity_str
                std::getline(iss, aisle)) {
                
                priceValue = std::stod(price);
                quantityValue = std::stoi(quantity);
                GroceryItem item(name, upcCode, priceValue, quantityValue, aisle);
                insert(item);
            }
        }
        file.close();

    }
    // Save inventory to a file
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        saveToFileHelper(root, file);
        file.close();
    }
    void saveToFileHelper(Node* node, std::ofstream& file) const {
        if (node == nullptr) return;
        
        saveToFileHelper(node->left, file);
        file << node->item.getName() << ","
             << node->item.getUpcCode() << ","
             << node->item.getPrice() << ","
             << node->item.getQuantity() << ","
             << node->item.getAisle() << "\n";
        saveToFileHelper(node->right, file);
    }
    // Clear the entire tree
    void clear() {
        clearTree(root);
        root = nullptr;
    }
};



#endif 