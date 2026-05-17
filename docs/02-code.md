# Пълен C++ код

Копирай всеки блок в съответния файл. Започни с `mkdir src` от root папката на проекта.

---

## src/BankEntity.h

```cpp
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
```

---

## src/Transaction.h

```cpp
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
```

---

## src/Account.h

```cpp
#pragma once
#include "BankEntity.h"
#include "Transaction.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

class Client;  // forward declaration

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
        // Replace last WITHDRAWAL with TRANSFER_OUT
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

    virtual void applyInterest() {}  // virtual, not pure — CheckingAccount does nothing

    virtual void display() const = 0;
};
```

---

## src/Account.cpp

```cpp
#include "Account.h"
// All implementation is inline in the header for this project size.
```

---

## src/CheckingAccount.h

```cpp
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
};
```

---

## src/CheckingAccount.cpp

```cpp
#include "CheckingAccount.h"
// All implementation is inline in the header.
```

---

## src/SavingsAccount.h

```cpp
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

    void display() const override {
        std::cout << "SavingsAccount [" << id << "]"
                  << " | Balance: " << std::fixed << std::setprecision(2) << balance
                  << " | Rate: " << (interestRate * 100) << "%"
                  << " | Min balance: " << minBalance
                  << " | " << (locked ? "LOCKED" : "active") << "\n";
    }
};
```

---

## src/SavingsAccount.cpp

```cpp
#include "SavingsAccount.h"
// All implementation is inline in the header.
```

---

## src/Client.h

```cpp
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
    std::vector<Account*> accounts;  // non-owning pointers (Bank owns accounts)

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
```

---

## src/Client.cpp

```cpp
#include "Client.h"
// All implementation is inline in the header.
```

---

## src/Bank.h

```cpp
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
```

---

## src/Bank.cpp

```cpp
#include "Bank.h"
// All implementation is inline in the header.
```

---

## src/main.cpp

```cpp
#include <iostream>
#include <string>
#include <limits>
#include "Bank.h"

Bank bank;

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void menuDeposit() {
    std::string accId;
    double amount;
    std::cout << "Account ID: "; std::cin >> accId;
    std::cout << "Amount: ";     std::cin >> amount;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    try { acc->deposit(amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuWithdraw() {
    std::string accId;
    double amount;
    std::cout << "Account ID: "; std::cin >> accId;
    std::cout << "Amount: ";     std::cin >> amount;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    try { acc->withdraw(amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuTransfer() {
    std::string fromId, toId;
    double amount;
    std::cout << "From account ID: "; std::cin >> fromId;
    std::cout << "To account ID: ";   std::cin >> toId;
    std::cout << "Amount: ";          std::cin >> amount;
    Account* from = bank.findAccount(fromId);
    Account* to   = bank.findAccount(toId);
    if (!from || !to) { std::cout << "Account not found.\n"; return; }
    try { from->transfer(*to, amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuHistory() {
    std::string accId;
    std::cout << "Account ID: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    acc->printHistory();
}

void menuCreateClient() {
    std::string name, email;
    std::cout << "Name: ";  std::cin >> name;
    std::cout << "Email: "; std::cin >> email;
    Client* c = bank.addClient(name, email);
    std::cout << "Client created: " << c->getId() << "\n";
}

void menuCreateAccount() {
    std::string clientId, type;
    double balance;
    std::cout << "Client ID: ";          std::cin >> clientId;
    std::cout << "Type (chk/sav): ";     std::cin >> type;
    std::cout << "Initial balance: ";    std::cin >> balance;
    Client* c = bank.findClient(clientId);
    if (!c) { std::cout << "Client not found.\n"; return; }
    if (type == "chk") {
        double overdraft;
        std::cout << "Overdraft limit: "; std::cin >> overdraft;
        bank.createChecking(c, balance, overdraft);
    } else if (type == "sav") {
        bank.createSavings(c, balance);
    } else {
        std::cout << "Unknown type.\n";
    }
}

int main() {
    std::cout << "=== Banking System ===\n";

    // Demo data so the program shows something immediately
    Client* alice = bank.addClient("Alice", "alice@example.com");
    Client* bob   = bank.addClient("Bob",   "bob@example.com");
    Account* a1 = bank.createChecking(alice, 1000.0, 200.0);
    Account* a2 = bank.createSavings(alice,  500.0);
    Account* a3 = bank.createChecking(bob,   750.0);

    int choice = 0;
    while (true) {
        std::cout << "\n--- Menu ---\n"
                  << "1. List all clients\n"
                  << "2. List all accounts\n"
                  << "3. Create client\n"
                  << "4. Create account\n"
                  << "5. Deposit\n"
                  << "6. Withdraw\n"
                  << "7. Transfer\n"
                  << "8. Transaction history\n"
                  << "9. Client report\n"
                  << "10. Apply interest (savings)\n"
                  << "0. Exit\n"
                  << "Choice: ";
        std::cin >> choice;
        if (std::cin.fail()) { clearInput(); continue; }

        switch (choice) {
            case 1: bank.listClients(); break;
            case 2: bank.listAllAccounts(); break;
            case 3: menuCreateClient(); break;
            case 4: menuCreateAccount(); break;
            case 5: menuDeposit(); break;
            case 6: menuWithdraw(); break;
            case 7: menuTransfer(); break;
            case 8: menuHistory(); break;
            case 9: {
                std::string id;
                std::cout << "Client ID: "; std::cin >> id;
                Client* c = bank.findClient(id);
                if (c) c->displayReport();
                else std::cout << "Not found.\n";
                break;
            }
            case 10: bank.applyInterestAll(); break;
            case 0: std::cout << "Bye.\n"; return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
```

---

## Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src
SRC = $(wildcard src/*.cpp)
TARGET = banking

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```
