# Commit 14 — Change Overdraft Limit on Checking Account

## Какво правиш в тази стъпка

Добавяш нова функционалност **18. Change overdraft limit** — позволява промяна на овърдрафт лимита на разплащателна сметка.
Аналогично на commit 12 (setInterestRate), но за `CheckingAccount`.

**Изисква:** Commit 13 да е направен.

---

## Стъпка 1: Добави сетър в src/CheckingAccount.h

В класа `CheckingAccount`, след метода `display()`, добави:

```cpp
void setOverdraftLimit(double newLimit) {
    if (newLimit < 0) throw std::invalid_argument("Overdraft limit cannot be negative.");
    overdraftLimit = newLimit;
    std::cout << "Overdraft limit updated to " << newLimit << "\n";
}

double getOverdraftLimit() const { return overdraftLimit; }
```

---

## Стъпка 2: Добави метод в Bank.h

В класа `Bank`, след `setAccountInterestRate()`, добави:

```cpp
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
```

Увери се, че `#include "CheckingAccount.h"` е включен в `Bank.h` (вече е).

---

## Стъпка 3: Добави menu функцията в src/main.cpp

Добави след `menuEditClient()`:

```cpp
void menuChangeOverdraftLimit() {
    std::string accId;
    double limit;
    std::cout << "Checking account ID: "; std::cin >> accId;
    std::cout << "New overdraft limit: "; std::cin >> limit;
    if (!bank.setAccountOverdraftLimit(accId, limit))
        std::cout << "Failed to update overdraft limit.\n";
}
```

---

## Стъпка 4: Обнови менюто и switch в main()

В менюто добави след опция 17:
```
<< "18. Change overdraft limit (checking)\n"
```

В switch добави:
```cpp
case 18: menuChangeOverdraftLimit(); break;
```

---

## Стъпка 5: Валидация — компилация

```bash
make
```

Без грешки.

---

## Стъпка 6: Функционален тест

```bash
./banking
```

1. Избери **2** (List accounts) — намери ID на разплащателна сметка, примерно `CHK1`
2. Избери **18** → въведи `CHK1` → въведи `500` → трябва: `Overdraft limit updated to 500`
3. Избери **2** → в дисплея на `CHK1` трябва да пише `Overdraft limit: 500.00`
4. Опитай **18** с ID на спестовна сметка → трябва: `Account is not a checking account.`

---

## Стъпка 7: Git commit

```bash
git add src/CheckingAccount.h src/Bank.h src/main.cpp
git commit -m "Add setOverdraftLimit for CheckingAccount with menu option 18"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-15.md](commit-15.md) — експорт на история на транзакции в текстов файл.
