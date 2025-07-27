#include "iostream"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <map>

struct Expense
{
    int id;
    std::string category; // format: YYYY-MM-DD
    double amount;
    std::string date;
    std::string note;
};

std::vector<Expense> expenses;
int expenseCounter = 1;
const double DAILY_LIMIT = 500.0;

// Load expenses from file
void loadExpenses()
{
    std::ifstream file("expenses.txt");
    std::string line;
    expenses.clear();

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        Expense e;
        std::string amountStr;
        char delim;
        ss >> e.id >> delim;
        std::getline(ss, e.category, ',');
        std::getline(ss, amountStr, ',');
        e.amount = std::stod(amountStr); // d
        std::getline(ss, e.date, ',');
        std::getline(ss, e.note);
        expenses.push_back(e);
        expenseCounter = std::max(expenseCounter, e.id + 1); // d
    }
}

// Menu
void showMenu()
{
    std::cout << "\n===============================\n";
    std::cout << "     Smart Expense Tracker     \n";
    std::cout << "===============================\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View All Expenses\n";
    std::cout << "3. View Category Summary\n";
    std::cout << "4. Smart Spending Suggestions\n";
    std::cout << "0. Exit\n";
    std::cout << "===============================\n";
    std::cout << "Enter your choice: ";
}

std::string getTodayDate()
{
    time_t t = time(nullptr);
//time_t is a type from <ctime> representing the number of seconds since Unix epoch (Jan 1, 1970).
//time(nullptr) returns the current system time.
//So, t holds the raw current time.
    std::cout<<"time: "<<t<<std::endl;
    tm *now = localtime(&t);
//localtime(&t) converts the raw time t into local time (broken down into components like year, month, day, etc.).
//Returns a pointer to a tm struct.
/*struct tm {
  int tm_year;  // years since 1900
  int tm_mon;   // months since January [0–11]
  int tm_mday;  // day of the month [1–31]
  ...
};*/
    char buf[11];
//Declares a character buffer of size 11 to hold the formatted date string.
//Enough space for YY-MM-DD (8 chars + 1 null + safety buffer).
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", now);
/*Formats the date stored in the tm struct now and stores it in buf.
%y: last 2 digits of the year
%m: zero-padded month (01–12)
%d: zero-padded day (01–31)*/
    return std::string(buf);
// Converts the C-style string buf into a C++ std::string and returns it.
}

// Save expenses to file
void saveExpenses()
{
    std::ofstream file("expenses.txt");
    for (const auto &e : expenses)
    {
        file << e.id << "," << e.category << "," << e.amount << "," << e.date << "," << e.note << "\n";
    }
}

// Add a new expense
void addExpense()
{
    Expense e;
    e.id = expenseCounter++;
    std::cin.ignore();

    std::cout << "Enter category (Food,Travel,Bills,etc.): ";
    std::getline(std::cin, e.category);
    std::cout << "Enter amount: ";
    std::cin >> e.amount;
    std::cin.ignore();
    std::cout << "Enter note (optional): ";
    std::getline(std::cin, e.note);
    e.date = getTodayDate();

    expenses.push_back(e);
    saveExpenses();
    std::cout << "Expense added successfully!\n";
}

// View all expenses
void viewExpense()
{
    std::cout << "\n--- Expense List ---\n";
    for (const auto &e : expenses)
    {
        std::cout << "ID: " << e.id
                  << " | Category: " << e.category
                  << " | Amount: " << e.amount
                  << " | Date: " << e.date
                  << " | Note: " << e.note << "\n";
    }
}

// View category-wise summary
void categorySummary() {
    std::map<std::string,double> summary;
    for(auto& e: expenses){
        summary[e.category] += e.amount;
    }
     std::cout << "\n--- Category-wise Summary ---\n";
    for(const auto& pair: summary){
        std::cout<<pair.first<<": Rs"<<pair.second<<std::endl;
    } 
}

// Show spending suggestions
void showSuggestions() {
    double todaySpent = 0;
    std::map<std::string,double> categoryTotals;
    std::string today = getTodayDate();

    for(const auto& e: expenses){
        if(e.date == today){
            todaySpent += e.amount;
            categoryTotals[e.category] += e.amount;
        }
    }

    std::cout << "\n--- Smart Suggestions ---\n";

    if(todaySpent > DAILY_LIMIT){
        std::cout << "You've exceeded today's spending limit of Rs " << DAILY_LIMIT << " (Spent: Rs" << todaySpent << ") by"<<DAILY_LIMIT-todaySpent<<std::endl;;
    }else{
        std::cout << "You're within today's budget. (Spent: Rs" << todaySpent << ")\n";
    }
     
    for(const auto& pair: categoryTotals){
        if(pair.second>0.4*todaySpent){
            std::cout<<"You're spending a lot on"<<pair.first<<" (Rs"<<pair.second<<").Consider cutting back\n";
        }
    }
    if(expenses.empty())
    std::cout<<"No expenses added yet.\n";
}

int main()
{
    loadExpenses();
    int choice;
    do
    {
        showMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            addExpense();
            break;
        case 2:
            viewExpense();
            break;
        case 3:
            categorySummary();
            break;
        case 4:
            showSuggestions();
            break;
        case 0:
            std::cout << "Goodbye! \n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
