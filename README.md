# Grocery Store Simulation - CSC 307 Final Project

## Project Members
- Omar Abdelkhaleq (w10193627)
- Muhammad Abdullah Shaikh (w1019005)
- Samir Bhattarai (w10174370)
- Trinav Dhakal (w10194047)

## Project Description
This project simulates a grocery store inventory and shopping system using an AVL Tree for efficient data management. Users can add, update, delete, and search for grocery items. The program also features a shopping cart system, transaction processing, and sales reporting.

The primary data structure used is an AVL Tree to ensure balanced, efficient inventory operations.

## How to Compile
Open a terminal or command prompt, navigate to the project directory, and run:

```bash
g++ main.cpp TransactionProcessing.cpp -o GroceryStoreSimulator
```

This will compile all necessary files and generate an executable named `GroceryStoreSimulator`.

## How to Run
- On Linux/macOS:
  ```bash
  ./GroceryStoreSimulator
  ```
- On Windows:
  ```bash
  GroceryStoreSimulator.exe
  ```

## Features
- **Inventory Management**: Add, search, update, and delete grocery items using an AVL Tree.
- **Shopping Cart**: Add and remove items from a virtual shopping cart, with real-time inventory updates.
- **Checkout System**: Calculate total purchase cost including 8.25% sales tax, generate receipts.
- **Transaction Processing**: Record each completed transaction in a persistent `transactions.csv` file.
- **Sales Reporting**:
  - View all previous transactions.
  - Generate sales analytics such as total revenue and best-selling items.

## Notes
- The program automatically inserts a few sample items (Milk, Bread, Eggs, Apples, Chicken) into the inventory at startup.
- All completed purchases are logged into `transactions.csv` automatically.
- Transactions are saved even after the program exits.
- Guest checkouts are supported by default.

## File Structure
- `main.cpp` – Main menu and program logic
- `AVLTree.h` – Inventory AVL Tree implementation
- `GroceryItem.h` – Grocery item class
- `ShoppingCart.h` – Shopping cart management
- `TransactionProcessing.h/cpp` – Transaction classes and file persistence
- `transactions.csv` – Auto-generated file to store transaction history

---

# Thank you for reviewing our project!
