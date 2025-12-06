#include "BorrowRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>

BorrowRepository::BorrowRepository(const std::string& file) {
    filename = file;
}

void BorrowRepository::loadFromFile() {
    borrowList.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, itemIdStr, quantityStr;

        // Split line by commas
        if (!std::getline(ss, username, ',')) continue;
        if (!std::getline(ss, itemIdStr, ',')) continue;
        if (!std::getline(ss, quantityStr, ',')) continue;

        int itemId = std::stoi(itemIdStr);
        int qty = std::stoi(quantityStr);

        borrowList.push_back({username, itemId, qty});
    }

    file.close();
}

void BorrowRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& b : borrowList) {
        file << b.username << "," << b.itemId << "," << b.quantity << "\n";
    }

    file.close();
}

void BorrowRepository::addBorrow(const std::string& username, int itemId, int qty) {
    Borrow b{username, itemId, qty};
    borrowList.push_back(b);
}

const std::vector<Borrow>& BorrowRepository::getBorrowList() const {
    return borrowList;
}

void BorrowRepository::removeBorrowAt(size_t index) {
    if (index < borrowList.size())
        borrowList.erase(borrowList.begin() + index);
}

void BorrowRepository::reduceBorrowQuantity(size_t index, int qty) {
    if (index < borrowList.size())
        borrowList[index].quantity -= qty;
}
