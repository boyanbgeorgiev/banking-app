# Commit 4 — Абстрактен клас Account + CheckingAccount + SavingsAccount

## Какво правиш в тази стъпка

Добавяш трите класа за сметки:
- `Account` — абстрактен базов клас с обща логика (deposit, withdraw, transfer, history)
- `CheckingAccount` — разплащателна сметка с овърдрафт лимит
- `SavingsAccount` — спестовна сметка с лихва и минимален баланс

**Изисква:** Commit 3 да е направен (`src/BankEntity.h` и `src/Transaction.h` да съществуват).

---

## Стъпка 1: Създай src/Account.h

```cpp
#pragma once
#include "BankEntity.h"
#include "Transaction.h"
#include <vector>
#include <iostream>
#include <iomanip>
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

    virtual void applyInterest() {}

    virtual void display() const = 0;
};
```

---

## Стъпка 2: Създай src/Account.cpp

```cpp
#include "Account.h"
// All implementation is inline in the header for this project size.
```

---

## Стъпка 3: Създай src/CheckingAccount.h

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

## Стъпка 4: Създай src/CheckingAccount.cpp

```cpp
#include "CheckingAccount.h"
// All implementation is inline in the header.
```

---

## Стъпка 5: Създай src/SavingsAccount.h

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

## Стъпка 6: Създай src/SavingsAccount.cpp

```cpp
#include "SavingsAccount.h"
// All implementation is inline in the header.
```

---

## Стъпка 7: Валидация — провери файловете

```bash
ls src/
```

Трябва да видиш:
```
Account.cpp  Account.h  BankEntity.h  CheckingAccount.cpp
CheckingAccount.h  SavingsAccount.cpp  SavingsAccount.h  Transaction.h
```

Провери ключови неща:
```bash
grep "virtual void withdraw" src/Account.h
```
Трябва: `virtual void withdraw(double amount) {`

```bash
grep "void applyInterest" src/SavingsAccount.h
```
Трябва: `void applyInterest() override {`

```bash
grep "overdraftLimit" src/CheckingAccount.h
```
Трябва: `double overdraftLimit;`

---

## Стъпка 8: Тест — синтактична компилация

```bash
g++ -std=c++17 -Wall -I src -fsyntax-only src/Account.cpp src/CheckingAccount.cpp src/SavingsAccount.cpp
```

**Очакван резултат:** без изход = без грешки.

Ако има грешка от типа `error: 'BankEntity' was not declared` — провери дали `#include "BankEntity.h"` е в `Account.h`.

---

## Стъпка 9: Тест — бърза функционална проверка

Създай временен тестов файл (ще го изтриеш след това):

```bash
cat > /tmp/test_accounts.cpp << 'EOF'
#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include <iostream>
#include <cassert>

int main() {
    CheckingAccount chk(1000.0, "CLI1", 200.0);
    chk.deposit(500.0);
    assert(chk.getBalance() == 1500.0);

    chk.withdraw(200.0);
    assert(chk.getBalance() == 1300.0);

    // Овърдрафт — трябва да мине
    chk.withdraw(1400.0);
    assert(chk.getBalance() == -100.0);

    SavingsAccount sav(1000.0, "CLI1", 0.1, 100.0);
    sav.applyInterest();
    assert(sav.getBalance() == 1100.0);

    // Минимален баланс — трябва да хвърли грешка
    try {
        sav.withdraw(1100.0);
        assert(false && "Should have thrown");
    } catch (const std::runtime_error&) {
        std::cout << "Min balance protection works.\n";
    }

    std::cout << "All account tests passed.\n";
    return 0;
}
EOF

g++ -std=c++17 -Wall -I src /tmp/test_accounts.cpp -o /tmp/test_accounts && /tmp/test_accounts
```

**Очакван изход:**
```
Deposited 500. New balance: 1500
Withdrew 200. New balance: 1300
Withdrew 1400. New balance: -100
Interest applied: 100. New balance: 1100
Min balance protection works.
All account tests passed.
```

Изтрий временния файл:
```bash
rm /tmp/test_accounts.cpp /tmp/test_accounts
```

---

## Стъпка 10: Git commit

```bash
git add src/Account.h src/Account.cpp \
        src/CheckingAccount.h src/CheckingAccount.cpp \
        src/SavingsAccount.h  src/SavingsAccount.cpp
git commit -m "Implement Account hierarchy: CheckingAccount and SavingsAccount"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш **4 реда**.

```bash
git show --stat HEAD
```

Трябва да видиш 6 файла добавени.

---

## Следваща стъпка

Отиди на [commit-05.md](commit-05.md) — Client и Bank класовете.
