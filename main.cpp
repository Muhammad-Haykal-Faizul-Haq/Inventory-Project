#include <iostream>
#include <string>
#include <iomanip>
#include "User.h"
#include "UserRepository.h"
#include "Item.h"
#include "ItemRepository.h"
#include "BorrowRepository.h"

// Borrowing Item Function 
void borrowItem(ItemRepository& itemRepo, BorrowRepository& borrowRepo, const std::string& username) {
    int id, qty;

    while (true) {
        std::cout << "Enter Item ID to borrow: ";
        std::cin >> id;

        Item* item = itemRepo.findById(id);
        if (!item) {
            std::cout << "Item not found! Try again.\n";
            continue;
        }

        std::cout << "Enter quantity to borrow: ";
        std::cin >> qty;

        if (qty <= 0) {
            std::cout << "Quantity must be positive.\n";
            continue;
        }

        if (qty > item->getQuantity()) {
            std::cout << "Not enough stock! Available: "
                      << item->getQuantity() << "\n";
            continue;
        }

        // SUCCESS: update stock
        item->setQuantity(item->getQuantity() - qty);
        itemRepo.saveToFile();

        // Log borrow
        borrowRepo.addBorrow(username, id, qty);
        borrowRepo.saveToFile();

        std::cout << "Successfully borrowed " << qty
                  << " of " << item->getItemName()
                  << ". Remaining: " << item->getQuantity() << "\n";
        break;
    }
}

