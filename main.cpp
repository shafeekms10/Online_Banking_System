#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

class Administrator;
class Employee;
class Customer;
class Transaction;

class Bank
{
public:
    vector < Employee > employee_Arr;
    vector < string > employee_Name_Arr;
    vector < Customer > customer_Arr;
    vector < string > customer_Name_Arr;
    vector <Transaction> transactions;
    long bank_Balance=1000000;
};

class Administrator
{
private:
    string date_Today;
    float annual_Interest = 20;
    float OD_Charge = 2;
    int increased_Date=0;

public:
    void creating_Employee(Bank& bank);
    void set_Date();
    void increasing_Date();
    void set_Annual_Interest();
    void set_OD_Charge();
    void update_Customer_OD(Customer & customer);
    void update_Customer_Interest(Customer & customer,Bank& bank,ofstream& Bank_File);
    string get_Employee_Name(Bank& bank);
    string get_Date();
    float get_Annual_Interest();
    float get_OD_Charge();
    int employee_No(Bank& bank);
    vector<string> get_Employee_Name_Arr(Bank& bank);
};

class Employee
{
private:
    string employee_Name;
    string date_Today;
public:
    void set_Date(Administrator & Admin);
    void set_Employee_Name(string employee_Name);
    string get_Employee_Name();
    void creating_Customer(Customer & customerCreated,Bank& bank,ofstream& MyFile);
    void deposit_Cash(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& Bank_File);
    void withdraw_Cash(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& Bank_File);
    void view_Transactions(Bank& bank);
    void close_Account(Customer & customer,ofstream& MyFile);
};

class Customer
{
public:
    string Customer_Name;
    string type_Of_Account;
    string customer_Contact_No;
    long cash_Deposit;
    bool is_Current_Acc = false;
    long OD_Max = 0;
    bool is_Closed = false;
    long OD_Now = 0;
    string date_Today;
    string created_Date;
    vector <Transaction> transactions;

    void view_Transaction();
    void deposit_Cash_2(Bank& bank,ofstream& MyFile,ofstream& Bank_File);
    void set_Date(Administrator& Admin);
    void withdraw_Cash_2(Bank& bank,ofstream& MyFile,ofstream& Bank_File);
};

class Transaction
{
private:
    string date_Today;
    string description;
public:

    void set_Transaction_Date(string date_Today)
    {
        this->date_Today=date_Today;
    }

    void set_Description(string customer_name,string debitOrCredit,long amount)
    {
        this->description=this->date_Today+" - "+customer_name+" : "+debitOrCredit+" : "+to_string(amount)+" Rs.";
    }
    string get_Description()
    {
        return this->description;
    }

};
