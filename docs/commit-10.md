# Commit 10 — Delete Client

## Какво правиш в тази стъпка

Добавяш нова функционалност **14. Delete client** — премахва клиент и всичките му сметки от банката.
Това демонстрира управление на паметта с `unique_ptr` и изтриване от вектор.

**Изисква:** Commit 9 да е направен.

---

## Стъпка 1: Добави removeClient в src/Bank.h

В класа `Bank`, след метода `listClients()`, добави:

```cpp
bool removeClient(const std::string& clientId) {
    auto it = std::find_if(clients.begin(), clients.end(),
        [&](const std::unique_ptr<Client>& c){ return c->getId() == clientId; });
    if (it == clients.end()) return false;

    // Remove all accounts belonging to this client
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(),
            [&](const std::unique_ptr<Account>& a){ return a->getOwnerId() == clientId; }),
        accounts.end());

    clients.erase(it);
    std::cout << "Client " << clientId << " and all their accounts removed.\n";
    return true;
}
```

Увери се, че `#include <algorithm>` е включен в `Bank.h` (добави го ако липсва).

---

## Стъпка 2: Добави menu функцията в src/main.cpp

Добави след `menuFindClientByName()`:

```cpp
void menuDeleteClient() {
    std::string clientId;
    std::cout << "Client ID to delete: "; std::cin >> clientId;
    if (!bank.removeClient(clientId))
        std::cout << "Client not found.\n";
}
```

---

## Стъпка 3: Обнови менюто и switch в main()

В менюто добави след опция 13:
```
<< "14. Delete client\n"
```

В switch добави:
```cpp
case 14: menuDeleteClient(); break;
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

1. Избери **1** (List clients) — запомни ID на Bob, примерно `CLI2`
2. Избери **14** → въведи `CLI2` → трябва: `Client CLI2 and all their accounts removed.`
3. Избери **1** → Bob вече не трябва да се вижда
4. Избери **2** → сметките на Bob вече не трябва да се виждат
5. Опитай **14** с несъществуващо ID → трябва: `Client not found.`

---

## Стъпка 6: Git commit

```bash
git add src/Bank.h src/main.cpp
git commit -m "Add delete client functionality with account cleanup (menu option 14)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-11.md](commit-11.md) — филтрирана история на транзакциите.
