#pragma once
#include <vector>
#include <string>
#include "Item.h"

class ItemRepository {
private:
    std::vector<Item> items;
    std::string filename;

    void loadItems();
    void saveItems();

public:
    ItemRepository(const std::string &filename);

    std::vector<Item> getAllItems() const;
    Item* getItemById(int id);

    void addItem(const Item &item);
    bool removeItem(int id);
    bool updateItem(const Item &updatedItem);
    void saveToFile();
    Item* findById(int id);

};