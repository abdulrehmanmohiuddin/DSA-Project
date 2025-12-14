#ifndef MODULE1_USERMANAGEMENT_H
#define MODULE1_USERMANAGEMENT_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const string DB_FILE = "students.txt";
const char DELIMITER = '|';

// --- Student Class ---
class Student {
private:
    string name;
    string regNo;   
    string roomNo;

public:
    Student();
    Student(string n, string reg, string room);
    
    string getRegNo() const;
    string getName() const;
    string getRoomNo() const;

    void displayStudent() const;
};

// --- Admin Class (Manager) ---
class Admin {
private:
    unordered_map<string, Student> studentDB;

    // Helper
    string getSafeString(const string& prompt);

public:
    Admin();
    
    // --- FEATURES ---
    void addStudent();          // Feature 1: Create
    void displayAllStudents();  // Feature 2: Read
    void searchStudent();       // Feature 3: Search (New)
    void deleteStudent();       // Feature 4: Delete (New)

    // File Handling
    void saveToFile();
    void loadFromFile();
};

#endif