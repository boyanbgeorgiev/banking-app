# Commit 12 — Change Interest Rate on Savings Account

## Какво правиш в тази стъпка

Добавяш нова функционалност **16. Change interest rate** — позволява промяна на лихвения процент на спестовна сметка.
Изисква добавяне на `setInterestRate()` в `SavingsAccount` и нова menu опция.

**Изисква:** Commit 11 да е направен.

---

## Стъпка 1: Добави setInterestRate в src/SavingsAccount.h

В класа `SavingsAccount`, след метода `applyInterest()`, добави:

```cpp
void setInterestRate(double newRate) {
    if (newRate < 0) throw std::invalid_argument("Interest rate cannot be negative.");
    interestRate = newRate;
    std::cout << "Interest rate updated to " << (newRate * 100) << "%\n";
}

double getInterestRate() const { return interestRate; }
```

---

## Стъпка 2: Добави setInterestRate в Bank.h

В класа `Bank`, след `applyInterestAll()`, добави:

```cpp
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
```

Увери се, че `#include "SavingsAccount.h"` е включен в `Bank.h` (вече е).

---

## Стъпка 3: Добави menu функцията в src/main.cpp

Добави след `menuFilteredHistory()`:

```cpp
void menuChangeInterestRate() {
    std::string accId;
    double rate;
    std::cout << "Savings account ID: "; std::cin >> accId;
    std::cout << "New interest rate (e.g. 0.05 for 5%): "; std::cin >> rate;
    if (!bank.setAccountInterestRate(accId, rate))
        std::cout << "Failed to update interest rate.\n";
}
```

---

## Стъпка 4: Обнови менюто и switch в main()

В менюто добави след опция 15:
```
<< "16. Change interest rate (savings)\n"
```

В switch добави:
```cpp
case 16: menuChangeInterestRate(); break;
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

1. Избери **2** (List accounts) — намери ID на спестовна сметка, примерно `SAV2`
2. Избери **16** → въведи `SAV2` → въведи `0.05` → трябва: `Interest rate updated to 5%`
3. Избери **2** → в дисплея на `SAV2` трябва да пише `Rate: 5%`
4. Избери **10** (Apply interest) → лихвата трябва да се изчисли с 5%
5. Опитай **16** с ID на разплащателна сметка → трябва: `Account is not a savings account.`

---

## Стъпка 7: Git commit

```bash
git add src/SavingsAccount.h src/Bank.h src/main.cpp
git commit -m "Add setInterestRate for SavingsAccount with menu option 16"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-13.md](commit-13.md) — редактиране на данните на клиент.
