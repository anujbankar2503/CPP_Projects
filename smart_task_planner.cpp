// | Stream         | Derived Class (File) | Used For                 |
// | -------------- | -------------------- | ------------------------ |
// | `std::istream` | `std::ifstream`      | Reading from files       |
// | `std::ostream` | `std::ofstream`      | Writing to files         |
// | both           | `std::fstream`       | Reading and writing both |

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>

void saveTasks();

struct Task
{
    int id;
    std::string name;
    int duration;         // in minutes
    std::string deadline; // format: YYYY-MM-DD
    bool isDone;

    // Prioritization score based on urgency and duration

    double priorityScore()
    {
        int daysLeft = getDaysUntilDeadline(deadline); // calculate how many days left from deadline
    }

    // Helper to calculate days left till deadline

    static int getDaysUntilDeadline(const std::string &deadline)
    {
        std::tm deadline_tm = {};
        // Parse date
        sscanf(deadline.c_str(), "%d-%d-%d", &deadline_tm.tm_year, &deadline_tm.tm_mon, &deadline_tm.tm_mday);

        // UNCOMMENT this to fix it
        deadline_tm.tm_year -= 1900; // REQUIRED
        deadline_tm.tm_mon -= 1;     // REQUIRED

        std::time_t deadline_time = std::mktime(&deadline_tm);

        // ✅ What is std::mktime()?
        // std::mktime() is a function from the C++ <ctime> library (originally from C), and its main purpose is:
        // 🔁 Convert a tm (calendar time structure) to time_t (Unix timestamp)

        // Year: 2025
        // Month: 07
        // Day: 27
        // Hour: 0
        // Minute: 0
        // Second: 0
        // And converts it into a single number (time_t) that represents:
        // 🕒 Seconds since Jan 1, 1970 (Unix Epoch)

        std::time_t now = std::time(nullptr);

        // Gets the current system time in seconds since epoch (1970-01-01).(for above)
        // nullptr means “use current system time.

        return (deadline_time - now) / (60 * 60 * 24);

        // Subtracts the two times → difference in seconds.
        // Converts:
        // Seconds → Minutes → Hours → Days
        // Returns how many days remain until the deadline.
    }
};

// | Code Version      | tm\_year Value | Resulting Year    | Output (Readable)        |
// | ----------------- | -------------- | ----------------- | ------------------------ |
// | ❌ Without `-1900` | 2025          | 3925              | Sunday, July 27, 3925 😵|
// | ✅ With `-1900`    | 125           | 1900 + 125 = 2025 | Sunday, July 27, 2025 😊|

std::vector<Task> tasks;
int taskCounter = 1;

void loadTask()
{
    std::ifstream file("tasks.txt");
    std::string line;
    tasks.clear();
    while (std::getline(file, line))
    {
        Task t;
        char delim;
        std::istringstream ss(line);
        // For istringstream: Converts the line string into a stream so that we can extract formatted data.
        // istringstream works like cin but on a string.
        // Comes from: #include <sstream>
        ss >> t.id >> delim;
        std::getline(ss, t.name, ',');
        // std::getline(ss, t.name, ',');
        // Reads the task name until it sees a comma.
        // Supports names with spaces.
        ss >> t.duration >> delim;
        // We consume the comma immediately after the number:
        // ss >> t.duration >> delim;
        // t.duration = 20
        // delim = ',' (just thrown away, not used)
        // This cleans up the stream for the next part: reading the deadline using getline.

        std::getline(ss, t.deadline, ',');
        // getline(): This reads up to the comma, and removes the comma from the stream. So the comma does NOT remain in the stream.

        ss >> t.isDone;
        tasks.push_back(t);
        taskCounter = std::max(taskCounter, t.id + 1);
    }
};

// ==========================
// Menu
// ==========================

void showMenu()
{
    std::cout << "\n================================\n";
    std::cout << "       Smart Task Planner       \n";
    std::cout << "================================\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. View Tasks\n";
    std::cout << "3. Suggest Task\n";
    std::cout << "4. Mark Task as Done\n";
    std::cout << "0. Exit\n";
    std::cout << "================================\n";
}

