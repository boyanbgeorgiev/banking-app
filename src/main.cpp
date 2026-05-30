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
                  << "11. Lock account\n"
                  << "12. Unlock account\n"
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
            case 11: menuLockAccount(); break;
            case 12: menuUnlockAccount(); break;
            case 0:  std::cout << "Bye.\n"; return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
