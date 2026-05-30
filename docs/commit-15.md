# Commit 15 — Export Transaction History to File

## Какво правиш в тази стъпка

Добавяш нова функционалност **19. Export history to file** — записва историята на транзакциите на дадена сметка в текстов файл `history_<accId>.txt`.
Демонстрира file I/O с `<fstream>`.

**Изисква:** Commit 14 да е направен.

---

## Стъпка 1: Добави exportHistory в src/Account.h

В началото на `Account.h` добави към include-овете:
```cpp
#include <fstream>
```

В класа `Account`, след метода `printHistory()`, добави:

```cpp
bool exportHistory(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "Transaction history for account " << id << "\n";
    file << std::string(40, '-') << "\n";
    for (const auto& tx : history) {
        file << tx.typeToString()
             << " | Amount: " << std::fixed << std::setprecision(2) << tx.getAmount()
             << " | Account: " << tx.getAccountId() << "\n";
    }
    if (history.empty()) file << "No transactions.\n";
    return true;
}
```

---

## Стъпка 2: Добави menu функцията в src/main.cpp

Добави след `menuChangeOverdraftLimit()`:

```cpp
void menuExportHistory() {
    std::string accId;
    std::cout << "Account ID: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }

    std::string filename = "history_" + accId + ".txt";
    if (acc->exportHistory(filename))
        std::cout << "History exported to " << filename << "\n";
    else
        std::cout << "Failed to write file.\n";
}
```

---

## Стъпка 3: Обнови менюто и switch в main()

В менюто добави след опция 18:
```
<< "19. Export history to file\n"
```

В switch добави:
```cpp
case 19: menuExportHistory(); break;
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

1. Направи няколко транзакции по сметка `CHK1` (депозит, теглене)
2. Избери **19** → въведи `CHK1` → трябва: `History exported to history_CHK1.txt`
3. Излез от програмата (0) и провери файла:

```bash
cat history_CHK1.txt
```

Трябва да видиш списък с транзакциите в текстов формат.

---

## Стъпка 6: Git commit

```bash
git add src/Account.h src/main.cpp
git commit -m "Add export transaction history to text file (menu option 19)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-16.md](commit-16.md) — глобална статистика на банката.
