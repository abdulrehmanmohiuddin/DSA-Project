//module_3.hpp
#ifndef MODULE3_BILLING_H
#define MODULE3_BILLING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdio>  // For remove()
#include <direct.h> // For _mkdir (Windows specific)

using namespace std;

// Forward declaration (Break circular dependency)
class Student; 

// --- Transaction Node (Linked List) ---
struct TransactionNode {
    string date;        // The unique key for the day
    float price;        // The daily rate charged
    TransactionNode* next;

    TransactionNode(string d, float p) {
        date = d;
        price = p;
        next = NULL;
    }
};

// --- Billing System Class ---
class BillingSystem {
private:
    string currentDate; // "The Simulation Date" set by Admin
    const string LOG_FILE = "transactions_log.txt";

    // Private Helper: Append to Master Log
    void logToMasterFile(string regNo, string action, float amount);

public:
    BillingSystem();

    // 1. System Setup (Admin)
    void setSystemDate(string date);
    string getSystemDate();

    // 2. Core Operations (Student)
    // Returns true if student is ALREADY checked in for 'currentDate'
    bool isCheckedIn(Student& student); 
    
    // Toggles status: Check-In <-> Check-Out
    // Returns string message to display to user
    string toggleCheckIn(Student& student, float dailyPrice);

    // 3. Billing & Reporting
    float calculateTotalBill(Student& student);
    void generateStudentBill(Student& student); // Generates .txt file
    void displayTransactionHistory(Student& student);

    // 4. Admin Management
    void clearDues(Student& student); // "Mark as Paid" -> Deletes list
};

#endif // MODULE3_BILLING_H