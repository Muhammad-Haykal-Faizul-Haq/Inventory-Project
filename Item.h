#pragma once
#include <string>

class Item {
private:
    int itemId;
    std::string itemName;
    int quantity;

public:
    Item();
    Item(int itemId, const std::string &name, int quantity);

    int getItemId() const;
    std::string getItemName() const;
    int getQuantity() const;

    void setName(const std::string &itemName);
    void setQuantity(int quantity);

};