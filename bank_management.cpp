#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

class Account {
private:
    long long accountNo;
    string name;
    string type;
    double balance;

public:
    Account() : accountNo(0), balance(0) {}

    void createAccount() {
        cout << "\nEnter Account Number : ";
        cin >> accountNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Customer Name  : ";
        getline(cin, name);

        cout << "Account Type         : ";
        getline(cin, type);

        cout << "Initial Deposit      : Rs. ";
        cin >> balance;

        if (balance < 0) balance = 0;
    }

    void display() const {
        cout << left
             << setw(15) << accountNo
             << setw(24) << name.substr(0, 22)
             << setw(15) << type.substr(0, 13)
             << "Rs. " << fixed << setprecision(2) << balance << '\n';
    }

    long long getAccountNo() const {
        return accountNo;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance)
            return false;

        balance -= amount;
        return true;
    }

    void saveTransaction(const string& action, double amount) const {
        ofstream file("transactions.txt", ios::app);

        file << "A/C: " << accountNo
             << " | " << action
             << " | Rs. " << fixed << setprecision(2) << amount
             << " | Balance: Rs. " << balance << '\n';

        file.close();
    }
};

// ---------------- BANK OPERATIONS ----------------

bool findAccount(long long accNo, Account& result) {
    ifstream file("accounts.dat", ios::binary);

    while (file.read(reinterpret_cast<char*>(&result), sizeof(result))) {
        if (result.getAccountNo() == accNo) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void createAccount() {
    Account account;
    account.createAccount();

    ofstream file("accounts.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&account), sizeof(account));
    file.close();

    cout << "\n✓ Account created successfully.\n";
}

void showAccounts() {
    Account account;
    ifstream file("accounts.dat", ios::binary);

    cout << "\n================ ACCOUNT DIRECTORY ================\n";
    cout << left
         << setw(15) << "ACCOUNT NO"
         << setw(24) << "CUSTOMER"
         << setw(15) << "TYPE"
         << "BALANCE\n";
    cout << "----------------------------------------------------\n";

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&account), sizeof(account))) {
        account.display();
        found = true;
    }

    if (!found)
        cout << "No accounts available.\n";

    file.close();
}

void balanceInquiry() {
    long long accNo;
    Account account;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    if (findAccount(accNo, account)) {
        cout << "\nAccount Holder : " << "Available\n";
        cout << "Account Number : " << accNo << '\n';
        cout << "Current Balance: Rs. "
             << fixed << setprecision(2)
             << account.getBalance() << '\n';
    } else {
        cout << "\n✗ Account not found.\n";
    }
}

void updateBalance(long long accNo, double amount, bool depositMode) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account account;

    while (file.read(reinterpret_cast<char*>(&account), sizeof(account))) {

        if (account.getAccountNo() == accNo) {

            bool success;

            if (depositMode) {
                account.deposit(amount);
                success = true;
            } else {
                success = account.withdraw(amount);
            }

            if (!success) {
                cout << "\n✗ Insufficient balance or invalid amount.\n";
                file.close();
                return;
            }

            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&account), sizeof(account));
            file.close();

            account.saveTransaction(
                depositMode ? "DEPOSIT" : "WITHDRAWAL",
                amount
            );

            cout << "\n✓ Transaction successful.";
            cout << "\nNew Balance: Rs. "
                 << fixed << setprecision(2)
                 << account.getBalance() << '\n';

            return;
        }
    }

    file.close();
    cout << "\n✗ Account not found.\n";
}

void transaction(bool depositMode) {
    long long accNo;
    double amount;

    cout << "\nEnter Account Number: ";
    cin >> accNo;

    cout << "Enter Amount: Rs. ";
    cin >> amount;

    if (amount <= 0) {
        cout << "\n✗ Enter a valid amount.\n";
        return;
    }

    updateBalance(accNo, amount, depositMode);
}

void transactionHistory() {
    ifstream file("transactions.txt");

    cout << "\n================ TRANSACTION HISTORY ================\n";

    string line;
    bool found = false;

    while (getline(file, line)) {
        cout << line << '\n';
        found = true;
    }

    if (!found)
        cout << "No transactions available.\n";

    file.close();
}

// ---------------- MAIN MENU ----------------

int main() {
    int choice;

    do {
        cout << "\n\n";
        cout << "╔════════════════════════════════════════╗\n";
        cout << "║          SECURE BANKING SYSTEM         ║\n";
        cout << "╠════════════════════════════════════════╣\n";
        cout << "║  1. Create Account                     ║\n";
        cout << "║  2. View All Accounts                  ║\n";
        cout << "║  3. Deposit Money                      ║\n";
        cout << "║  4. Withdraw Money                     ║\n";
        cout << "║  5. Balance Inquiry                    ║\n";
        cout << "║  6. Transaction History                ║\n";
        cout << "║  0. Exit                               ║\n";
        cout << "╚════════════════════════════════════════╝\n";

        cout << "\nSelect Option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                showAccounts();
                break;

            case 3:
                transaction(true);
                break;

            case 4:
                transaction(false);
                break;

            case 5:
                balanceInquiry();
                break;

            case 6:
                transactionHistory();
                break;

            case 0:
                cout << "\n✓ Thank you for using Secure Banking System.\n";
                break;

            default:
                cout << "\n✗ Invalid option. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}