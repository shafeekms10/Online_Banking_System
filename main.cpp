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

int Administrator::employee_No(Bank& bank)
{
    return bank.employee_Arr.size();
};

vector<string> Administrator::get_Employee_Name_Arr(Bank& bank)
{
    return bank.employee_Name_Arr;
};

void Administrator::creating_Employee(Bank& bank)
{
    string initial_Letter="";
    if(bank.employee_Arr.size()<9)
    {
        initial_Letter="00";
    }
    else if(bank.employee_Arr.size()<99)
    {
        initial_Letter="0";
    }
    string employee_Name=("employee" +initial_Letter+to_string((bank.employee_Arr.size()) + 1));
    Employee employee;
    employee.set_Employee_Name(employee_Name);
    bank.employee_Arr.push_back(employee);
    bank.employee_Name_Arr.push_back(employee_Name);
    Employee last_Created_Employee=bank.employee_Arr.back();
    cout<<last_Created_Employee.get_Employee_Name()+" : Employee Created Successfully"<<endl;;
};

void Administrator::set_Date()
{
    string date_Today;
    time_t ttime = time(0);
    tm * local_time = localtime( & ttime);
    date_Today = to_string(1900 + local_time -> tm_year) + ":" + to_string(1 + local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
    this -> date_Today = date_Today;
};

void Administrator::increasing_Date()
{
    this->increased_Date+=1;
    time_t ttime = time(0);
    ttime += (24 * 60 * 60)*(this->increased_Date);
    tm * local_time = localtime( & ttime);
    this->date_Today = to_string(1900 + local_time -> tm_year) + ":" + to_string(1+local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
};

void Administrator::set_Annual_Interest()
{
    float annual_Interest;
    cout << "Enter New Annual Savings Interest (%) : ";
    cin >> annual_Interest;
    this -> annual_Interest = annual_Interest;
    cout<<"Current Annual Savings Interest (%) : "<< annual_Interest<<endl;
};

void Administrator::set_OD_Charge()
{
    float OD_Charge;
    cout << "Enter New OD Charge ('%' per day) : ";
    cin >> OD_Charge;
    this -> OD_Charge = OD_Charge;
    cout<<"Current OD Charge "<< OD_Charge<<endl;
};

void Administrator::update_Customer_OD(Customer & customer)
{
    if (customer.OD_Now > 0)
    {
        customer.OD_Now += ((this -> OD_Charge) / 100) * (customer.OD_Now);
        customer.cash_Deposit=(customer.OD_Now)*(-1);

    }
}

void Administrator::update_Customer_Interest(Customer & customer,Bank& bank,ofstream& Bank_File)
{
    float daily_Interest = (this -> annual_Interest) / 365;
    if (customer.cash_Deposit > 0)
    {
        cout<<"Line : 1"<<endl;
        customer.cash_Deposit += ((daily_Interest) / 100) * (customer.cash_Deposit);
        bank.bank_Balance-=((daily_Interest) / 100) * (customer.cash_Deposit);
        Bank_File<<this->date_Today<<" - "<<customer.Customer_Name<<" : Interest Debited to Bank account : "<<to_string(((daily_Interest) / 100) * (customer.cash_Deposit))<<" Rs."<<endl;
        Bank_File<<this->date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
        Transaction transaction_now;
        transaction_now.set_Transaction_Date(this->date_Today);
        transaction_now.set_Description(customer.Customer_Name,"Interest Credited to Customer Account",((daily_Interest) / 100) * (customer.cash_Deposit));
        bank.transactions.push_back(transaction_now);
        customer.transactions.push_back(transaction_now);

    }
}

string Administrator::get_Employee_Name(Bank& bank)
{
    return bank.employee_Arr.back().get_Employee_Name();
}

string Administrator::get_Date()
{
    return this->date_Today;
}

float Administrator::get_Annual_Interest()
{
    return (this->annual_Interest);
};

float Administrator::get_OD_Charge()
{
    return (this->OD_Charge);
};


void Customer::view_Transaction()
{
    int no_Of_Transactions=this->transactions.size();
    for(int i=0; i<no_Of_Transactions; i++)
    {
        cout<<this->transactions[i].get_Description()<<endl;
    }
};

void Customer::deposit_Cash_2(Bank& bank,ofstream& MyFile,ofstream& Bank_File)
{
    int cash_Added;
    cout << "Enter the amount to Deposit : ";
    cin >> cash_Added;
    this -> cash_Deposit += cash_Added;
    MyFile<<this->date_Today<<" - "<<this->Customer_Name<<" : Deposited "<<to_string(cash_Added)<<endl;
    Transaction transaction_now_1;
    transaction_now_1.set_Transaction_Date(this->date_Today);
    transaction_now_1.set_Description(this->Customer_Name,"Money Credited to Customer Account",cash_Added);
    this->transactions.push_back(transaction_now_1);

    if (this -> cash_Deposit >= 0)
    {
        bank.bank_Balance+=OD_Now;
        Bank_File<<this->date_Today<<" - "<<this->Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(OD_Now)<<" LKR."<<endl;
        Bank_File<<this->date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
        Transaction transaction_now_2;
        transaction_now_2.set_Transaction_Date(this->date_Today);
        transaction_now_2.set_Description(this->Customer_Name,"Overdraft Credited to Bank Account",OD_Now);
        bank.transactions.push_back(transaction_now_2);
        this -> OD_Now = 0;
    }
    else
    {
        bank.bank_Balance+=cash_Added;
        Bank_File<<this->date_Today<<" - "<<this->Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(cash_Added)<<" Rs."<<endl;
        Bank_File<<this->date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
        this -> OD_Now -= cash_Added;
        Transaction transaction_now_2;
        transaction_now_2.set_Transaction_Date(this->date_Today);
        transaction_now_2.set_Description(this->Customer_Name,"Overdraft Credited to Bank Account",cash_Added);
        bank.transactions.push_back(transaction_now_2);
    }
    for(int i=0; i<=bank.customer_Name_Arr.size(); i++)
    {
        if(this->Customer_Name==bank.customer_Name_Arr[i])
        {
            bank.customer_Arr[i].cash_Deposit=this->cash_Deposit;
            bank.customer_Arr[i].OD_Now=this->OD_Now;
        }
    }
}

void Customer::set_Date(Administrator & Admin)
{
    this -> date_Today = Admin.get_Date();
};

void Customer::withdraw_Cash_2(Bank& bank,ofstream& MyFile,ofstream& Bank_File)
{
    int withdraw_Amount;
    cout << "Enter the Amount to Withdraw : ";
    cin >> withdraw_Amount;
    if (this -> is_Current_Acc == false && withdraw_Amount > this -> cash_Deposit)
    {
        cout << "Sorry! Your Account Balance is Insufficient." << endl;
    }
    else if (this -> is_Current_Acc == false && withdraw_Amount <= this -> cash_Deposit)
    {
        this -> cash_Deposit -= withdraw_Amount;
        cout << "Withdrawal Successful." << endl;
        MyFile<<this->date_Today<<" - "<<this->Customer_Name<<" : Withdrawn "<<to_string(withdraw_Amount)<<endl;
        Transaction transaction_now_1;
        transaction_now_1.set_Transaction_Date(this->date_Today);
        transaction_now_1.set_Description(this->Customer_Name,"Money Withdrawn from Customer Account",withdraw_Amount);
        this->transactions.push_back(transaction_now_1);
    }
    else if (this -> is_Current_Acc == true && withdraw_Amount > this -> cash_Deposit)
    {
        if (((this -> cash_Deposit - withdraw_Amount) * (-1)) <= this -> OD_Max && ((this -> cash_Deposit - withdraw_Amount) * (-1)) <= bank.bank_Balance )
        {
            this -> OD_Now = withdraw_Amount - (this -> cash_Deposit);
            bank.bank_Balance-=this->OD_Now;
            this -> cash_Deposit -= withdraw_Amount;
            cout << "Withdrawal Successful." << endl;
            Bank_File<<this->date_Today<<" - "<<this->Customer_Name<<" : Overdraft Debited to Bank Account : "<<to_string(this->OD_Now)<<" LKR."<<endl;
            Bank_File<<this->date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
            MyFile<<this->date_Today<<" - "<<this->Customer_Name<<" : Withdrawn "<<to_string(withdraw_Amount)<<endl;
            Transaction transaction_now_2;
            Transaction transaction_now_3;
            transaction_now_2.set_Transaction_Date(this->date_Today);
            transaction_now_3.set_Transaction_Date(this->date_Today);
            transaction_now_2.set_Description(this->Customer_Name,"Money Withdrawn from Customer Account",withdraw_Amount);
            transaction_now_3.set_Description(this->Customer_Name,"Overdraft Debited to Bank Account",this->OD_Now);
            this->transactions.push_back(transaction_now_2);
            bank.transactions.push_back(transaction_now_3);
        }
        else if(((this -> cash_Deposit - withdraw_Amount) * (-1)) <= this -> OD_Max && ((this -> cash_Deposit - withdraw_Amount) * (-1)) > bank.bank_Balance)
        {
            cout << "Withdrawl Deined!! Requested Amount Exceeds the Balance in the Bank.";
        }
        else
        {
            cout << "Withdrawl Deined!! Requested Amount Exceeds the Maximum OverDraft";
        }
    }
    else if (this -> is_Current_Acc == true && withdraw_Amount <= this -> cash_Deposit)
    {
        this -> cash_Deposit -= withdraw_Amount;
        cout << "Withdrawal Successful" << endl;
        MyFile<<this->date_Today<<" - "<<this->Customer_Name<<" : Withdrawn "<<to_string(withdraw_Amount)<<endl;
        Transaction transaction_now_4;
        transaction_now_4.set_Transaction_Date(this->date_Today);
        transaction_now_4.set_Description(this->Customer_Name,"Money Withdrawn from Customer Account",withdraw_Amount);
        this->transactions.push_back(transaction_now_4);
    }
    for(int i=0; i<=bank.customer_Name_Arr.size(); i++)
    {
        if(this->Customer_Name==bank.customer_Name_Arr[i])
        {
            bank.customer_Arr[i].cash_Deposit=this->cash_Deposit;
            bank.customer_Arr[i].OD_Now=this->OD_Now;
        }
    }

};
