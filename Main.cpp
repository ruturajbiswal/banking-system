#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits> // For std::numeric_limits
#include <cstdlib> // For system("cls") or system("clear")
#include "json.hpp" // Include the JSON library
#include <sstream> // For std::ostringstream

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

using json = nlohmann::json;

// Helper function to format amounts in INR
std::string formatINR(double amount) {
    std::ostringstream oss;
    oss << "INR " << std::fixed << std::setprecision(2) << amount; // Using Unicode for the rupee symbol
    return oss.str();
}

class Account {
private:
    std::string accountNumber;
    double balance;
    std::vector<std::string> transactions;

    void saveTransaction(const std::string& transaction) {
        transactions.push_back(transaction);
        saveToFile();
    }

    void saveToFile() {
        json j;
        j["accountNumber"] = accountNumber;
        j["balance"] = balance;
        j["transactions"] = transactions;

        std::ofstream file("Accounts/" + accountNumber + ".json");
        if (file.is_open()) {
            file << std::setw(4) << j << std::endl;
            file.close();
        } else {
            std::cerr << "Unable to open file";
        }
    }

    void loadFromFile() {
        std::ifstream file("Accounts/" + accountNumber + ".json");
        if (file.is_open()) {
            json j;
            file >> j;
            file.close();

            accountNumber = j["accountNumber"];
            balance = j["balance"];
            transactions = j["transactions"].get<std::vector<std::string>>();
        }
    }

public:
    Account(const std::string& accNum) : accountNumber(accNum), balance(0.0) {
        loadFromFile();
    }

    void deposit(double amount) {
        balance += amount;
        std::string transaction = "Deposit: " + formatINR(amount) + " | Balance: " + formatINR(balance);
        saveTransaction(transaction);
        std::cout << transaction << std::endl;
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            std::string transaction = "Withdraw: " + formatINR(amount) + " | Balance: " + formatINR(balance);
            saveTransaction(transaction);
            std::cout << transaction << std::endl;
        } else {
            std::cout << "Insufficient balance." << std::endl;
        }
    }

    void showBalance() const {
        std::cout << "Current Balance: " << formatINR(balance) << std::endl;
    }

    void accountStatement() const {
        std::cout << "Account Statement for " << accountNumber << ":" << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << transaction << std::endl;
        }
    }
};

void displayMenu() {
    std::cout << "Banking System Menu:" << std::endl;
    std::cout << "1. Deposit" << std::endl;
    std::cout << "2. Withdraw" << std::endl;
    std::cout << "3. Show Balance" << std::endl;
    std::cout << "4. Account Statement" << std::endl;
    std::cout << "5. Switch Account" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

void holdScreen() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    // Ensure Accounts directory exists
    system("mkdir Accounts");

    std::string accountNumber;
    std::cout << "Enter account number: ";
    std::cin >> accountNumber;
    Account account(accountNumber);

    int choice;
    double amount;
    do {
        system(CLEAR_SCREEN);
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter amount to deposit: ";
                std::cin >> amount;
                account.deposit(amount);
                break;
            case 2:
                std::cout << "Enter amount to withdraw: ";
                std::cin >> amount;
                account.withdraw(amount);
                break;
            case 3:
                account.showBalance();
                break;
            case 4:
                account.accountStatement();
                break;
            case 5:
                std::cout << "Enter new account number: ";
                std::cin >> accountNumber;
                account = Account(accountNumber); // Reinitialize account with new account number
                break;
            case 6:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }

        if (choice != 6) {
            holdScreen();
        }
    } while (choice != 6);

    return 0;
}
