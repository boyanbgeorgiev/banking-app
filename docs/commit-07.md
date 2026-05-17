# Commit 7 — README и финална документация

## Какво правиш в тази стъпка

Обновяваш `README.md` с пълно описание на проекта — функционалности, йерархия, как се компилира, и задължителната бележка за промени спрямо Checkpoint 1.

**Изисква:** Commits 3–6 да са направени и `./banking` да работи.

---

## Стъпка 1: Замени съдържанието на README.md

Отвори `README.md` (в root папката) и замени **цялото му съдържание** с:

```markdown
# Banking System — OOP Project (C++)

Банкова система, имплементирана в C++17 като проект по Обектно-ориентирано програмиране.

## Класова йерархия

```
BankEntity  (abstract)
├── Client
└── Account  (abstract)
    ├── CheckingAccount
    └── SavingsAccount

Transaction  (самостоятелен клас)
TxType       (enum: DEPOSIT | WITHDRAWAL | TRANSFER_IN | TRANSFER_OUT)
```

### Ключови дизайн решения

- `Account` е абстрактен — съдържа обща логика, но не може да се инстанцира директно
- `balance` е `protected` — достъпен за подкласове, скрит от external код
- `applyInterest()` е `virtual` (не pure virtual) — `CheckingAccount` не го override-ва

## Имплементирани функционалности

| # | Функционалност | Статус |
|---|----------------|--------|
| 1 | Управление на сметки (CRUD) | ✅ |
| 2 | Депозит и теглене с валидация | ✅ |
| 3 | Превод между сметки | ✅ |
| 4 | История на транзакциите | ✅ |
| 5 | Справка за баланс | ✅ |
| 6 | Начисляване на лихва (спестовни) | ✅ |
| 7 | Управление на клиенти | ✅ |
| 8 | Обобщен финансов отчет | ✅ |

## Компилиране и стартиране

Изисква `g++` с C++17 и `make`.

```bash
make
./banking
```

За почистване:
```bash
make clean
```

## Файлова структура

```
src/
├── BankEntity.h         # абстрактен базов клас
├── Transaction.h        # транзакция + TxType enum
├── Account.h / .cpp     # абстрактен клас Account
├── CheckingAccount.h / .cpp
├── SavingsAccount.h / .cpp
├── Client.h / .cpp
├── Bank.h / .cpp
└── main.cpp             # меню и демо данни
Makefile
```

## Промени спрямо Checkpoint 1

Дизайнът следва плана от Checkpoint 1 без съществени промени.
Единствената адаптация: имплементацията на методите е inline в header файловете
(вместо отделни .cpp с дефиниции) поради малкия размер на проекта —
функционално и архитектурно еквивалентно.
```

---

## Стъпка 2: Валидация — провери README

```bash
grep "Функционалности" README.md
```
Трябва да намери реда с таблицата.

```bash
grep "Промени спрямо Checkpoint 1" README.md
```
Трябва да намери секцията за промени.

```bash
wc -l README.md
```
Трябва да е поне 50 реда.

---

## Стъпка 3: Финален системен тест — всичко заедно

Компилирай от нулата и тествай:

```bash
make clean && make
```

Очакван резултат: `BUILD OK` без warnings.

```bash
echo "1
2
0" | ./banking
```

Това праща автоматично: избор 1 (клиенти), избор 2 (сметки), избор 0 (изход).

**Очакван изход:**
```
=== Banking System ===
...
=== All Clients ===
Client [CLI1] Alice ...
Client [CLI2] Bob   ...

=== All Accounts ===
CheckingAccount [CHK1] ...
SavingsAccount  [SAV2] ...
CheckingAccount [CHK3] ...

Bye.
```

---

## Стъпка 4: Финална проверка на git history

```bash
git log --oneline
```

Трябва да видиш **поне 5 реда** (идеално 7):
```
xxxxxxx Update README with project description and implemented features
xxxxxxx Add main menu, Makefile, and demo data — project compiles and runs
xxxxxxx Add Client and Bank classes with account management
xxxxxxx Implement Account hierarchy: CheckingAccount and SavingsAccount
xxxxxxx Add base class BankEntity and Transaction with TxType enum
xxxxxxx 1st commmit
xxxxxxx first commit
```

---

## Стъпка 5: Git commit

```bash
git add README.md
git commit -m "Update README with project description and implemented features"
```

---

## Стъпка 6: Вземи commit hash за предаване

```bash
git log --oneline -1
```

Изходът ще е нещо като:
```
a1b2c3d Update README with project description and implemented features
```

**`a1b2c3d` е твоят commit hash — това предаваш на преподавателя.**

За пълния hash (ако се иска):
```bash
git rev-parse HEAD
```

---

## Финален чеклист преди предаване

```bash
# 1. Минимум 5 commits
git log --oneline | wc -l   # трябва >= 5

# 2. Проектът компилира
make clean && make           # без errors

# 3. Проектът стартира
echo "0" | ./banking         # трябва да покаже менюто и да излезе с "Bye."

# 4. Поне 4 функционалности работят (тествани в commit-06.md)

# 5. Класовата йерархия е имплементирана
grep "class BankEntity"      src/BankEntity.h
grep "class Account"         src/Account.h
grep "class CheckingAccount" src/CheckingAccount.h
grep "class SavingsAccount"  src/SavingsAccount.h
grep "class Client"          src/Client.h
```

Всичко зелено — готов си за предаване.
