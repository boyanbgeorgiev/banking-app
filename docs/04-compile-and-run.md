# Компилиране и стартиране

## Изисквания

- `g++` с поддръжка на C++17
- `make`

Провери дали ги имаш:
```bash
g++ --version
make --version
```

Ако нямаш:
```bash
sudo apt install build-essential   # Ubuntu/Debian
sudo dnf install gcc-c++ make      # Fedora
```

---

## Компилиране

От root папката на проекта (там където е `Makefile`):

```bash
make
```

Трябва да видиш нещо като:
```
g++ -std=c++17 -Wall -Wextra -I src -o banking src/Account.cpp src/Bank.cpp ...
```

---

## Стартиране

```bash
./banking
```

Трябва да видиш:
```
=== Banking System ===
Created CheckingAccount CHK1
Created SavingsAccount SAV2
Created CheckingAccount CHK3

--- Menu ---
1. List all clients
2. List all accounts
...
Choice:
```

---

## Примерна сесия за тестване

Тези стъпки демонстрират **6 функционалности** пред преподавателя:

```
Choice: 1          ← управление на клиенти (листване)
Choice: 2          ← управление на сметки (листване)
Choice: 5          ← депозит
  Account ID: CHK1
  Amount: 500
Choice: 6          ← теглене
  Account ID: CHK1
  Amount: 200
Choice: 7          ← превод между сметки
  From: CHK1
  To: SAV2
  Amount: 100
Choice: 8          ← история на транзакциите
  Account ID: CHK1
Choice: 9          ← финансов отчет на клиент
  Client ID: CLI1
Choice: 10         ← начисляване на лихва
```

---

## Почистване

```bash
make clean
```

---

## README.md текст (копирай в README.md)

```markdown
# Banking System — OOP Project (C++)

Банкова система, имплементирана в C++17 като проект по ООП.

## Класова йерархия

- `BankEntity` (abstract) — базов клас с ID и timestamp
  - `Client` — клиент с множество сметки
  - `Account` (abstract) — обща логика за сметки
    - `CheckingAccount` — разплащателна сметка с овърдрафт
    - `SavingsAccount` — спестовна сметка с лихва
- `Transaction` — запис за транзакция
- `Bank` — управлява всички клиенти и сметки

## Имплементирани функционалности

1. Управление на сметки (CRUD) — създаване на разплащателни и спестовни сметки
2. Депозит и теглене — с валидация
3. Превод между сметки — атомарна операция
4. История на транзакциите — с тип, сума и timestamp
5. Справка за баланс — текущ баланс на сметка
6. Управление на клиенти — добавяне и листване
7. Обобщен финансов отчет — всички сметки, сортирани по баланс
8. Начисляване на лихва — за спестовни сметки

## Компилиране

\`\`\`bash
make
./banking
\`\`\`

## Бележка за промени спрямо Checkpoint 1

Дизайнът следва плана без съществени промени. Методите са имплементирани
inline в header файловете поради малкия размер на проекта.
```
