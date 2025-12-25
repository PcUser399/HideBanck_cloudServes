#include <iostream>
#include "Cpp JSON Methods/CppToJson.h"
#include "../../../BackEnd/Include/Controllers/EmployeeController.h"
#include "../../../BackEnd/Include/Controllers/CustomerController.h"
#include "../../../BackEnd/Include/Controllers/LoanController.h"
#include "../../../BackEnd/Include/Controllers/StatisticsController.h"
#include "../../../BackEnd/CSV/CustomerCSV.h"
#include "../../../BackEnd/CSV/TransactionCSV.h"
#include "../../../BackEnd/CSV/EmployeeCSV.h"

#include <typeinfo>
using namespace std;

int main()
{
    loadEverything();
    string input;
    getline(cin, input);
    json j = json::parse(input);

    string command = j["command"].get<string>();
    string info = j["info"].get<string>();

    if (command == "Load All Accounts")
    {
        std::cout << StringifyAllAccounts() << endl;
    }
    else if (command == "Load All Employees")
    {
        std::cout << StringifyAllEmployees() << endl;
    }
    else if (command == "Load Account")
    {
        std::cout << StringifyAccount(info) << endl;
    }
    else if (command == "Load Employee")
    {

        std::cout << StringifyEmployee(info) << endl;
    }
    else if (command == "createAccount")
    {
        int customerCountq = 0;
        int accountCountq = 0;
        Customer::Customer cusarr[50];
        Account::Account accarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        EmployeeController::AddCustomerAccount(cusarr, customerCountq, accarr, accountCountq, j["object"]["Branch"].get<string>(), j["object"]["AccountName"].get<string>(), j["object"]["CustomerId"].get<string>(), j["object"]["note"].get<string>(), j["object"]["AccountType"].get<string>());
    }
    else if (command == "Add New Transaction")
    {
        int customerCountq = 0;
        int transactionCountq = 0;
        int accountCountq = 0;
        Customer::Customer cusarr[50];
        Transaction::Transaction tansarr[50];
        Account::Account accarr[50];
        TransactionCSV::Load(tansarr, transactionCountq, accarr, accountCountq, cusarr, customerCountq);
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        if (j["object"]["type"] == "Withdraw")
        {
            CustomerController::Withdraw(stod(j["object"]["amount"].get<string>()), j["object"]["accountNumber"].get<string>(), accounts, accountCount, transactions, transactionCount);
        }
        else
        {
            CustomerController::Deposit(stod(j["object"]["amount"].get<string>()), j["object"]["accountNumber"].get<string>(), accounts, accountCount, transactions, transactionCount);
        }
    }
    else if (command == "Undo Last Transaction")
    {
        int customerCountq = 0;
        int transactionCountq = 0;
        int accountCountq = 0;
        Customer::Customer cusarr[50];
        Transaction::Transaction tansarr[50];
        Account::Account accarr[50];
        TransactionCSV::Load(tansarr, transactionCountq, accarr, accountCountq, cusarr, customerCountq);
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        CustomerController::UndoTransaction(j["info"].get<string>(), accounts, accountCount, transactions, transactionCount);
    }
    else if (command == "Add Loan Request")
    {

        int customerCountq = 0;
        int loanCount = 0;
        int accountCountq = 0;
        Account::Account accarr[50];
        Customer::Customer cusarr[50];
        Loan::Loan loansarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        LoanCSV::Load(loansarr, loanCount);
        CustomerController::RequestLoan(loansarr, loanCount, accarr, accountCountq, j["info"].get<string>(), stod(j["object"]["amount"].get<string>()), stoi(j["object"]["duration"].get<string>()), ("Request-" + j["object"]["type"].get<string>()));
    }
    else if (command == "Finalize Day")
    {
        EmployeeController::FinalizeTransactions();
    }
    else if (command == "Refuse Loan Request")
    {
        int customerCount = 0, loanCount = 0, loanReqCount = 0, accountCount = 0, transactionCount = 0;

        Customer::Customer cusarr[50];
        Account::Account accarr[50];
        Loan::Loan loansarr[50];
        Transaction::Transaction transarr[200];

        CustomerCSV::Load(cusarr, customerCount);
        AccountCSV::Load(accarr, accountCount, cusarr, &customerCount);
        LoanCSV::Load(loansarr, loanCount);
        TransactionCSV::Load(transarr, transactionCount, accarr, accountCount, cusarr, customerCount);

        EmployeeController::HandleSingleLoanRequest(
            j["info"].get<string>(),
            false,
            cusarr, customerCount,
            accarr, accountCount,
            loansarr, loanCount,
            transarr, transactionCount);
    }
    else if (command == "Accept Request")
    {
        int customerCount = 0, loanCount = 0, loanReqCount = 0, accountCount = 0, transactionCount = 0;

        Customer::Customer cusarr[50];
        Account::Account accarr[50];
        Loan::Loan loansarr[50];
        Transaction::Transaction transarr[200];

        CustomerCSV::Load(cusarr, customerCount);
        AccountCSV::Load(accarr, accountCount, cusarr, &customerCount);
        LoanCSV::Load(loansarr, loanCount);
        TransactionCSV::Load(transarr, transactionCount, accarr, accountCount, cusarr, customerCount);

        EmployeeController::HandleSingleLoanRequest(
            j["info"].get<string>(),
            true,
            cusarr, customerCount,
            accarr, accountCount,
            loansarr, loanCount,
            transarr, transactionCount);
    }
    else if (command == "Change Loan Status")
    {
        Loan::Loan loansarr[50];
        int loanCount = 0;
        LoanCSV::Load(loansarr, loanCount);
        EmployeeController::ChangeLoanStatus(loans, loanCount, j["info"].get<string>(), j["object"]["newStatus"].get<string>());
    }
    else if (command == "Delete Loan")
    {
        int customerCountq = 0;
        int loanCountq = 0;
        int accountCountq = 0;
        Account::Account accarr[50];
        Customer::Customer cusarr[50];
        Loan::Loan loansarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        LoanCSV::Load(loansarr, loanCountq);
        EmployeeController::DeleteLoan(loansarr, loanCountq, j["info"].get<string>());
    }
    else if (command == "Delete Closed Accounts")
    {
        int customerCountq = 0;
        int accountCountq = 0;
        Account::Account accarr[50];
        Customer::Customer cusarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);

        EmployeeController::DeleteClosedAccounts(accarr, accountCountq);
    }
    else if (command == "Delete Completed Loans")
    {
        int customerCountq = 0;
        int loanCountq = 0;
        int accountCountq = 0;
        Account::Account accarr[50];
        Customer::Customer cusarr[50];
        Loan::Loan loansarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);
        LoanCSV::Load(loansarr, loanCountq);

        LoanController::DeleteCompletedLoans(loansarr, loanCountq);
    }
    else if (command == "Fire Employee")
    {
        int employeeCountq = 0;
        Employee::Employee emparr[50];

        EmployeeCSV::Load(emparr, employeeCountq);

        EmployeeController::DeleteEmployee(emparr, employeeCountq, j["info"].get<string>());
    }
    else if (command == "Modify Employee")
    {
        // employee Id : j["info"].get<string>()
        // new Salary : j["object"]["newSalary"].get<string>()
        // newBranch : j["object"]["newBranch"].get<string>()
        // newStatus : j["object"]["newStatus"].get<string>()

        int employeeCountq = 0;
        Employee::Employee emparr[50];

        EmployeeCSV::Load(emparr, employeeCountq);

        EmployeeController::EditEmployee(emparr, employeeCountq, j["info"].get<string>(), stod(j["object"]["newSalary"].get<string>()), j["object"]["newBranch"].get<string>(), j["object"]["newStatus"].get<string>());
    }
    else if (command == "Add Employee")
    {
        //  Employee  Name : j["object"]["EmpName"].get<string>()
        //  Employee  LastName : j["object"]["EmpLastName"].get<string>()
        //  Employee  Address : j["object"]["EmpAdress"].get<string>()
        //  Employee  Salary : j["object"]["EmpSalary"].get<string>()
        //  Employee  BranchCode : j["object"]["EmpBranch"].get<string>()
        int employeeCountq = 0;
        Employee::Employee emparr[50];

        EmployeeCSV::Load(emparr, employeeCountq);

        EmployeeController::AddEmployee(emparr, employeeCountq, j["object"]["EmpName"].get<string>(), j["object"]["EmpLastName"].get<string>(), j["object"]["EmpAdress"].get<string>(), j["object"]["EmpBranch"].get<string>(), stod(j["object"]["EmpSalary"].get<string>()));
    }
    else if (command == "Change Account Status")
    {
        // account number : j["info"].get<string>()
        // new status : j["object"]["newStatus"].get<string>()

        int customerCountq = 0;
        int accountCountq = 0;
        Account::Account accarr[50];
        Customer::Customer cusarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        AccountCSV::Load(accarr, accountCountq, cusarr, &customerCountq);

        EmployeeController::ChangeAccountStatus(accarr, accountCountq, j["info"].get<string>(), j["object"]["newStatus"].get<string>());
    }
    else if (command == "load Interest Rate Evolution")
    {
        std::cout << timeLineCSV::stringifyInterestRateAsArray() << endl;
    }
    else if (command == "load Accounts Number Evolution")
    {
        std::cout << timeLineCSV::stringifyAccountsNumberAsArray() << endl;
    }
    else if (command == "load Employees Number Evolution")
    {
        std::cout << timeLineCSV::stringifyEmployeesNumberAsArray() << endl;
    }

    else if (command == "load loans Statistics")
    {
        int loanCountq = 0;
        Loan::Loan loansarr[50];
        LoanCSV::Load(loansarr, loanCountq);
        int totalNumberOfLoans = StatisticsController::TotalLoans(loansarr,loanCountq);
        int*types = StatisticsController::LoansByType(loansarr,loanCountq);
        int buisness = types[0];
        int car = types[1];
        int home = types[2];
        int student = types[3];
        int general = types[4];


        delete[] types ;

        int*status = StatisticsController::LoansByStatus(loansarr,loanCountq);
        int active = status[0];
        int completed = status[1];
        int overdue = status[2];
        int Requests = status[3];

        delete[] status ;

        std::cout<<StringifyLoanStatistics(totalNumberOfLoans,buisness,car,home,student,general,active,completed,overdue,Requests)<<endl ;

    }
    else if(command=="get Loans From Date"){
        int loanCountq = 0;
        Loan::Loan loansarr[50];
        LoanCSV::Load(loansarr, loanCountq);
        std::string startDate = j["object"]["startDate"].get<string>();
        std::string endDate = j["object"]["endDate"].get<string>();



        int nb = StatisticsController::ActiveLoansInDateRange(loansarr,loanCountq,startDate,endDate);
        std::cout<<nb<<endl ;
    }
    else if(command=="get Employees From Branch"){
        int employeeCountq = 0;
        string*arr;
        Employee::Employee emparr[50];
        EmployeeCSV::Load(emparr, employeeCountq);

        string br = j["info"].get<string>();



        int nb = StatisticsController::EmployeesPerBranch(emparr,employeeCountq,stoi(br));
        std::cout<<nb<<endl ;
    }
    else if (command == "load Customer Statistics"){
        loadEverything();
        

        int totalAccountNb = StatisticsController::TotalNumberOfAccounts(accounts,accountCount);

        timeLineCSV::updateAccountNumber(totalAccountNb);

        string*arr = StatisticsController::AccountMostLoans(accounts,accountCount);
        string MLCusId = arr[0];
        string ML = arr[1];
        string MLN = arr[2];

        delete[] arr ;

        arr = StatisticsController::CustomerHighestBalance(customers,customerCount);
        string MBCusId = arr[0];
        string MB = arr[1];
        string MBN = arr[2];

        delete[] arr;


        arr = StatisticsController::CustomerLowestBalance(customers,customerCount);
        string LBCusId = arr[0];
        string LB = arr[1];
        string LBN = arr[2];

        delete[]arr;



        std::cout<<StringifyCustomerStatistics(totalAccountNb,MLCusId,ML,MLN,MBCusId,MB,MBN,LBCusId,LB,LBN)<<endl ;


    }
    else if (command == "load Employee Statistics"){
        int employeeCountq = 0;
        string*arr;
        Employee::Employee emparr[50];

        EmployeeCSV::Load(emparr, employeeCountq);


        int totalNbOfEmp = StatisticsController::TotalEmployees(emparr,employeeCountq);

        timeLineCSV::updateEmployeeNumber(totalNbOfEmp);


        arr = StatisticsController::HighestSalaryEmployee(emparr,employeeCountq);


        string HSEI = arr[0];
        string HSES = arr[1];
        string HSESt = arr[2];

        delete [] arr;
        

        arr = StatisticsController::LowestSalaryEmployee(emparr,employeeCountq);


        string LSEI = arr[0];
        string LSES = arr[1];
        string LSESt = arr[2];

        delete[] arr;

        std::cout<<StringifyEmployeeStatistics(totalNbOfEmp,HSEI,HSES,HSESt,LSEI,LSES,LSESt)<<endl ;

    }


    else if (command == "check new month")
    {
        string inf = j["info"].get<string>();
        int oldMonth = stoi(timeLineCSV::getLastMonth());
        int newMonth = stoi(inf);
        int diff = Utils::monthDifference(oldMonth, newMonth);
        if(diff<=0){return -1;}
        else if (diff >=1)
        {
            if(diff > 1){
                for (int i = 0; i < diff - 1; i++) {
                    timeLineCSV::addNewMonth(stoi(timeLineCSV::getCurrentInterestRate()), stoi(timeLineCSV::getCurrentMonthAccountsNumber()), stoi(timeLineCSV::getCurrentMonthEmployeesNumber()), stoi(timeLineCSV::getlastMonthLoansNumber()));
                }
            }

            int loanCountq = 0;
            Loan::Loan loansarr[50];
            LoanCSV::Load(loansarr, loanCountq);
            int totalNumberOfLoans = StatisticsController::TotalLoans(loansarr,loanCountq);

            int employeeCountq = 0;
            Employee::Employee emparr[50];
            EmployeeCSV::Load(emparr, employeeCountq);
            int totalNbOfEmp = StatisticsController::TotalEmployees(emparr,employeeCountq);

            loadEverything();
            int totalAccountNb = StatisticsController::TotalNumberOfAccounts(accounts,accountCount);

            timeLineCSV::addNewMonth(stoi(timeLineCSV::getCurrentInterestRate()), totalAccountNb , totalNbOfEmp , totalNumberOfLoans);
            timeLineCSV::updateLastMonth(newMonth);
        }
    }
    else if (command == "Check CusId Existence")
    {
        int customerCountq = 0;
        Customer::Customer cusarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        Customer::Customer *cus;
        cus = CustomerController::FindCustomer(cusarr, customerCountq, info);
        if (cus)
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }
    else if (command == "Create Customer"){
        string name = j["object"]["name"].get<string>();
        string lastName = j["object"]["lastName"].get<string>();
        string adress = j["object"]["adress"].get<string>();
        string phoneNumber = j["object"]["phoneNumber"].get<string>();

        int customerCountq = 0;
        Customer::Customer cusarr[50];
        CustomerCSV::Load(cusarr, customerCountq);
        CustomerController::CreateCustomer(cusarr,customerCountq,name,lastName,adress,phoneNumber);

    }

    return 0;
}