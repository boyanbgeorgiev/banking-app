#pragma once
#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;
    double minBalance;

public:
    SavingsAccount(double initialBalance, const std::string& owner,
                   double rate = 0.03, double minBal = 100.0)
        : Account(initialBalance, owner, "SAV"),
          interestRate(rate), minBalance(minBal) {}

    void withdraw(double amount) override {
        if (locked) throw std::runtime_error("Account is locked.");
        if (amount <= 0) throw std::invalid_argument("Amount must be positive.");
        if (balance - amount < minBalance)
            throw std::runtime_error("Cannot go below minimum balance.");
        balance -= amount;
        history.emplace_back(TxType::WITHDRAWAL, amount, id);
        std::cout << "Withdrew " << amount << ". New balance: " << balance << "\n";
    }

    void applyInterest() override {
        double interest = balance * interestRate;
        balance += interest;
        history.emplace_back(TxType::DEPOSIT, interest, id);
        std::cout << "Interest applied: " << interest
                  << ". New balance: " << balance << "\n";
    }

    void setInterestRate(double newRate) {
        if (newRate < 0) throw std::invalid_argument("Interest rate cannot be negative.");
        interestRate = newRate;
        std::cout << "Interest rate updated to " << (newRate * 100) << "%\n";
    }

    double getInterestRate() const { return interestRate; }

    void display() const override {
        std::cout << "SavingsAccount [" << id << "]"
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance
                  << " | Rate: " << (interestRate * 100) << "%"
                  << " | Min balance: " << minBalance
                  << " | " << (locked ? "LOCKED" : "active") << "\n";
    }
};