// Users Viewing Their Borrowed Item Function
void viewUserBorrowedItems(BorrowRepository& borrowRepo, ItemRepository& itemRepo, const std::string& username) {
    const auto& borrowList = borrowRepo.getBorrowList();
    bool found = false;

    std::cout << "\nItems you have borrowed:\n";
    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(30) << "Item Name"
              << std::setw(10) << "Quantity" << "\n";
    std::cout << std::string(44, '-') << "\n";

    for (const auto& b : borrowList) {
        if (b.username == username) {
            Item* item = itemRepo.findById(b.itemId);
            std::string itemName = item ? item->getItemName() : "Unknown";

            std::cout << std::left
                      << std::setw(4) << b.itemId
                      << std::setw(30) << itemName
                      << std::setw(10) << b.quantity << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "You have not borrowed any items yet.\n";
    }
}

// Admins Validating Returns Function
void validateReturn(BorrowRepository& borrowRepo, ItemRepository& itemRepo) {
    const auto& borrowList = borrowRepo.getBorrowList();

    if (borrowList.empty()) {
        std::cout << "No borrowed items to validate.\n";
        return;
    }

    std::cout << "Currently borrowed items:\n";
    std::cout << std::left
              << std::setw(4) << "No"
              << std::setw(15) << "Username"
              << std::setw(4) << "ID"
              << std::setw(30) << "Item Name"
              << std::setw(8) << "Qty" << "\n";
    std::cout << std::string(61, '-') << "\n";

    int index = 1;
    for (auto& b : borrowRepo.getBorrowList()) {
        Item* item = itemRepo.findById(b.itemId);
        std::string itemName = item ? item->getItemName() : "Unknown";

        std::cout << std::left
                  << std::setw(4) << index++
                  << std::setw(15) << b.username
                  << std::setw(4) << b.itemId
                  << std::setw(30) << itemName
                  << std::setw(8) << b.quantity
                  << "\n";
    }

    int choice;
    std::cout << "\nEnter the number of the borrowed record to validate return (0 to cancel): ";
    std::cin >> choice;

    if (choice <= 0 || choice > borrowRepo.getBorrowList().size()) {
        std::cout << "Return cancelled.\n";
        return;
    }

    // Adjust the borrow record
    Borrow b = borrowRepo.getBorrowList()[choice - 1];

    int returnQty;
    std::cout << "Enter quantity to return (max " << b.quantity << "): ";
    std::cin >> returnQty;

    if (returnQty <= 0 || returnQty > b.quantity) {
        std::cout << "Invalid quantity.\n";
        return;
    }

    // Update item stock
    Item* item = itemRepo.findById(b.itemId);
    if (item) {
        item->setQuantity(item->getQuantity() + returnQty);
        itemRepo.saveToFile();
    }

    // Update borrow record
    if (returnQty == b.quantity) {
        // Full return, remove record
        borrowRepo.removeBorrowAt(choice - 1);
    } else {
        // Partial return, reduce quantity
        borrowRepo.reduceBorrowQuantity(choice - 1, returnQty);
    }

    borrowRepo.saveToFile();

    std::cout << "Return validated successfully.\n";
}

int main() {
    BorrowRepository borrowRepo("borrowList.txt");
    borrowRepo.loadFromFile();

    UserRepository repo;
    ItemRepository itemRepo("itemList.txt");

    repo.loadFromFile("userList.txt");
    std::string username, password;

    std::cout << "Inventaris Perlengkapan App\n";

    // Start Menu Loop
    while (true) {
        std::cout << "===== START MENU =====\n";
        std::cout << "1. Login\n";
        std::cout << "2. Exit\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;

        if (choice == 2) {
            std::cout << "Goodbye!\n";
            std::cout << "Press Enter to exit...";
            std::cin.ignore();
            std::cin.get();
            return 0;   // closes the app
        }

        if (choice == 1) {
            break;      // go to login
        }

        std::cout << "Invalid choice.\n\n";
    }

    std::string roleNow; // roleNow is a local variable to check what the current user's role is
    std::string loggedInUsername; // same but for username
    User* loggedIn = nullptr;  // start with no user

    // Login Menu Loop
    std::cout << "\n===== LOGIN MENU =====";
    while (true) {
    std::cout << "\nEnter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    std::cin >> password;

    // Validate after user inputs credentials
    loggedIn = repo.validateLogin(username, password);

    if (loggedIn == nullptr) {
        std::cout << "\nInvalid login.\nTry again.\n";
    } else {
        break; // stop loop if login is successful
    }
    }

    // Login Success Greeting
    roleNow = (loggedIn->getRole() == 0) ? "User" : "Admin";
    loggedInUsername = loggedIn->getUsername();
    std::cout << "\nSelamat datang " << loggedInUsername
              << "!\nKamu berhasil login sebagai " << roleNow << std::endl;

    // Main Menu Loop
    std::cout << "===== MAIN MENU =====\n";
    while (true) {
        std::cout << "0. Exit\n";
        std::cout << "1. View Stock\n";
        std::cout << "2. Borrow item\n";
        std::cout << "3. My borrowed items\n"; // Formerly called "Return item"

        // Admin Menu Choice for Admins
        if(roleNow == "Admin"){
            std::cout << "10. View Borrowed Items\n"; // Formerly called "Admin Menu"
            std::cout << "11. Validate a Return\n";
        }
        std::cout << "Input: ";
        
        int choice;
        std::cin >> choice;

        // Exit
        if (choice == 0) {
            std::cout << "Goodbye!\n";
            std::cout << "Press Enter to exit...";
            std::cin.ignore();
            std::cin.get();
            return 0;   // closes the app
        }

        // View Stock
        if (choice == 1) {
            // Show the itemlist
            std::vector<Item> items = itemRepo.getAllItems();
            std::cout << "Daftar barang yang tersedia di inventaris kami :\n";
            std::cout << std::left
                << std::setw(4) << "ID"
                << std::setw(30) << "Name"
                << std::setw(10) << "Quantity"
                << "\n";

            std::cout << std::string(40, '-') << "\n"; // separator line

            for (const auto& item : items) {
                std::cout << std::left
                        << std::setw(4) << item.getItemId()
                        << std::setw(30) << item.getItemName()
                        << std::setw(10) << item.getQuantity()
                        << "\n";
                }
        }

        // Borrow Item
        if (choice == 2) {
            borrowItem(itemRepo, borrowRepo, loggedInUsername);
        }

        // Return Item
        if (choice == 3) {
            viewUserBorrowedItems(borrowRepo, itemRepo, loggedInUsername);
            std::cout << "Please return the item to Admin.\n";
        }

        // Admin Menu to View all Borrowed Items
        if (choice == 10 && roleNow == "Admin") { // Admin menu
            std::cout << "Currently borrowed items:\n";
            for (auto &b : borrowRepo.getBorrowList()) {
            Item* item = itemRepo.findById(b.itemId);
            std::string itemName = item ? item->getItemName() : "Unknown";
            std::cout << b.username << " borrowed " << b.quantity
                    << " of " << itemName << "\n";
            }
        }
    
        // Admins can Validate Returns
        if (choice == 11 && roleNow == "Admin") { // Admin Return Validation
            validateReturn(borrowRepo, itemRepo);
        }
    }
    
    std::cout << "Press Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
