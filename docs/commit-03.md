# Commit 3 — Базова структура: BankEntity и Transaction

## Какво правиш в тази стъпка

Създаваш `src/` папката и двата най-базови класа:
- `BankEntity` — абстрактен базов клас за всичко в системата
- `Transaction` + `TxType` enum — запис за всяка транзакция

---

## Стъпка 1: Създай папката

```bash
mkdir src
```

---

## Стъпка 2: Създай src/BankEntity.h

Отвори текстов редактор и създай файла `src/BankEntity.h` със следното съдържание:

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

## Стъпка 3: Създай src/Transaction.h

Създай файла `src/Transaction.h`:

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

## Стъпка 4: Валидация — провери файловете

```bash
ls src/
```

Трябва да видиш:
```
BankEntity.h  Transaction.h
```

```bash
cat src/BankEntity.h | grep "virtual void display"
```

Трябва да видиш:
```
    virtual void display() const = 0;
```

```bash
cat src/Transaction.h | grep "enum class TxType"
```

Трябва да видиш:
```
enum class TxType { DEPOSIT, WITHDRAWAL, TRANSFER_IN, TRANSFER_OUT };
```

---

## Стъпка 5: Тест — компилира ли се изолирано?

```bash
g++ -std=c++17 -Wall -x c++ /dev/null -include src/BankEntity.h -include src/Transaction.h -fsyntax-only
```

Ако няма изход — **файловете са синтактически правилни**. Ако има грешка, провери дали си копирал кода точно.

---

## Стъпка 6: Git commit

```bash
git add src/BankEntity.h src/Transaction.h
git commit -m "Add base class BankEntity and Transaction with TxType enum"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш **3 реда** — двата стари + новия commit.

```bash
git show --stat HEAD
```

Трябва да видиш:
```
 src/BankEntity.h   | ...
 src/Transaction.h  | ...
 2 files changed, ...
```

---

## Следваща стъпка

Отиди на [commit-04.md](commit-04.md) — абстрактен клас Account и конкретните типове сметки.
