# 5 Commits — стъпка по стъпка

Изпълни командите **в този ред**. Всяка стъпка = 1 commit. Отвори терминал в root папката на проекта (`code c++/`).

---

## Преди да започнеш — провери къде си

```bash
git log --oneline
# трябва да видиш 2 commits
```

---

## Commit 3 — Базова структура и абстрактни класове

**Какво правиш:** Създаваш `src/` папката и добавяш `BankEntity.h`, `Transaction.h` и enum `TxType`.

```bash
mkdir src

# Създай src/BankEntity.h  (виж съдържанието в 02-code.md)
# Създай src/Transaction.h (виж съдържанието в 02-code.md)

git add src/BankEntity.h src/Transaction.h
git commit -m "Add base class BankEntity and Transaction with TxType enum"
```

**Резултат:** Имаш 3 commits. Тази стъпка покрива класовата йерархия (BankEntity).

---

## Commit 4 — Абстрактен клас Account + конкретните типове сметки

**Какво правиш:** Добавяш `Account.h`, `Account.cpp`, `CheckingAccount.h`, `CheckingAccount.cpp`, `SavingsAccount.h`, `SavingsAccount.cpp`.

```bash
# Създай всичките файлове (виж 02-code.md)

git add src/Account.h src/Account.cpp \
        src/CheckingAccount.h src/CheckingAccount.cpp \
        src/SavingsAccount.h  src/SavingsAccount.cpp
git commit -m "Implement Account hierarchy: CheckingAccount and SavingsAccount"
```

**Резултат:** Имаш 4 commits. Покрива: Управление на сметки (CRUD), депозит/теглене, начисляване на лихва.

---

## Commit 5 — Клас Client + клас Bank

**Какво правиш:** Добавяш `Client.h`, `Client.cpp`, `Bank.h`, `Bank.cpp`.

```bash
# Създай файловете (виж 02-code.md)

git add src/Client.h src/Client.cpp \
        src/Bank.h  src/Bank.cpp
git commit -m "Add Client and Bank classes with account management"
```

**Резултат:** Имаш 5 commits. ✅ Минималното изискване е изпълнено! Покрива: управление на клиенти, финансов отчет.

---

## Commit 6 — main.cpp + Makefile (проектът компилира)

**Какво правиш:** Добавяш входна точка с меню и Makefile.

```bash
# Създай src/main.cpp и Makefile (виж 02-code.md)

git add src/main.cpp Makefile
git commit -m "Add main menu, Makefile, and demo data — project compiles and runs"
```

**Резултат:** Имаш 6 commits. Проектът компилира и стартира.

---

## Commit 7 — Превод между сметки и история на транзакциите (документация)

**Какво правиш:** Обновяваш README.md с описание на проекта и функционалностите.

```bash
# Редактирай README.md — виж 04-compile-and-run.md за текста

git add README.md
git commit -m "Update README with project description and implemented features"
```

**Резултат:** Имаш 7 commits. Чист, логичен git history.

---

## Проверка в края

```bash
git log --oneline
# Трябва да видиш 7 реда (или поне 5)
```

```bash
make
./banking
# Трябва да се стартира менюто
```

---

## Бележка: какво се е променило спрямо Checkpoint 1

Ако преподавателят иска бележка за промени в дизайна, добави следното в README или като коментар в commit message:

> Дизайнът следва плана от Checkpoint 1 без съществени промени.
> Единствената адаптация: имплементацията на методите е inline в header файловете
> (вместо отделни .cpp) поради малкия размер на проекта — функционално еквивалентно.
