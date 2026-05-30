# Commit 9 — Search Client by Name

## Какво правиш в тази стъпка

Методът `findClientByName()` вече съществува в `Bank`, но **не е достъпен от менюто**.
Добавяш нова опция **13. Find client by name** — търси клиент по точно или частично съвпадение на името и извежда пълния му профил.

**Изисква:** Commit 8 да е направен.

---

## Стъпка 1: Подобри findClientByName в Bank.h

Сегашният метод търси само точно съвпадение. Замени го с версия, която търси частично (case-insensitive не е задължително — достатъчно е partial match):

Намери в `src/Bank.h`:
```cpp
Client* findClientByName(const std::string& name) const {
    for (auto& c : clients)
        if (c->getName() == name) return c.get();
    return nullptr;
}
```

Замени с:
```cpp
Client* findClientByName(const std::string& name) const {
    for (auto& c : clients)
        if (c->getName().find(name) != std::string::npos) return c.get();
    return nullptr;
}
```

---

## Стъпка 2: Добави menu функцията в src/main.cpp

Добави след `menuUnlockAccount()`:

```cpp
void menuFindClientByName() {
    std::string name;
    std::cout << "Client name (or part of name): "; std::cin >> name;
    Client* c = bank.findClientByName(name);
    if (!c) { std::cout << "No client found with name containing \"" << name << "\".\n"; return; }
    c->displayReport();
}
```

---

## Стъпка 3: Обнови менюто и switch в main()

В менюто добави след опция 12:
```
<< "13. Find client by name\n"
```

В switch добави:
```cpp
case 13: menuFindClientByName(); break;
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

1. Избери **13** → въведи `Ali` → трябва да изведе репорта на Alice с всичките й сметки
2. Избери **13** → въведи `xyz` → трябва: `No client found with name containing "xyz".`

---

## Стъпка 6: Git commit

```bash
git add src/Bank.h src/main.cpp
git commit -m "Add search client by name with partial match (menu option 13)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-10.md](commit-10.md) — изтриване на клиент.
