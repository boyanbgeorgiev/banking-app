#pragma once
#include "BankEntity.h"
#include "Account.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>

class Client : public BankEntity {
private:
    std::string name;
    std::string email;
    std::vector<Account*> accounts;

public:
    Client(const std::string& n, const std::string& e)
        : BankEntity("CLI"), name(n), email(e) {}

    std::string getName() const { return name; }
    std::string getEmail() const { return email; }

    void addAccount(Account* acc) { accounts.push_back(acc); }

    const std::vector<Account*>& getAccounts() const { return accounts; }

    double getNetWorth() const {
        double total = 0;
        for (auto* acc : accounts) total += acc->getBalance();
        return total;
    }

    Account* findAccount(const std::string& accId) const {
        for (auto* acc : accounts)
            if (acc->getId() == accId) return acc;
        return nullptr;
    }

    void display() const override {
        std::cout << "Client [" << id << "] " << name
                  << " | Email: " << email
                  << " | Accounts: " << accounts.size()
                  << " | Net worth: " << std::fixed << std::setprecision(2)
                  << getNetWorth() << "\n";
    }

    void displayReport() const {
        std::cout << "\n=== Financial Report: " << name << " ===\n";
        std::vector<Account*> sorted = accounts;
        std::sort(sorted.begin(), sorted.end(),
            [](Account* a, Account* b){ return a->getBalance() > b->getBalance(); });
        for (auto* acc : sorted) acc->display();
        std::cout << "Total net worth: " << std::fixed << std::setprecision(2)
                  << getNetWorth() << "\n";
    }
};
