/******************************************
Quest 09 - Programming Quest
Group Members
	- 21/ENG/016 - B.Anchuthan
	- 21/ENG/087 - M.S.S.Rahuman
	- 21/ENG/091 - N.Mathuwanthi
*******************************************/

// 21/ENG/087 - M.S.S.Rahuman
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

//21/ENG/091 - N.Mathuwanthi
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

// 21/ENG/016 - B.Anchuthan
void Employee::set_Date(Administrator & Admin)
{
    this -> date_Today = Admin.get_Date();
};

void Employee::set_Employee_Name(string employee_Name)
{
    this->employee_Name = employee_Name;
};

void Employee::creating_Customer(Customer & customerCreated,Bank& bank,ofstream& MyFile)
{
    string initial_Letter="";
    if(bank.customer_Arr.size()<9)
    {
        initial_Letter="00";
    }
    else if(bank.customer_Arr.size()<99)
    {
        initial_Letter="0";
    }
    string customer_contact;
    int account_type;
    customerCreated.created_Date = date_Today;
    string customer_Name= "Customer" +initial_Letter+to_string((bank.customer_Arr.size()) + 1);
    cout << "Customer Name : " << "Customer" +initial_Letter+to_string((bank.customer_Arr.size()) + 1)<<endl;
    cout << "Enter Contact Number : ";
    cin >> customer_contact;
    customerCreated.Customer_Name=customer_Name;
    customerCreated.customer_Contact_No = customer_contact;
    cout << "1) Current account" << endl;
    cout << "2) Saving account" << endl;
    cout << "Select Account Type : ";
    cin >> account_type;
    cout << endl;

    while (account_type < 0 || account_type > 3)
    {
        cout << "Input is Incorrect!! Select the Account from the Given List.";
        cout << "1) Current account" << endl;
        cout << "2) Saving account" << endl;
        cout << "Select Account Type : ";
        cin >> account_type;
        cout << endl;
    }

    if (account_type == 1)
    {
        customerCreated.type_Of_Account = "Current";
        customerCreated.is_Current_Acc = true;
        cout << "Enter the OD Limit : ";
        cin >> customerCreated.OD_Max;
    }
    else if (account_type == 2)
    {
        customerCreated.type_Of_Account = "Saving";
    }

    long cash_Deposit_Now;
    cout << "Enter the Amount Initially deposited : ";
    cin>>cash_Deposit_Now;
    customerCreated.cash_Deposit=cash_Deposit_Now;
    bank.customer_Name_Arr.push_back(customer_Name);
    bank.customer_Arr.push_back(customerCreated);
    MyFile<<customerCreated.date_Today<<" - "<<customerCreated.Customer_Name<<" : Account created."<<endl;
    MyFile<<customerCreated.date_Today<<" - "<<customerCreated.Customer_Name<<" : Deposited "<<cash_Deposit_Now<<endl;
    Transaction transaction_now_1;
    transaction_now_1.set_Transaction_Date(customerCreated.date_Today);
    transaction_now_1.set_Description(customerCreated.Customer_Name,"Money Credited to Customer Account",cash_Deposit_Now);
    customerCreated.transactions.push_back(transaction_now_1);

};

void Employee::deposit_Cash(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& Bank_File)
{
    if (customer.is_Closed == false)
    {
        long account_balance = customer.cash_Deposit;
        long now_deposited;
        cout << "Enter the Amount to be Deposited: ";
        cin >> now_deposited;
        customer.cash_Deposit = account_balance + now_deposited;
        MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Deposited "<<now_deposited<<endl;
        Transaction transaction_now_1;
        transaction_now_1.set_Transaction_Date(customer.date_Today);
        transaction_now_1.set_Description(customer.Customer_Name,"Money Credited to Customer Account",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.cash_Deposit >= 0)
        {
            bank.bank_Balance+=customer.OD_Now;
            Bank_File<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(customer.OD_Now)<<" LKR."<<endl;
            Bank_File<<customer.date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.set_Transaction_Date(customer.date_Today);
            transaction_now_2.set_Description(customer.Customer_Name,"Overdraft Credited to Bank Account",customer.OD_Now);
            bank.transactions.push_back(transaction_now_2);
            customer.OD_Now = 0;

        }
        else
        {
            bank.bank_Balance+=now_deposited;
            customer.OD_Now -= now_deposited;
            Bank_File<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(now_deposited)<<" LKR."<<endl;
            Bank_File<<customer.date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.set_Transaction_Date(customer.date_Today);
            transaction_now_2.set_Description(customer.Customer_Name,"Overdraft Credited to Bank Account",now_deposited);
            bank.transactions.push_back(transaction_now_2);
        }
    }
    else
    {
        cout << "Account doesn't Exist!! Already Closed." << endl;
    }
};

