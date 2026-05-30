# Commit 16 — Bank Statistics Report

## Какво правиш в тази стъпка

Добавяш нова функционалност **20. Bank statistics** — извежда глобална статистика за цялата банка:
- Брой клиенти
- Брой сметки (общо, разплащателни, спестовни)
- Общ актив (сума на всички баланси)
- Средна стойност на сметка

**Изисква:** Commit 15 да е направен.

---

## Стъпка 1: Добави printStats в src/Bank.h

В началото на `Bank.h` добави към include-овете (ако липсва):
```cpp
#include <iostream>
#include <iomanip>
```

В класа `Bank`, след `listAllAccounts()`, добави:

```cpp
void printStats() const {
    int totalClients = clients.size();
    int totalAccounts = accounts.size();
    int checkingCount = 0, savingsCount = 0;
    double totalAssets = 0.0;

    for (const auto& a : accounts) {
        totalAssets += a->getBalance();
        if (dynamic_cast<CheckingAccount*>(a.get())) ++checkingCount;
        else ++savingsCount;
    }

    double avgBalance = totalAccounts > 0 ? totalAssets / totalAccounts : 0.0;

    std::cout << "\n=== Bank Statistics ===\n"
              << "Clients:          " << totalClients << "\n"
              << "Total accounts:   " << totalAccounts << "\n"
              << "  Checking:       " << checkingCount << "\n"
              << "  Savings:        " << savingsCount << "\n"
              << "Total assets:     " << std::fixed << std::setprecision(2) << totalAssets << "\n"
              << "Average balance:  " << avgBalance << "\n";
}
```

---

## Стъпка 2: Добави menu функцията в src/main.cpp

Добави след `menuExportHistory()`:

```cpp
void menuBankStats() {
    bank.printStats();
}
```

---

## Стъпка 3: Обнови менюто и switch в main()

В менюто добави след опция 19:
```
<< "20. Bank statistics\n"
```

В switch добави:
```cpp
case 20: menuBankStats(); break;
```

---

## Стъпка 4: Валидация — компилация

```bash
make
```

Без грешки.

---

## Стъпка 5: Функционален тест

```bash
./banking
```

1. Избери **20** → трябва да видиш статистика за 2 клиента, 3 сметки (2 разплащателни + 1 спестовна), общ актив = 1000 + 500 + 750 = 2250.00
2. Добави нов клиент и сметка, след това провери **20** отново — числата трябва да се актуализират

---

## Стъпка 6: Git commit

```bash
git add src/Bank.h src/main.cpp
git commit -m "Add bank statistics report with totals and averages (menu option 20)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-17.md](commit-17.md) — последния commit.
