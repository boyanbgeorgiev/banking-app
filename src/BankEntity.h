#pragma once
#include <string>
#include <ctime>

class BankEntity {
protected:
    std::string id;
    std::time_t createdAt;

    static std::string generateId(const std::string& prefix) {
        static int counter = 0;
        return prefix + std::to_string(++counter);
    }

public:
    BankEntity(const std::string& prefix)
        : id(generateId(prefix)), createdAt(std::time(nullptr)) {}

    virtual ~BankEntity() = default;

    std::string getId() const { return id; }
    virtual void display() const = 0;
};
