#include "ItemRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>

ItemRepository::ItemRepository(const std::string &filename)
    : filename(filename) {
    loadItems();
}

void ItemRepository::loadItems() {
    items.clear();
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string idStr, name, qtyStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, qtyStr, ',');

        if (idStr.empty() || name.empty() || qtyStr.empty())
            continue;

        int id = std::stoi(idStr);
        int qty = std::stoi(qtyStr);

        items.emplace_back(id, name, qty);
    }
}

void ItemRepository::saveItems() {
    std::ofstream file(filename);

    for (const auto &item : items) {
        file << item.getItemId() << ","
             << item.getItemName() << ","
             << item.getQuantity() << "\n";
    }
}

std::vector<Item> ItemRepository::getAllItems() const {
    return items;
}

Item* ItemRepository::getItemById(int id) {
    for (auto &item : items) {
        if (item.getItemId() == id)
            return &item;
    }
    return nullptr;
}

void ItemRepository::addItem(const Item &item) {
    items.push_back(item);
    saveItems();
}

bool ItemRepository::removeItem(int id) {
    for (int i = 0; i < items.size(); i++) {
        if (items[i].getItemId() == id) {
            items.erase(items.begin() + i);
            saveItems();
            return true;
        }
    }
    return false;
}

bool ItemRepository::updateItem(const Item &updatedItem) {
    for (auto &item : items) {
        if (item.getItemId() == updatedItem.getItemId()) {
            item = updatedItem;
            saveItems();
            return true;
        }
    }
    return false;
}

void ItemRepository::saveToFile() {
    std::ofstream file(filename);   // overwrite the file

    if (!file.is_open()) {
        std::cout << "Error: cannot open " << filename << "\n";
        return;
    }

    for (auto& item : items) {
        file << item.getItemId() << ","
             << item.getItemName() << ","
             << item.getQuantity() << "\n";
    }

    file.close();
}

Item* ItemRepository::findById(int id) {
    for (auto &item : items) {   // iterate through vector<Item>
        if (item.getItemId() == id) {
            return &item;        // return pointer to the item
        }
    }
    return nullptr;              // not found
}
