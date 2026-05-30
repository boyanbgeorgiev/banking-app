#pragma once
#include "Account.h"

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(double initialBalance, const std::string& owner, double overdraft = 0.0)
        : Account(initialBalance, owner, "CHK"), overdraftLimit(overdraft) {}

    void withdraw(double amount) override {
        if (locked) throw std::runtime_error("Account is locked.");
        if (amount <= 0) throw std::invalid_argument("Amount must be positive.");
        if (amount > balance + overdraftLimit)
            throw std::runtime_error("Exceeds overdraft limit.");
        balance -= amount;
        history.emplace_back(TxType::WITHDRAWAL, amount, id);
        std::cout << "Withdrew " << amount << ". New balance: " << balance << "\n";
    }

    void display() const override {
        std::cout << "CheckingAccount [" << id << "]"
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance
                  << " | Overdraft limit: " << overdraftLimit
                  << " | " << (locked ? "LOCKED" : "active") << "\n";
    }

    void setOverdraftLimit(double newLimit) {
        if (newLimit < 0) throw std::invalid_argument("Overdraft limit cannot be negative.");
        overdraftLimit = newLimit;
        std::cout << "Overdraft limit updated to " << newLimit << "\n";
    }

    double getOverdraftLimit() const { return overdraftLimit; }
};
