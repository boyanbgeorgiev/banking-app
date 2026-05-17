#pragma once
#include "Client.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Bank {
private:
    std::vector<std::unique_ptr<Client>> clients;
    std::vector<std::unique_ptr<Account>> accounts;

public:
    Client* addClient(const std::string& name, const std::string& email) {
        clients.push_back(std::make_unique<Client>(name, email));
        return clients.back().get();
    }

    Client* findClient(const std::string& id) const {
        for (auto& c : clients)
            if (c->getId() == id) return c.get();
        return nullptr;
    }

    Client* findClientByName(const std::string& name) const {
        for (auto& c : clients)
            if (c->getName() == name) return c.get();
        return nullptr;
    }

    void listClients() const {
        std::cout << "\n=== All Clients ===\n";
        if (clients.empty()) { std::cout << "No clients.\n"; return; }
        for (auto& c : clients) c->display();
    }

    Account* createChecking(Client* client, double balance, double overdraft = 0) {
        auto acc = std::make_unique<CheckingAccount>(balance, client->getId(), overdraft);
        Account* ptr = acc.get();
        accounts.push_back(std::move(acc));
        client->addAccount(ptr);
        std::cout << "Created CheckingAccount " << ptr->getId() << "\n";
        return ptr;
    }

    Account* createSavings(Client* client, double balance,
                           double rate = 0.03, double minBal = 100.0) {
        auto acc = std::make_unique<SavingsAccount>(balance, client->getId(), rate, minBal);
        Account* ptr = acc.get();
        accounts.push_back(std::move(acc));
        client->addAccount(ptr);
        std::cout << "Created SavingsAccount " << ptr->getId() << "\n";
        return ptr;
    }

    Account* findAccount(const std::string& id) const {
        for (auto& a : accounts)
            if (a->getId() == id) return a.get();
        return nullptr;
    }

    void applyInterestAll() {
        std::cout << "\n--- Applying interest to all savings accounts ---\n";
        for (auto& a : accounts) a->applyInterest();
    }

    void listAllAccounts() const {
        std::cout << "\n=== All Accounts ===\n";
        if (accounts.empty()) { std::cout << "No accounts.\n"; return; }
        for (auto& a : accounts) a->display();
    }
};
