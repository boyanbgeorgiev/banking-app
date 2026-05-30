# Commit 13 — Edit Client Name and Email

## Какво правиш в тази стъпка

Добавяш нова функционалност **17. Edit client info** — позволява промяна на името и/или имейла на съществуващ клиент.
Изисква добавяне на сетъри в `Client` и нова menu опция.

**Изисква:** Commit 12 да е направен.

---

## Стъпка 1: Добави сетъри в src/Client.h

В класа `Client`, след метода `getEmail()`, добави:

```cpp
void setName(const std::string& newName) {
    if (newName.empty()) throw std::invalid_argument("Name cannot be empty.");
    name = newName;
    std::cout << "Name updated to \"" << newName << "\"\n";
}

void setEmail(const std::string& newEmail) {
    if (newEmail.empty()) throw std::invalid_argument("Email cannot be empty.");
    email = newEmail;
    std::cout << "Email updated to \"" << newEmail << "\"\n";
}
```

---

## Стъпка 2: Добави updateClient в Bank.h

В класа `Bank`, след `removeClient()`, добави:

```cpp
bool updateClientName(const std::string& clientId, const std::string& newName) {
    Client* c = findClient(clientId);
    if (!c) return false;
    try { c->setName(newName); return true; }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; return false; }
}

bool updateClientEmail(const std::string& clientId, const std::string& newEmail) {
    Client* c = findClient(clientId);
    if (!c) return false;
    try { c->setEmail(newEmail); return true; }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; return false; }
}
```

---

## Стъпка 3: Добави menu функцията в src/main.cpp

Добави след `menuChangeInterestRate()`:

```cpp
void menuEditClient() {
    std::string clientId;
    std::cout << "Client ID: "; std::cin >> clientId;
    Client* c = bank.findClient(clientId);
    if (!c) { std::cout << "Client not found.\n"; return; }

    std::cout << "What to edit?\n"
              << "  1. Name\n"
              << "  2. Email\n"
              << "Choice: ";
    int choice; std::cin >> choice;

    std::string value;
    if (choice == 1) {
        std::cout << "New name: "; std::cin >> value;
        bank.updateClientName(clientId, value);
    } else if (choice == 2) {
        std::cout << "New email: "; std::cin >> value;
        bank.updateClientEmail(clientId, value);
    } else {
        std::cout << "Invalid choice.\n";
    }
}
```

---

## Стъпка 4: Обнови менюто и switch в main()

В менюто добави след опция 16:
```
<< "17. Edit client info\n"
```

В switch добави:
```cpp
case 17: menuEditClient(); break;
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

1. Избери **1** (List clients) — запомни ID на Alice, примерно `CLI1`
2. Избери **17** → въведи `CLI1` → избери **1** (Name) → въведи `Alicia` → трябва: `Name updated to "Alicia"`
3. Избери **1** → трябва да видиш `Alicia` вместо `Alice`
4. Избери **17** → въведи `CLI1` → избери **2** (Email) → въведи `alicia@example.com`
5. Избери **9** (Client report) → провери актуализираните данни

---

## Стъпка 7: Git commit

```bash
git add src/Client.h src/Bank.h src/main.cpp
git commit -m "Add edit client name and email functionality (menu option 17)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-14.md](commit-14.md) — промяна на овърдрафт лимита на разплащателна сметка.
