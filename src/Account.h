#pragma once
#include "BankEntity.h"
#include "Transaction.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

class Account : public BankEntity {
protected:
    double balance;
    bool locked;
    std::vector<Transaction> history;
    std::string ownerId;

public:
    Account(double initialBalance, const std::string& owner, const std::string& prefix)
        : BankEntity(prefix), balance(initialBalance), locked(false), ownerId(owner) {}

    virtual ~Account() = default;

    double getBalance() const { return balance; }
    bool isLocked() const { return locked; }
    std::string getOwnerId() const { return ownerId; }

    void lock()   { locked = true;  std::cout << "Account " << id << " locked.\n"; }
    void unlock() { locked = false; std::cout << "Account " << id << " unlocked.\n"; }

    void deposit(double amount) {
        if (locked) throw std::runtime_error("Account is locked.");
        if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive.");
        balance += amount;
        history.emplace_back(TxType::DEPOSIT, amount, id);
        std::cout << "Deposited " << amount << ". New balance: " << balance << "\n";
    }

    virtual void withdraw(double amount) {
        if (locked) throw std::runtime_error("Account is locked.");
        if (amount <= 0) throw std::invalid_argument("Amount must be positive.");
        if (amount > balance) throw std::runtime_error("Insufficient funds.");
        balance -= amount;
        history.emplace_back(TxType::WITHDRAWAL, amount, id);
        std::cout << "Withdrew " << amount << ". New balance: " << balance << "\n";
    }

    void transfer(Account& target, double amount) {
        if (locked) throw std::runtime_error("Source account is locked.");
        if (target.isLocked()) throw std::runtime_error("Target account is locked.");
        if (amount <= 0) throw std::invalid_argument("Amount must be positive.");
        withdraw(amount);
        history.back() = Transaction(TxType::TRANSFER_OUT, amount, id);
        target.balance += amount;
        target.history.emplace_back(TxType::TRANSFER_IN, amount, target.id);
        std::cout << "Transferred " << amount << " to account " << target.getId() << "\n";
    }

    void printHistory(TxType* filterType = nullptr) const {
        std::cout << "--- Transaction history for account " << id << " ---\n";
        bool found = false;
        for (const auto& tx : history) {
            if (!filterType || tx.getType() == *filterType) {
                tx.display();
                found = true;
            }
        }
        if (!found) std::cout << "No transactions found.\n";
    }

    bool exportHistory(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "Transaction history for account " << id << "\n";
        file << std::string(40, '-') << "\n";
        for (const auto& tx : history) {
            file << tx.typeToString()
                 << " | Amount: " << std::fixed << std::setprecision(2) << tx.getAmount()
                 << " | Account: " << tx.getAccountId() << "\n";
        }
        if (history.empty()) file << "No transactions.\n";
        return true;
    }

    virtual void applyInterest() {}

    virtual void display() const = 0;
};