void Employee::withdraw_Cash(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& Bank_File)
{
    if (customer.is_Closed == false)
    {
        long now_Withdraw;
        cout << "Enter the Amount You Want to Withdraw : ";
        cin >> now_Withdraw;

        if (customer.is_Current_Acc == false && now_Withdraw > customer.cash_Deposit)
        {
            cout << "Sorry!! Account Balance is Insufficient." << endl;
        }
        else if (customer.is_Current_Acc == false && now_Withdraw <= customer.cash_Deposit)
        {
            customer.cash_Deposit -= now_Withdraw;
            cout << "Withdrawal Successful." << endl;
            MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Withdrawn "<<now_Withdraw<<endl;
            Transaction transaction_now_1;
            transaction_now_1.set_Transaction_Date(customer.date_Today);
            transaction_now_1.set_Description(customer.Customer_Name,"Money Debited from Customer Account",now_Withdraw);
            bank.transactions.push_back(transaction_now_1);
        }
        else if (customer.is_Current_Acc == true && now_Withdraw > customer.cash_Deposit)
        {
            if (((customer.cash_Deposit - now_Withdraw) * (-1)) <= customer.OD_Max && ((customer.cash_Deposit - now_Withdraw) * (-1)) <= bank.bank_Balance )
            {
                customer.OD_Now = now_Withdraw - (customer.cash_Deposit);
                bank.bank_Balance-=customer.OD_Now;
                customer.cash_Deposit -= now_Withdraw;
                cout << "Withdrawal Successful." << endl;
                Bank_File<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Overdraft Debited to Bank Account : "<<to_string(customer.OD_Now)<<" LKR."<<endl;
                Bank_File<<customer.date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
                MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Withdrawn "<<now_Withdraw<<endl;
                Transaction transaction_now_2;
                Transaction transaction_now_3;
                transaction_now_2.set_Transaction_Date(customer.date_Today);
                transaction_now_3.set_Transaction_Date(customer.date_Today);
                transaction_now_2.set_Description(customer.Customer_Name,"Money Withdrawn from Customer Account",now_Withdraw);
                transaction_now_3.set_Description(customer.Customer_Name,"Overdraft Debited to Bank Account",customer.OD_Now);
                customer.transactions.push_back(transaction_now_2);
                bank.transactions.push_back(transaction_now_3);
            }
            else if(((customer.cash_Deposit - now_Withdraw) * (-1)) <= customer.OD_Max && ((customer.cash_Deposit - now_Withdraw) * (-1)) > bank.bank_Balance)
            {
                cout << "Withdrawl Deined!! Requested Amount Exceeds the Balance in the Bank.";
            }
            else
            {
                cout << "Withdrawl Deined!! Requested Amount Exceeds the Maximum OverDraft";
            }
        }
        else if (customer.is_Current_Acc == true && now_Withdraw <= customer.cash_Deposit)
        {
            customer.cash_Deposit -= now_Withdraw;
            cout << "Withdrawal Successful." <<endl;
            MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Withdrawn "<<now_Withdraw<<endl;
            Transaction transaction_now_4;
            transaction_now_4.set_Transaction_Date(customer.date_Today);
            transaction_now_4.set_Description(customer.Customer_Name,"Money Debited from Customer Account",now_Withdraw);
            customer.transactions.push_back(transaction_now_4);
        }
    }
    else
    {
        cout << "Account doesn't Exist!! Already Closed." << endl;
    }
};

