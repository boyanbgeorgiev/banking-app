# Файлова структура на проекта

След като свършиш всичко, структурата трябва да изглежда така:

```
code c++/
├── docs/                    ← тази папка (инструкции)
├── src/
│   ├── BankEntity.h         ← абстрактен базов клас
│   ├── Transaction.h        ← клас за транзакции + enum TxType
│   ├── Account.h            ← абстрактен клас Account
│   ├── Account.cpp
│   ├── CheckingAccount.h    ← разплащателна сметка
│   ├── CheckingAccount.cpp
│   ├── SavingsAccount.h     ← спестовна сметка
│   ├── SavingsAccount.cpp
│   ├── Client.h             ← клиент
│   ├── Client.cpp
│   ├── Bank.h               ← главен клас — управлява всичко
│   ├── Bank.cpp
│   └── main.cpp             ← входна точка + меню
├── README.md
└── Makefile
```

## Класова йерархия (от Checkpoint 1)

```
BankEntity  (abstract)
├── Client
└── Account  (abstract)
    ├── CheckingAccount
    └── SavingsAccount

Transaction  (самостоятелен клас)
TxType       (enum)
```

## Стъпки за създаване на папките

```bash
mkdir -p src
```

Всички `.h` и `.cpp` файлове влизат в `src/`.
