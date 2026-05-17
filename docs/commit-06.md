# Commit 6 — main.cpp + Makefile (проектът компилира и стартира)

## Какво правиш в тази стъпка

Добавяш:
- `src/main.cpp` — входна точка с интерактивно меню и демо данни
- `Makefile` — за лесно компилиране с `make`

**Изисква:** Commits 3, 4 и 5 да са направени.

---

## Стъпка 1: Създай src/main.cpp

```cpp
#include <iostream>
#include <string>
#include <limits>
#include "Bank.h"

Bank bank;

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void menuDeposit() {
    std::string accId;
    double amount;
    std::cout << "Account ID: "; std::cin >> accId;
    std::cout << "Amount: ";     std::cin >> amount;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    try { acc->deposit(amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuWithdraw() {
    std::string accId;
    double amount;
    std::cout << "Account ID: "; std::cin >> accId;
    std::cout << "Amount: ";     std::cin >> amount;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    try { acc->withdraw(amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuTransfer() {
    std::string fromId, toId;
    double amount;
    std::cout << "From account ID: "; std::cin >> fromId;
    std::cout << "To account ID: ";   std::cin >> toId;
    std::cout << "Amount: ";          std::cin >> amount;
    Account* from = bank.findAccount(fromId);
    Account* to   = bank.findAccount(toId);
    if (!from || !to) { std::cout << "Account not found.\n"; return; }
    try { from->transfer(*to, amount); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
}

void menuHistory() {
    std::string accId;
    std::cout << "Account ID: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    acc->printHistory();
}

void menuCreateClient() {
    std::string name, email;
    std::cout << "Name: ";  std::cin >> name;
    std::cout << "Email: "; std::cin >> email;
    Client* c = bank.addClient(name, email);
    std::cout << "Client created: " << c->getId() << "\n";
}

void menuCreateAccount() {
    std::string clientId, type;
    double balance;
    std::cout << "Client ID: ";       std::cin >> clientId;
    std::cout << "Type (chk/sav): "; std::cin >> type;
    std::cout << "Initial balance: "; std::cin >> balance;
    Client* c = bank.findClient(clientId);
    if (!c) { std::cout << "Client not found.\n"; return; }
    if (type == "chk") {
        double overdraft;
        std::cout << "Overdraft limit: "; std::cin >> overdraft;
        bank.createChecking(c, balance, overdraft);
    } else if (type == "sav") {
        bank.createSavings(c, balance);
    } else {
        std::cout << "Unknown type.\n";
    }
}

int main() {
    std::cout << "=== Banking System ===\n";

    // Demo data
    Client* alice = bank.addClient("Alice", "alice@example.com");
    Client* bob   = bank.addClient("Bob",   "bob@example.com");
    bank.createChecking(alice, 1000.0, 200.0);
    bank.createSavings(alice,  500.0);
    bank.createChecking(bob,   750.0);

    int choice = 0;
    while (true) {
        std::cout << "\n--- Menu ---\n"
                  << "1.  List all clients\n"
                  << "2.  List all accounts\n"
                  << "3.  Create client\n"
                  << "4.  Create account\n"
                  << "5.  Deposit\n"
                  << "6.  Withdraw\n"
                  << "7.  Transfer\n"
                  << "8.  Transaction history\n"
                  << "9.  Client report\n"
                  << "10. Apply interest (savings)\n"
                  << "0.  Exit\n"
                  << "Choice: ";
        std::cin >> choice;
        if (std::cin.fail()) { clearInput(); continue; }

        switch (choice) {
            case 1:  bank.listClients(); break;
            case 2:  bank.listAllAccounts(); break;
            case 3:  menuCreateClient(); break;
            case 4:  menuCreateAccount(); break;
            case 5:  menuDeposit(); break;
            case 6:  menuWithdraw(); break;
            case 7:  menuTransfer(); break;
            case 8:  menuHistory(); break;
            case 9: {
                std::string id;
                std::cout << "Client ID: "; std::cin >> id;
                Client* c = bank.findClient(id);
                if (c) c->displayReport();
                else   std::cout << "Not found.\n";
                break;
            }
            case 10: bank.applyInterestAll(); break;
            case 0:  std::cout << "Bye.\n"; return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
```

---

## Стъпка 2: Създай Makefile (в root папката, НЕ в src/)

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src
SRC = $(wildcard src/*.cpp)
TARGET = banking

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```

> **Важно:** Редът с `$(CXX)` трябва да започва с **TAB**, не с интервали. Ако редактираш в gedit/nano, внимавай.

---

## Стъпка 3: Валидация — провери файловете

```bash
ls src/
```

Трябва да видиш `main.cpp` сред останалите файлове.

```bash
ls Makefile
```

Трябва: `Makefile`

```bash
cat -A Makefile | grep CXX
```

Редът трябва да започва с `^I` (символ за TAB), не с интервали.

---

## Стъпка 4: Тест — компилиране

```bash
make
```

**Очакван изход (без грешки):**
```
g++ -std=c++17 -Wall -Wextra -I src -o banking src/Account.cpp src/Bank.cpp \
    src/CheckingAccount.cpp src/Client.cpp src/SavingsAccount.cpp src/main.cpp
```

Ако виждаш `error:` — провери дали всички файлове от commits 3, 4, 5 съществуват в `src/`.

---

## Стъпка 5: Тест — стартиране и проверка на менюто

```bash
./banking
```

**Очакван изход при стартиране:**
```
=== Banking System ===
Client [CLI1] created
Client [CLI2] created
Created CheckingAccount CHK1
Created SavingsAccount SAV2
Created CheckingAccount CHK3

--- Menu ---
1.  List all clients
...
Choice:
```

### Тест последователност (въведи ги ръчно):

| Вход | Очакван резултат |
|------|-----------------|
| `1` | Показва Alice и Bob с балансите им |
| `2` | Показва CHK1, SAV2, CHK3 |
| `5` → `CHK1` → `300` | `Deposited 300. New balance: 1300` |
| `6` → `CHK1` → `100` | `Withdrew 100. New balance: 1200` |
| `7` → `CHK1` → `SAV2` → `200` | `Transferred 200 to account SAV2` |
| `8` → `CHK1` | Показва 3 транзакции: DEPOSIT, TRANSFER\_OUT |
| `9` → `CLI1` | Финансов отчет на Alice, сортиран по баланс |
| `10` | Начислява лихва на SAV2 |
| `0` | `Bye.` |

---

## Стъпка 6: Тест — грешни входове (edge cases)

```bash
./banking
```

| Вход | Очакван резултат |
|------|-----------------|
| `5` → `CHK999` → `100` | `Account not found.` |
| `6` → `SAV2` → `9999` | `Error: Cannot go below minimum balance.` |
| `abc` (невалиден избор) | Менюто се показва отново без crash |

---

## Стъпка 7: Git commit

```bash
git add src/main.cpp Makefile
git commit -m "Add main menu, Makefile, and demo data — project compiles and runs"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш **6 реда**.

```bash
make clean && make && echo "BUILD OK"
```

Трябва да завърши с `BUILD OK`.

---

## Следваща стъпка

Отиди на [commit-07.md](commit-07.md) — финален README и затваряне на проекта.
