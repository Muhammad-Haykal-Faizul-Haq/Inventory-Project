#include "UserRepository.h"
#include <fstream> // file stream
#include <sstream> // string stream
#include <iostream>

void UserRepository::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string idStr, username, password, roleStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, roleStr, ',');

        int id = std::stoi(idStr);
        int role = std::stoi(roleStr);    // stoi = string to integer

        users.emplace_back(id, username, password, role);
    }

    file.close();
}

User* UserRepository::validateLogin(const std::string& username, const std::string& password) {
    for (auto& u : users) {
        if (u.getUsername() == username && u.getPassword() == password) {
            return &u;
        }
    }
    return nullptr;
}
