#include "module_1.hpp"
// We include Module 3 here because the Destructor needs to know what a TransactionNode is
#include "module_3.hpp" 
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio> // For remove()

using namespace std;

// ==========================================
//           Student Implementation
// ==========================================

Student::Student() {
    head = NULL;
}

Student::Student(string n, string reg, string room, string pass) 
    : name(n), regNo(reg), roomNo(room), password(pass) {
    head = NULL;
}

// DESTROYS the linked list to prevent memory leaks when student is removed
Student::~Student() {
    TransactionNode* current = head;
    while (current != NULL) {
        TransactionNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = NULL;
}

string Student::getName() const { return name; }
string Student::getRegNo() const { return regNo; }
string Student::getRoomNo() const { return roomNo; }
string Student::getPassword() const { return password; }

void Student::setPassword(string newPass) { password = newPass; }
void Student::setRoomNo(string newRoom) { roomNo = newRoom; }

void Student::displayStudent() const {
    cout << "---------------------------------" << endl;
    cout << "Name:     " << name << endl;
    cout << "Reg No:   " << regNo << endl;
    cout << "Room No:  " << roomNo << endl;
    cout << "---------------------------------" << endl;
}

// ==========================================
//            Admin Implementation
// ==========================================

Admin::Admin() {}

unordered_map<string, Student>& Admin::getStudentDB() {
    return studentDB;
}

// --- Helper: Get Safe String (Crash Prevention) ---
string Admin::getSafeString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Validation 1: Not Empty
        if (input.empty()) {
            cout << "Error: Input cannot be empty.\n";
            continue;
        }

        // Validation 2: No Delimiters (prevents file corruption)
        if (input.find(DELIMITER) != string::npos) {
            cout << "Error: Input cannot contain '" << DELIMITER << "'.\n";
            continue;
        }
        return input;
    }
}

// --- Helper: Validate Reg No (Strict 7 Digits) ---
bool Admin::isRegNoValid(const string& regNo) {
    if (regNo.length() != REG_NO_LENGTH) return false;
    for (size_t i = 0; i < regNo.length(); i++) {
        if (!isdigit(regNo[i])) return false;
    }
    return true;
}

// --- Helper: Check Room Capacity (Max 2) ---
bool Admin::isRoomAvailable(const string& roomNo) {
    int count = 0;
    for (unordered_map<string, Student>::iterator it = studentDB.begin(); it != studentDB.end(); ++it) {
        if (it->second.getRoomNo() == roomNo) {
            count++;
        }
    }
    return (count < MAX_ROOM_CAPACITY);
}

// --- Login ---
bool Admin::login() {
    string u, p;
    cout << "==== Admin Login ====\n";
    cout << "Username: "; getline(cin, u);
    cout << "Password: "; getline(cin, p);

    if (u == ADMIN_USER && p == ADMIN_PASS) {
        cout << "Login Successful!\n";
        return true;
    }
    cout << "Invalid Credentials.\n";
    return false;
}

// --- Add Student (Robust) ---
void Admin::addStudent() {
    cout << "\n--- Register New Student ---\n";

    // 1. Get and Validate Reg No
    string reg;
    while (true) {
        cout << "Enter Reg No (7 digits): ";
        getline(cin, reg);
        
        if (!isRegNoValid(reg)) {
            cout << "Error: Reg No must be exactly 7 integers.\n";
            continue;
        }
        if (studentDB.find(reg) != studentDB.end()) {
            cout << "Error: Student with Reg No " << reg << " already exists! Registration Rejected.\n";
            return; // Reject immediately
        }
        break;
    }

    // 2. Get and Validate Room No
    string room;
    while (true) {
        room = getSafeString("Enter Room No: ");
        if (!isRoomAvailable(room)) {
            cout << "Error: Room " << room << " is full (Max " << MAX_ROOM_CAPACITY << " students). Try another room.\n";
            continue;
        }
        break;
    }

    // 3. Get Details
    string name = getSafeString("Enter Name: ");
    string pass = getSafeString("Enter Password: ");

    // 4. Create and Save
    Student newStudent(name, reg, room, pass);
    studentDB[reg] = newStudent;
    cout << "Success: Student Registered.\n";
    saveToFile(); // Auto-save
}

// --- Delete Student ---
void Admin::deleteStudent() {
    string reg;
    cout << "Enter Reg No to delete: ";
    getline(cin, reg);

    if (studentDB.find(reg) != studentDB.end()) {
        // 1. Remove from Map (Destructor will clean up linked list RAM)
        studentDB.erase(reg);
        
        // 2. Delete the Bill File
        string billFile = "bills/" + reg + "_bill.txt";
        if (remove(billFile.c_str()) == 0) {
            cout << "Billing record deleted.\n";
        } else {
            cout << "No billing record found to delete.\n";
        }

        cout << "Student deleted successfully.\n";
        saveToFile(); // Update DB file
    } else {
        cout << "Error: Student not found.\n";
    }
}

// --- Search Student ---
void Admin::searchStudent() {
    string reg;
    cout << "Enter Reg No to search: ";
    getline(cin, reg);

    if (studentDB.find(reg) != studentDB.end()) {
        cout << "Student Found:\n";
        studentDB[reg].displayStudent();
    } else {
        cout << "Error: Student not found.\n";
    }
}

// --- Display All ---
void Admin::displayAllStudents() {
    if (studentDB.empty()) {
        cout << "No students registered.\n";
        return;
    }
    cout << "\n==== Registered Students ====\n";
    for (unordered_map<string, Student>::iterator it = studentDB.begin(); it != studentDB.end(); ++it) {
        it->second.displayStudent();
    }
}

// --- File Handling: Save ---
void Admin::saveToFile() {
    ofstream file(DB_FILE.c_str());
    if (!file.is_open()) {
        cout << "CRITICAL ERROR: Could not open " << DB_FILE << " for saving!\n";
        return;
    }

    for (unordered_map<string, Student>::iterator it = studentDB.begin(); it != studentDB.end(); ++it) {
        Student s = it->second;
        // Format: Reg|Name|Room|Password
        file << s.getRegNo() << DELIMITER 
             << s.getName() << DELIMITER 
             << s.getRoomNo() << DELIMITER 
             << s.getPassword() << "\n";
    }
    file.close();
    // cout << "Database saved.\n"; // Optional: keep silent for better UX
}

// --- File Handling: Load ---
void Admin::loadFromFile() {
    ifstream file(DB_FILE.c_str());
    if (!file.is_open()) {
        cout << "Warning: No database file found. Starting fresh.\n";
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string segment;
        vector<string> data;

        // Split by '|'
        while (getline(ss, segment, DELIMITER)) {
            data.push_back(segment);
        }

        // Validate data integrity (Must have 4 parts)
        if (data.size() == 4) {
            string reg = data[0];
            string name = data[1];
            string room = data[2];
            string pass = data[3];

            Student s(name, reg, room, pass);
            studentDB[reg] = s;
            count++;
        }
    }
    file.close();
    cout << "Database loaded: " << count << " students found.\n";
}