void Employee::view_Transactions(Bank& bank)
{
    int no_Of_Bank_Transactions=bank.transactions.size();
    cout<<"Transactions Related to the Bank's Money"<<endl;
    for(int i=0; i<no_Of_Bank_Transactions; i++)
    {
        cout<<bank.transactions[i].get_Description()<<endl;
    }
    cout<<endl;
    for(int i=0; i<bank.customer_Arr.size(); i++)
    {
        cout<<"Transaction Related with "+bank.customer_Arr[i].Customer_Name<<endl;
        for(int j=0; j<bank.customer_Arr[i].transactions.size(); j++)
        {
            cout<<bank.customer_Arr[i].transactions[j].get_Description()<<endl;
        }
    }
    cout<<endl;
};

string Employee::get_Employee_Name()
{
    return this->employee_Name;
};

void Employee::close_Account(Customer& customer,ofstream& MyFile)
{
    if (customer.cash_Deposit == 0)
    {
        customer.is_Closed = true;
        cout << "Account Closed Successfully." << endl;
        MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Account Closed"<<endl;
    }
    else if (customer.cash_Deposit > 0)
    {
        string user_Input_Close_Acc;
        cout << "If You Want to Withdraw the remaining amount of " << customer.cash_Deposit << " and Close the Account"<<endl;
        cout << "\tEnter 1 : Yes\nEnter 2 : No"<<endl;
        cout << "Selection : ";
        cin >> user_Input_Close_Acc;
        cout << endl;
        if (user_Input_Close_Acc == "1")
        {
            customer.cash_Deposit == 0;
            customer.is_Closed = true;
            cout << "Account Closed Successfully." << endl;
            MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Remaining Amount of"<<customer.cash_Deposit <<" is Taken and Account is Closed."<<endl;
            Transaction transaction_now_2;
            transaction_now_2.set_Transaction_Date(customer.date_Today);
            transaction_now_2.set_Description(customer.Customer_Name,"Money Debited from Customer Account",customer.cash_Deposit);
            customer.transactions.push_back(transaction_now_2);

        }
        else if (user_Input_Close_Acc == "2")
        {
            cout << "Account not Closed" << endl;
        }
    }
    else if (customer.cash_Deposit < 0)
    {
        cout << "Account Closing Request Denied!! There is an OverDraft in this Account." << endl;
    }

};

// 21/ENG/087 - M.S.S.Rahuman
void deposit_From_Outside(Bank& bank,Customer& customer,ofstream& MyFile,ofstream& Bank_File)
{
    if (customer.is_Closed == false)
    {
        long account_balance = customer.cash_Deposit;
        long now_deposited;
        cout << "Enter the amount to be deposited : ";
        cin >> now_deposited;
        customer.cash_Deposit = account_balance + now_deposited;
        Transaction transaction_now_1;
        transaction_now_1.set_Transaction_Date(customer.date_Today);
        transaction_now_1.set_Description(customer.Customer_Name,"Money Credited to Customer Account",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.cash_Deposit >= 0)
        {
            bank.bank_Balance+=customer.OD_Now;
            Bank_File<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(customer.OD_Now)<<" LKR."<<endl;
            Bank_File<<customer.date_Today<<" - "<<"Bank balance : "<<bank.bank_Balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.set_Transaction_Date(customer.date_Today);
            transaction_now_2.set_Description(customer.Customer_Name,"Overdraft Credited to Bank Account",customer.OD_Now);
            bank.transactions.push_back(transaction_now_2);
            customer.OD_Now = 0;
        }
        else
        {
            bank.bank_Balance+=now_deposited;
            customer.OD_Now -= now_deposited;
            Bank_File<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Overdraft Credited to Bank Account : "<<to_string(now_deposited)<<" LKR."<<endl;
            Bank_File<<customer.date_Today<<" - "<<"Bank Balance : "<<bank.bank_Balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.set_Transaction_Date(customer.date_Today);
            transaction_now_2.set_Description(customer.Customer_Name,"Overdraft Credited to Bank Account",now_deposited);
            bank.transactions.push_back(transaction_now_2);

        }

        for(int i=0; i< bank.customer_Arr.size(); i++)
        {
            if(customer.Customer_Name==bank.customer_Arr[i].Customer_Name)
            {
                bank.customer_Arr[i].cash_Deposit=customer.cash_Deposit;
                bank.customer_Arr[i].OD_Now=customer.OD_Now;
                break;
            }
        }
        cout<<"Deposit Succesfull."<<endl;
        MyFile<<customer.date_Today<<" - "<<customer.Customer_Name<<" : Deposited "<<now_deposited<<endl;
    }
    else
    {
        cout << "Account doesn't Exist!! Already Closed." << endl;
    }
}

