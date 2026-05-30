#pragma once
#include "Client.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include <vector>
#include <memory>
#include <algorithm>
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
            if (c->getName().find(name) != std::string::npos) return c.get();
        return nullptr;
    }

    void listClients() const {
        std::cout << "\n=== All Clients ===\n";
        if (clients.empty()) { std::cout << "No clients.\n"; return; }
        for (auto& c : clients) c->display();
    }

    bool updateClientName(const std::string& clientId, const std::string& newName) {
        Client* c = findClient(clientId);
        if (!c) return false;
        try { c->setName(newName); return true; }
        catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; return false; }
    }

    bool updateClientEmail(const std::string& clientId, const std::string& newEmail) {
        Client* c = findClient(clientId);
        if (!c) return false;
        try { c->setEmail(newEmail); return true; }
        catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; return false; }
    }

    bool removeClient(const std::string& clientId) {
        auto it = std::find_if(clients.begin(), clients.end(),
            [&](const std::unique_ptr<Client>& c){ return c->getId() == clientId; });
        if (it == clients.end()) return false;

        accounts.erase(
            std::remove_if(accounts.begin(), accounts.end(),
                [&](const std::unique_ptr<Account>& a){ return a->getOwnerId() == clientId; }),
            accounts.end());

        clients.erase(it);
        std::cout << "Client " << clientId << " and all their accounts removed.\n";
        return true;
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

    bool setAccountOverdraftLimit(const std::string& accId, double newLimit) {
        Account* acc = findAccount(accId);
        if (!acc) return false;
        CheckingAccount* chk = dynamic_cast<CheckingAccount*>(acc);
        if (!chk) { std::cout << "Account is not a checking account.\n"; return false; }
        try {
            chk->setOverdraftLimit(newLimit);
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            return false;
        }
    }

    bool setAccountInterestRate(const std::string& accId, double newRate) {
        Account* acc = findAccount(accId);
        if (!acc) return false;
        SavingsAccount* sav = dynamic_cast<SavingsAccount*>(acc);
        if (!sav) { std::cout << "Account is not a savings account.\n"; return false; }
        try {
            sav->setInterestRate(newRate);
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            return false;
        }
    }

    void listAllAccounts() const {
        std::cout << "\n=== All Accounts ===\n";
        if (accounts.empty()) { std::cout << "No accounts.\n"; return; }
        for (auto& a : accounts) a->display();
    }
};
