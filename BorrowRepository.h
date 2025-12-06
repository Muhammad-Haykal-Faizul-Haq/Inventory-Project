#pragma once
#include <vector>
#include <string>

struct Borrow {
    std::string username;
    int itemId;
    int quantity;
};

class BorrowRepository {
private:
    std::vector<Borrow> borrowList;
    std::string filename;

public:
    BorrowRepository(const std::string& file);

    void loadFromFile();
    void saveToFile();

    void addBorrow(const std::string& username, int itemId, int qty);
    const std::vector<Borrow>& getBorrowList() const;

    void removeBorrowAt(size_t index);
    void reduceBorrowQuantity(size_t index, int qty);
};
