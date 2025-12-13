#ifndef MODULE3_BILLING_H
#define MODULE3_BILLING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <direct.h>

using namespace std;
class Student; 

struct TransactionNode {
    string date;    
    float price;        
    TransactionNode* next;

    TransactionNode(string d, float p) {
        date = d;
        price = p;
        next = NULL;
    }
};

class BillingSystem {
private:
    string currentDate;
    const string LOG_FILE = "transactions_log.txt";
    
    void logToMasterFile(string regNo, string action, float amount);

public:
    BillingSystem();

    void setSystemDate(string date);
    
    string getSystemDate();
    bool isCheckedIn(Student& student); 
    string toggleCheckIn(Student& student, float dailyPrice);
    float calculateTotalBill(Student& student);

    void generateStudentBill(Student& student); 
    void displayTransactionHistory(Student& student);

    // 4. Admin Management
    void clearDues(Student& student); // "Mark as Paid" -> Deletes list
};

#endif // MODULE3_BILLING_H
