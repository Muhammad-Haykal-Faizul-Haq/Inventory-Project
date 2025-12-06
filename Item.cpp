#include "Item.h"

Item::Item() : itemId(0), itemName(""), quantity(0) {}

Item::Item(int itemId, const std::string &name, int quantity)
    : itemId(itemId), itemName(name), quantity(quantity) {}

int Item::getItemId() const { return itemId; }
std::string Item::getItemName() const { return itemName; }
int Item::getQuantity() const { return quantity; }

void Item::setName(const std::string &itemName) { this->itemName = itemName; }
void Item::setQuantity(int quantity) { this->quantity = quantity; }
