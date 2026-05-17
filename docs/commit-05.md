# Commit 5 — Client и Bank класове

## Какво правиш в тази стъпка

Добавяш двата управляващи класа:
- `Client` — клиент, притежава множество сметки, генерира финансов отчет
- `Bank` — централен клас, управлява всички клиенти и сметки

**Изисква:** Commits 3 и 4 да са направени.

---

## Стъпка 1: Създай src/Client.h

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
```

---

## Стъпка 2: Създай src/Client.cpp

```cpp
#include "Client.h"
// All implementation is inline in the header.
```

---

## Стъпка 3: Създай src/Bank.h

```cpp
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
```

---

## Стъпка 4: Създай src/Bank.cpp

```cpp
#include "Bank.h"
// All implementation is inline in the header.
```

---

## Стъпка 5: Валидация — провери файловете

```bash
ls src/
```

Трябва да видиш 10 файла:
```
Account.cpp      Account.h       Bank.cpp        Bank.h
BankEntity.h     CheckingAccount.cpp  CheckingAccount.h
Client.cpp       Client.h        SavingsAccount.cpp
SavingsAccount.h Transaction.h
```

Провери ключови части:
```bash
grep "getNetWorth" src/Client.h
```
Трябва: `double getNetWorth() const {`

```bash
grep "unique_ptr" src/Bank.h
```
Трябва да видиш два реда с `unique_ptr<Client>` и `unique_ptr<Account>`.

---

## Стъпка 6: Тест — синтактична компилация

```bash
g++ -std=c++17 -Wall -I src -fsyntax-only \
    src/Account.cpp src/CheckingAccount.cpp src/SavingsAccount.cpp \
    src/Client.cpp src/Bank.cpp
```

**Очакван резултат:** без изход = без грешки.

---

## Стъпка 7: Тест — функционална проверка на Client и Bank

```bash
cat > /tmp/test_client_bank.cpp << 'EOF'
#include "Bank.h"
#include <iostream>
#include <cassert>

int main() {
    Bank bank;

    // Управление на клиенти
    Client* alice = bank.addClient("Alice", "alice@example.com");
    Client* bob   = bank.addClient("Bob",   "bob@example.com");
    assert(alice != nullptr);
    assert(bob   != nullptr);
    assert(alice->getName() == "Alice");

    // Управление на сметки
    Account* a1 = bank.createChecking(alice, 1000.0, 200.0);
    Account* a2 = bank.createSavings(alice,  500.0);
    Account* a3 = bank.createChecking(bob,   750.0);
    assert(a1 != nullptr);
    assert(alice->getAccounts().size() == 2);
    assert(bob->getAccounts().size()   == 1);

    // Нетна стойност
    assert(alice->getNetWorth() == 1500.0);

    // Намери сметка по ID
    Account* found = bank.findAccount(a1->getId());
    assert(found == a1);

    // Намери клиент по ID
    Client* foundClient = bank.findClient(alice->getId());
    assert(foundClient == alice);

    // Финансов отчет
    std::cout << "\n--- Client report test ---\n";
    alice->displayReport();

    std::cout << "\nAll Client and Bank tests passed.\n";
    return 0;
}
EOF

g++ -std=c++17 -Wall -I src /tmp/test_client_bank.cpp -o /tmp/test_client_bank && /tmp/test_client_bank
```

**Очакван изход:**
```
Created CheckingAccount CHK1
Created SavingsAccount SAV2
Created CheckingAccount CHK3

--- Client report test ---

=== Financial Report: Alice ===
CheckingAccount [CHK1] | Balance: 1000.00 | Overdraft limit: 200.00 | active
SavingsAccount  [SAV2] | Balance: 500.00  | Rate: 3% | Min balance: 100.00 | active
Total net worth: 1500.00

All Client and Bank tests passed.
```

Изтрий временния файл:
```bash
rm /tmp/test_client_bank.cpp /tmp/test_client_bank
```

---

## Стъпка 8: Git commit

```bash
git add src/Client.h src/Client.cpp \
        src/Bank.h   src/Bank.cpp
git commit -m "Add Client and Bank classes with account management"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш **5 реда**. Минималното изискване за commits е изпълнено!

```bash
git show --stat HEAD
```

Трябва да видиш 4 файла добавени.

---

## Следваща стъпка

Отиди на [commit-06.md](commit-06.md) — main.cpp, Makefile, проектът компилира напълно.
