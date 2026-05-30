# Commit 17 — Top Clients Report (Ranked by Net Worth)

## Какво правиш в тази стъпка

Добавяш нова функционалност **21. Top clients report** — извежда всички клиенти, сортирани по нетна стойност (от най-богат към най-беден).
Демонстрира сортиране с `std::sort` и lambda функции.

**Изисква:** Commit 16 да е направен.

---

## Стъпка 1: Добави printTopClients в src/Bank.h

В класа `Bank`, след `printStats()`, добави:

```cpp
void printTopClients() const {
    if (clients.empty()) { std::cout << "No clients.\n"; return; }

    std::vector<Client*> sorted;
    for (const auto& c : clients) sorted.push_back(c.get());

    std::sort(sorted.begin(), sorted.end(),
        [](Client* a, Client* b){ return a->getNetWorth() > b->getNetWorth(); });

    std::cout << "\n=== Clients Ranked by Net Worth ===\n";
    int rank = 1;
    for (Client* c : sorted) {
        std::cout << "#" << rank++ << " ";
        c->display();
    }
}
```

---

## Стъпка 2: Добави menu функцията в src/main.cpp

Добави след `menuBankStats()`:

```cpp
void menuTopClients() {
    bank.printTopClients();
}
```

---

## Стъпка 3: Обнови менюто и switch в main()

В менюто добави след опция 20:
```
<< "21. Top clients by net worth\n"
```

В switch добави:
```cpp
case 21: menuTopClients(); break;
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

1. Избери **21** → трябва да видиш клиентите наредени: Alice (1500.00) преди Bob (750.00)
2. Депозирай голяма сума в сметка на Bob → избери **21** отново → редът трябва да се е сменил

---

## Стъпка 6: Git commit

```bash
git add src/Bank.h src/main.cpp
git commit -m "Add top clients report ranked by net worth (menu option 21)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш **15+ реда** (не броейки дублиращите начални commits).

```bash
make && ./banking
```

Програмата трябва да стартира без грешки и да показва менюто с опции 1–21.

---

## Готово!

Всички 10 нови функционалности са имплементирани. Проектът има:
- ✅ Минимум 15 реални commits
- ✅ Минимум 8 функционалности (всъщност 13+)
- ✅ Работещо меню с 21 опции
- ✅ Класова йерархия: BankEntity → Account → CheckingAccount / SavingsAccount