string get_PW()
{
	string password;
	char ch;
    cout << "Enter the Password : ";
    
    while (true)
    {
        ch = _getch(); 
        if (ch == '\r') 
        {
            cout << endl;
            break;
        }
        password += ch;
        cout << '*'; 
    }

    return password;
}


// 21/ENG/091 - N.Mathuwanthi
int main()
{
    Bank bank;
    vector<string> employee_Name_Arr;
    vector<string> customer_Name_Arr;

    ofstream Bank_File("Bank_Transaction.txt");
    ofstream Customer_File("Customer_Details.txt");
    fstream BankFile_handler;
    fstream CustomerFile_handler;

    BankFile_handler.open("Bank_Transaction.txt", ios:: in | ios::out);
    CustomerFile_handler.open("Customer_Details.txt", ios:: in | ios::out);

    if (!BankFile_handler)
    {
        cout << "Error!!! Bank File not Found.";
        exit(1);
    }

    if (!CustomerFile_handler)
    {
        cout << "Error!!! Customer details File not Found.";
        exit(1);
    }

    int input=1,user_Input_1, user_Input_2,user_Input_3;
    Administrator Admin;
    Admin.set_Date();
    string username, password;
    while(input!=0)
    {
    	cout << "-----------------------------------"<<endl;
    	cout << "     Online Banking - LMN Bank     "<< endl;
    	cout << "-----------------------------------"<<endl<< endl;
    	cout << "1) Deposit From an Outside Source" << endl;
        cout << "2) Login" << endl;
        cout << "Select an Option Above : " ;
        cin  >> user_Input_1;
        
        if (user_Input_1 == 2)
        {
            int login_Check=0;
            while(login_Check!=1)
            {
                cout << "\n1) Administrator Login" << endl;
                cout << "2) Customer Login" << endl;
                cout << "3) Employee Login" << endl;
                cout << "4) Go Back"<< endl;
                cout << "Select an Option Above : ";
                cin  >> user_Input_2;
                cout << endl;
                
                if (user_Input_2 == 1)
                {
                    cout << "Enter Username : ";
                    cin >> username;
                    password = get_PW();

                    while (username != "Admin" || password != "Password@1234")
                    {
                        cout << "Entered Username/Password Incorrect. Try Again" << endl << endl;
                        cout << "Enter Username : ";
                        cin  >> username;
                        password = get_PW();
                    }
                    cout << "Logged in Successfully" << endl<<endl;
                    int checker_admin=0;
                    while(checker_admin!=1)
                    {
                        cout<<"\nDate : "<<Admin.get_Date()<<endl;
                        float annualInterest=Admin.get_Annual_Interest();
                        float OD_Charge=Admin.get_OD_Charge();
                        cout<<"Annual Interest % : "<<annualInterest<<endl;
                        cout<<"OD Charge : "<<OD_Charge<<endl;
                        cout<<"Current Bank Balance : "<<bank.bank_Balance<<endl<<endl;
                        cout<<"1) Create Employee"<<endl;
                        cout<<"2) Increase the Date"<<endl;
                        cout<<"3) Update OD Charge"<<endl;
                        cout<<"4) Update Annual Savings Interest %"<<endl;
                        cout<<"5) Logout"<<endl;
                        cout<<"Select an Option Above : ";
                        cin >>user_Input_3;
                        cout<<endl;
                        switch(user_Input_3)
                        {
                        case 1:
                        {
                            Admin.creating_Employee(bank);
                            Employee employee;
                            break;
                        }
                        case 2:
                        {
                            Admin.increasing_Date();
                            if(bank.customer_Arr.size()>0)
                            {
                                for(int i=0; i<bank.customer_Arr.size(); i++)
                                {
                                    Admin.update_Customer_Interest(bank.customer_Arr[i],bank,Bank_File);
                                    Admin.update_Customer_OD(bank.customer_Arr[i]);
                                }
                            }
                            break;
                        }
                        case 3:
                        {
                            Admin.set_OD_Charge();
                            break;
                        }
                        case 4:
                        {
                            Admin.set_Annual_Interest();
                            break;
                        }
                        case 5:
                        {
                            input=0;
                            checker_admin=1;
                            break;
                        }
                        default:
                        {}
                        }
                    }
                }
                else if(user_Input_2==4)
                {
                    login_Check=1;
                    input=1;
                    break;
                }
                else if(user_Input_2==3)
                {
                    cout << "Enter the Employee Username : ";
                    cin >> username;
                    password = get_PW();
                    employee_Name_Arr=bank.employee_Name_Arr;
                    Employee employeeNow;
                    for(int i=0; i<employee_Name_Arr.size(); i++)
                    {
                        if(username==employee_Name_Arr[i] && password =="Password@1234")
                        {
                            employeeNow=bank.employee_Arr[i];
                            login_Check=0;
                            break;
                        }
                        login_Check=1;
                    }
                    if(login_Check==1)
                    {
                        cout<<"Login Failed";
                        login_Check=0;
                        cout<<endl;
                    }
                    else
                    {
                        cout << "Logged in Successfully" << endl<<endl;
                        int checker_employee=0;
                        while(checker_employee!=1)
                        {
                            cout<<"\nDate : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_Annual_Interest();
                            float OD_Charge=Admin.get_OD_Charge();
                            cout<<"Annual Interest Rate : "<<annualInterest<<endl;
                            cout<<"OD Charge : "<<OD_Charge<<endl<<endl;
                            cout<<"1) Create Customer Account"<<endl;
                            cout<<"2) Close Customer Account"<<endl;
                            cout<<"3) Deposit Cash in Account"<<endl;
                            cout<<"4) Withdraw Cash from Account"<<endl;
                            cout<<"5) View Transactions"<<endl;
                            cout<<"6) Logout"<<endl;
                            cout <<"Select an Option Above : ";
                            cin>>user_Input_3;
                            cout<<endl;
                            switch(user_Input_3)
                            {
                            case 1:
                            {
                                Customer customerCreated;
                                customerCreated.set_Date(Admin);
                                employeeNow.creating_Customer(customerCreated,bank,Customer_File);
                                break;
                            }
                            case 2:
                            {
                                string customer_Name;
                                cout<<"Enter the Name of the Customer : ";
                                cin>>customer_Name;
                                for(int i=0; i<bank.customer_Arr.size(); i++)
                                {
                                    if(customer_Name==bank.customer_Arr[i].Customer_Name)
                                    {
                                        bank.customer_Arr[i].set_Date(Admin);
                                        employeeNow.close_Account(bank.customer_Arr[i],Customer_File);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 3:
                            {
                                string customer_Name;
                                cout<<"Enter the Name of the Customer : ";
                                cin>>customer_Name;
                                for(int i=0; i<bank.customer_Arr.size(); i++)
                                {
                                    if(customer_Name==bank.customer_Arr[i].Customer_Name)
                                    {
                                        bank.customer_Arr[i].set_Date(Admin);
                                        employeeNow.deposit_Cash(bank.customer_Arr[i],bank,Customer_File,Bank_File);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4:
                            {
                                string customer_Name;
                                cout<<"Enter the Name of the Customer : ";
                                cin>>customer_Name;
                                for(int i=0; i<bank.customer_Arr.size(); i++)
                                {
                                    if(customer_Name==bank.customer_Arr[i].Customer_Name)
                                    {
                                        bank.customer_Arr[i].set_Date(Admin);
                                        employeeNow.withdraw_Cash(bank.customer_Arr[i],bank,Customer_File,Bank_File);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5:
                            {
                                employeeNow.view_Transactions(bank);
                                break;
                            }
                            case 6:
                            {
                                input=0;
                                checker_employee=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }


                }
                else if(user_Input_2==2)
                {
                    int customer_Arr_Element;
                    cout << "Enter Username : ";
                    cin >> username;
                    password = get_PW();
                    customer_Name_Arr=bank.customer_Name_Arr;
                    Customer customer;
                    for(int i=0; i<customer_Name_Arr.size(); i++)
                    {
                        cout<<customer_Name_Arr[i]<<endl;
                    }
                    for(int i=0; i<customer_Name_Arr.size(); i++)
                    {
                        if(username==customer_Name_Arr[i] && password =="Password@1234")
                        {
                            customer=bank.customer_Arr[i];
                            customer_Arr_Element=i;
                            login_Check=0;
                            break;
                        }
                        login_Check=1;
                    }
                    if(login_Check==1)
                    {
                        cout<<"Login Failed";
                        login_Check=0;
                        cout<<endl;
                    }
                    else
                    {
                        cout << "Logged in Successfully" << endl<<endl;
                        int checker_customer=0;
                        while(checker_customer!=1)
                        {
                            cout<<"\nDate : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_Annual_Interest();
                            float OD_Charge=Admin.get_OD_Charge();
                            cout<<"Annual Interest % : "<<annualInterest<<endl;
                            cout<<"Overdraft Charge : "<<OD_Charge<<endl<<endl;
                            cout<<"Account Name : "<<customer.Customer_Name<<endl;
                            cout<<"Account Type : "<<customer.type_Of_Account<<endl;
                            cout<<"Account Contact Details : "<<customer.customer_Contact_No<<endl<<endl;
                            cout<<"Account Balance : "<<customer.cash_Deposit<<endl<<endl;
                            cout<<"Account OD : "<<customer.OD_Now<<endl<<endl;
                            cout<<"1) Deposit Cash in Account"<<endl;
                            cout<<"2) Withdraw Cash from Account"<<endl;
                            cout<<"3) View Transactions"<<endl;
                            cout<<"4) Logout"<<endl;
                            cout <<"Select an Option Above : ";
                            cin>>user_Input_3;
                            cout<<endl;
                            switch(user_Input_3)
                            {
                            case 1:
                            {
                                if(customer.is_Closed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.deposit_Cash_2(bank,Customer_File,Bank_File);
                                    bank.customer_Arr[customer_Arr_Element]=customer;
                                }
                                else
                                {
                                    cout<<"Account doesn't Exist!! Already Closed."<<endl;
                                }
                                break;
                            }
                            case 2:
                            {
                                if(customer.is_Closed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.withdraw_Cash_2(bank,Customer_File,Bank_File);
                                    bank.customer_Arr[customer_Arr_Element]=customer;
                                }
                                else
                                {
                                    cout<<"Account doesn't Exist!! Already Closed."<<endl;
                                }
                                break;
                            }
                            case 3:
                            {
                                customer.view_Transaction();
                                break;
                            }
                            case 4:
                            {
                                input=0;
                                checker_customer=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }
                }

            }
        }
        else if(user_Input_1==1)
        {
            string customer_User_Name;
            int is_Success=0;
            cout<<"Enter the Customer Name : ";
            cin>>customer_User_Name;
            for(int i=0; i<bank.customer_Arr.size(); i++)
            {
                if(customer_User_Name==bank.customer_Name_Arr[i])
                {
                    bank.customer_Arr[i].set_Date(Admin);
                    deposit_From_Outside(bank,bank.customer_Arr[i],Customer_File,Bank_File);
                    is_Success=1;
                    break;
                }
            }
            if(is_Success==0)
            {
                cout<<"Transaction Failed"<<endl<<endl;
            }
        }
    }
    return 0;
}
