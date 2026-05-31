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

void menuTopClients() {
    bank.printTopClients();
}

void menuBankStats() {
    bank.printStats();
}

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

void menuChangeOverdraftLimit() {
    std::string accId;
    double limit;
    std::cout << "Checking account ID: "; std::cin >> accId;
    std::cout << "New overdraft limit: "; std::cin >> limit;
    if (!bank.setAccountOverdraftLimit(accId, limit))
        std::cout << "Failed to update overdraft limit.\n";
}

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

void menuChangeInterestRate() {
    std::string accId;
    double rate;
    std::cout << "Savings account ID: "; std::cin >> accId;
    std::cout << "New interest rate (e.g. 0.05 for 5%): "; std::cin >> rate;
    if (!bank.setAccountInterestRate(accId, rate))
        std::cout << "Failed to update interest rate.\n";
}

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

void menuDeleteClient() {
    std::string clientId;
    std::cout << "Client ID to delete: "; std::cin >> clientId;
    if (!bank.removeClient(clientId))
        std::cout << "Client not found.\n";
}

void menuFindClientByName() {
    std::string name;
    std::cout << "Client name (or part of name): "; std::cin >> name;
    Client* c = bank.findClientByName(name);
    if (!c) { std::cout << "No client found with name containing \"" << name << "\".\n"; return; }
    c->displayReport();
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
                  << "13. Find client by name\n"
                  << "14. Delete client\n"
                  << "15. Transaction history (filtered)\n"
                  << "16. Change interest rate (savings)\n"
                  << "17. Edit client info\n"
                  << "18. Change overdraft limit (checking)\n"
                  << "19. Export history to file\n"
                  << "20. Bank statistics\n"
                  << "21. Top clients by net worth\n"
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
            case 13: menuFindClientByName(); break;
            case 14: menuDeleteClient(); break;
            case 15: menuFilteredHistory(); break;
            case 16: menuChangeInterestRate(); break;
            case 17: menuEditClient(); break;
            case 18: menuChangeOverdraftLimit(); break;
            case 19: menuExportHistory(); break;
            case 20: menuBankStats(); break;
            case 21: menuTopClients(); break;
            case 0:  std::cout << "Bye.\n"; return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
