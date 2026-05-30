# Commit 8 — Account Lock/Unlock Menu Options

## Какво правиш в тази стъпка

Методите `lock()` и `unlock()` вече съществуват в `Account`, но **не са достъпни от менюто**.
Добавяш две нови опции в `main.cpp`:
- **11. Lock account** — заключва сметка (транзакции забранени)
- **12. Unlock account** — отключва сметка

**Изисква:** Проектът да компилира (commits 3–7 направени).

---

## Стъпка 1: Добави двете menu функции в src/main.cpp

Намери блока с другите menu функции (преди `int main()`) и добави след `menuCreateAccount()`:

```cpp
void menuLockAccount() {
    std::string accId;
    std::cout << "Account ID to lock: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    acc->lock();
}

void menuUnlockAccount() {
    std::string accId;
    std::cout << "Account ID to unlock: "; std::cin >> accId;
    Account* acc = bank.findAccount(accId);
    if (!acc) { std::cout << "Account not found.\n"; return; }
    acc->unlock();
}
```

---

## Стъпка 2: Обнови менюто в main()

Намери блока с `std::cout << "\n--- Menu ---\n"` и добави двете нови опции след `"10. Apply interest (savings)\n"`:

```
<< "11. Lock account\n"
<< "12. Unlock account\n"
```

---

## Стъпка 3: Добави case-овете в switch

В `switch (choice)` добави след `case 10`:

```cpp
case 11: menuLockAccount(); break;
case 12: menuUnlockAccount(); break;
```

---

## Стъпка 4: Валидация — компилация

```bash
make
```

Очакван резултат: без грешки, файлът `banking` се създава.

---

## Стъпка 5: Функционален тест

```bash
./banking
```

1. Избери **2** (List all accounts) — запомни някое ID, примерно `CHK1`
2. Избери **11** → въведи `CHK1` → трябва: `Account CHK1 locked.`
3. Избери **5** (Deposit) → въведи `CHK1` и сума → трябва: `Error: Account is locked.`
4. Избери **12** → въведи `CHK1` → трябва: `Account CHK1 unlocked.`
5. Избери **5** → депозирай отново → трябва да мине

---

## Стъпка 6: Git commit

```bash
git add src/main.cpp
git commit -m "Add lock/unlock account menu options (11 and 12)"
```

---

## Финална проверка

```bash
git log --oneline
```

Трябва да видиш новия commit най-отгоре.

---

## Следваща стъпка

Отиди на [commit-09.md](commit-09.md) — търсене на клиент по име.
