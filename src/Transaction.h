#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

enum class TxType { DEPOSIT, WITHDRAWAL, TRANSFER_IN, TRANSFER_OUT };

class Transaction {
private:
    TxType type;
    double amount;
    std::time_t timestamp;
    std::string accountId;

public:
    Transaction(TxType t, double amt, const std::string& accId)
        : type(t), amount(amt), timestamp(std::time(nullptr)), accountId(accId) {}

    TxType getType() const { return type; }
    double getAmount() const { return amount; }
    std::string getAccountId() const { return accountId; }

    std::string typeToString() const {
        switch (type) {
            case TxType::DEPOSIT:       return "DEPOSIT";
            case TxType::WITHDRAWAL:    return "WITHDRAWAL";
            case TxType::TRANSFER_IN:   return "TRANSFER_IN";
            case TxType::TRANSFER_OUT:  return "TRANSFER_OUT";
        }
        return "UNKNOWN";
    }

    void display() const {
        std::tm* t = std::localtime(&timestamp);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
        std::cout << "[" << buf << "] "
                  << typeToString()
                  << " | Amount: " << std::fixed << std::setprecision(2) << amount
                  << " | Account: " << accountId << "\n";
    }
};
