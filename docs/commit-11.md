# Commit 11 — Filtered Transaction History

## Какво правиш в тази стъпка

Методът `printHistory()` вече поддържа филтриране по тип (`TxType* filterType`), но менюто винаги показва всичко.
Добавяш нова опция **15. Transaction history (filtered)** — потребителят избира какъв тип транзакции да види.

**Изисква:** Commit 10 да е направен.

---

## Стъпка 1: Добави menu функцията в src/main.cpp

Добави след `menuDeleteClient()`:

```cpp
void menuFilteredHistory() {
    std::string accId;
    std::cout << "Account ID: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }

    std::cout << "Filter by type:\n"
              << "  1. DEPOSIT\n"
              << "  2. WITHDRAWAL\n"
              << "  3. TRANSFER_IN\n"
              << "  4. TRANSFER_OUT\n"
              << "  0. All (no filter)\n"
              << "Choice: ";
    int f; std::cin >> f;

    if (f == 0) {
        acc->printHistory();
        return;
    }

    TxType filter;
    switch (f) {
        case 1: filter = TxType::DEPOSIT;       break;
        case 2: filter = TxType::WITHDRAWAL;    break;
        case 3: filter = TxType::TRANSFER_IN;   break;
        case 4: filter = TxType::TRANSFER_OUT;  break;
        default: std::cout << "Invalid filter.\n"; return;
    }
    acc->printHistory(&filter);
}
```

---

## Стъпка 2: Обнови менюто и switch в main()

В менюто добави след опция 14:
```
<< "15. Transaction history (filtered)\n"
```

В switch добави:
```cpp
case 15: menuFilteredHistory(); break;
```

---

## Стъпка 3: Валидация — компилация

```bash
make
```

Без грешки.

---

## Стъпка 4: Функционален тест

```bash
./banking
```

1. Избери **5** (Deposit) → депозирай в `CHK1` сума 100
2. Избери **7** (Transfer) → прехвърли от `CHK1` към `SAV2` сума 50
3. Избери **15** → въведи `CHK1` → избери **1** (DEPOSIT) → трябва да видиш само депозита
4. Избери **15** → въведи `CHK1` → избери **4** (TRANSFER_OUT) → трябва да видиш само трансфера
5. Избери **15** → въведи `CHK1` → избери **0** (All) → трябва да видиш всичко

---

## Стъпка 5: Git commit

```bash
git add src/main.cpp
git commit -m "Add filtered transaction history by type (menu option 15)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-12.md](commit-12.md) — смяна на лихвен процент на спестовна сметка.
