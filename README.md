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
