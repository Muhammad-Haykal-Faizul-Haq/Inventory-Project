#pragma once
#include <vector>
#include <string>
#include "User.h"

class UserRepository {
private:
    std::vector<User> users;   // stores all users

public:
    // Loads userList.txt
    void loadFromFile(const std::string& filename);

    // Returns pointer to user if login valid, nullptr otherwise
    User* validateLogin(const std::string& username,
                        const std::string& password);

    // Lookup function
    User* findByUsername(const std::string& username);

    // For debugging or admin list display
    const std::vector<User>& getAllUsers() const { return users; }
};
