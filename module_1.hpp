#ifndef MODULE1_USERMANAGEMENT_H
#define MODULE1_USERMANAGEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> // For input validation

using namespace std;

// --- Forward Declarations ---
// We declare these here so Student knows they exist. 
// They will be defined fully in Module 3.
struct TransactionNode;
class BillingSystem;

// --- Constants ---
const string DB_FILE = "students.txt";
const char DELIMITER = '|'; // Safer than comma
const int MAX_ROOM_CAPACITY = 2;
const int REG_NO_LENGTH = 7;

// --- Helper Functions ---
// Clears input buffer to prevent skipping inputs
inline void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// --- Student Class ---
class Student {
private:
    string name;
    string regNo;   // The unique ID
    string roomNo;
    string password;

    // Pointer to the start of the linked list (Billing Data)
    // We keep the data here, but Logic is in Module 3.
    TransactionNode* head;

    // Grant BillingSystem access to 'head'
    friend class BillingSystem;

public:
    // Constructors
    Student();
    Student(string n, string reg, string room, string pass);
    
    // Destructor (Cleans up linked list memory)
    ~Student();

    // Getters
    string getName() const;
    string getRegNo() const;
    string getRoomNo() const;
    string getPassword() const;

    // Setters
    void setPassword(string newPass);
    void setRoomNo(string newRoom); // Only if needed later

    // Display
    void displayStudent() const;
};

// --- Admin Class ---
class Admin {
private:
    // The main database: Key = RegNo, Value = Student Object
    unordered_map<string, Student> studentDB;

    // Hardcoded Credentials
    const string ADMIN_USER = "admin";
    const string ADMIN_PASS = "admin123";

    // Internal Helper Functions
    bool isRegNoValid(const string& regNo);
    bool isRoomAvailable(const string& roomNo);
    string getSafeString(const string& prompt); // Prevents using delimiter '|'

public:
    Admin();
    
    // Authentication
    bool login();

    // Core Management
    void addStudent();
    void deleteStudent();
    void searchStudent();
    void displayAllStudents();
    void changeStudentPassword(); // Extra utility

    // File Handling
    void saveToFile();
    void loadFromFile();

    // Accessor for other modules
    unordered_map<string, Student>& getStudentDB();
};

#endif // MODULE1_USERMANAGEMENT_H