void addTask()
{
    Task t;
    t.id = taskCounter++;
    std::cin.ignore();
    // cin.ignore(): This discards any leftover newline characters in the input buffer (especially after previous std::cin >>).
    // Without this, the next getline() would read an empty string.
    std::cout << "Enter task name: ";
    std::getline(std::cin, t.name);
    // std::getline(): allows reading spaces in the name (e.g. "Buy groceries").
    std::cout << "Enter duration in minutes: ";
    std::cin >> t.duration;
    std::cin.ignore();
    std::cout << "Enter deadline (YYYY-MM-DD): ";
    std::getline(std::cin, t.deadline);
    t.isDone = false; // Marks this task as not completed when initially added.

    tasks.push_back(t);
    std::cout << "Task added successfully!\n";
    saveTasks();
}

// ==========================
// File Handling
// ==========================

void saveTasks()
{
    std::ofstream file("tasks.txt");

    // std::ofstream: Creates an output file stream object named file using std::ofstream (from <fstream> header).
    // If tasks.txt doesn't exist, it creates one.
    // If it does exist, it overwrites it (default mode).
    // This line opens the file for writing.

    for (const Task &t : tasks)
    {
        file << t.id << "," << t.name << "," << t.deadline << "," << t.isDone << "\n";
    }
}

void viewTasks()
{
    std::sort(tasks.begin(), tasks.end(), [](Task a, Task b)
              { return a.priorityScore() < b.priorityScore(); });

    //  What it does:
    // Sorts the tasks vector in-place, from highest priority to lowest.
    // It uses a lambda function ([](Task a, Task b) { ... }) to compare two tasks.
    // Tasks are compared based on their priorityScore() value (lower score = higher priority).
    // 📚 Standard Library Features:
    // std::sort → from <algorithm>
    // Lambda expressions → from C++11
    // tasks.begin() and tasks.end() → standard vector iterators from <vector>
    // 🧠 Example:
    // If you have tasks like:
    // Eat (score 3.2), Study (score 2.1), Read (score 5.0)
    // After sorting: Study → Eat → Read.

    std::cout << "\n-- Task List --\n";
    for (const Task &t : tasks)
    {
        std::cout << "ID: " << t.id
                  << " | Name: " << t.name
                  << " | Duration: " << t.duration << " mins"
                  << " | Deadline: : " << t.deadline
                  << " | Status: " << (t.isDone ? "Done" : "Pending")
                  << "\n";
    }
}

void suggestTask()
{
    // We're creating an iterator itr that will point to the task with the minimum priority score (i.e., highest urgency).
    // std::min_element is used to find the minimum element in the tasks vector based on a comparison.
    std::vector<Task>::iterator itr;
    itr = std::min_element(tasks.begin(), tasks.end(), [](Task &a, Task &b)
                           {
//  1. What does std::min_element expect?
// The std::min_element(begin, end, comp) algorithm returns the smallest element in the given range, based on the comparator you provide.
// The comparator should behave like:
// bool comp(const T& a, const T& b)
// It should return:
// true → if a should come before b (i.e., a is "less than" b)
// false → if a should come after b
//Think of std::min_element as:
//"Tell me if a is better than b. If yes, return true, I’ll keep a. Otherwise, I’ll discard it."
/*             
| Return value | Meaning                         | Result           |
| ------------ | ------------------------------- | ---------------- |
| `true`       | "`a` is better (less) than `b`" | Keep `a`         |
| `false`      | "`b` is better or equal"        | Keep `b` instead |*/

        if (a.isDone) return false; //returns b if true
        if(b.isDone) return true; //returns a if true
        return a.priorityScore()<b.priorityScore(); }); // returns a if a<b

    if (itr != tasks.end() && itr->isDone == 0)
    {
        std::cout << "\n>> Suggested task: " << itr->name << " (" << itr->duration << " mins) - Deadline: " << itr->deadline << "\n";
    }
    else
    {
        std::cout << "\n All tasks are complete or no tasks available!\n";
    }
}

void markDone()
{
    int id;
    std::cout << "Enter Task ID to mark as done: ";
    std::cin >> id;
    for (Task &t : tasks)
    {
        if (t.id == id)
        {
            t.isDone = true;
            std::cout << "Task marked as completed!\n";
            saveTasks();
            return;
        }
    }
    std::cout << "Task ID not found!\n";
}

// ==========================
// Main Function
// ==========================

int main()
{
    loadTask();
    int choice;
    do
    {
        showMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            addTask();
            break;
        case 2:
            viewTasks();
            break;
        case 3:
            suggestTask();
            break;
        case 4:
            markDone();
            break;
        case 0:
            std::cout << "Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);
    return 0;
}