#pragma once
#include <string>

class User {
private:
    int id;
    std::string password;
    std::string username;
    int role;

public:
    User(int id,
         const std::string& username,
         const std::string& password,
         int role)
        : id(id), username(username), password(password), role(role) {}

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    int getRole() const { return role; }
};
