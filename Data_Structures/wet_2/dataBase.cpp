#include "dataBase.h"

long long DataBase::getNumOfCompanies() const
{
    return num_of_companies;
}


static shared_ptr<Employee> createEmployee(const int EmployeeID, const int grade, MagicBox* magicbox)
{
    shared_ptr<Employee> emp(new Employee(EmployeeID, grade, magicbox));
    return emp;
}

void DataBase::addEmployee(const long long EmployeeID,const long long companyID, const long long grade)
{
    if (global_database->getEmployee(EmployeeID) != nullptr)
    {
        throw Failure();
    }


    MagicBox* company = companies_database->find(companyID);
    shared_ptr<Employee> emp = createEmployee(EmployeeID,grade,company);
    company->addEmployee(emp);
    global_database->addEmployee(emp);
}

void DataBase::removeEmployee(const long long EmployeeID)
{
    shared_ptr<Employee> emp = global_database->getEmployee(EmployeeID);
    if (emp == nullptr)
    {
        throw Failure();
    }
    
    MagicBox* company = emp->getMagicBox();
    company->removeEmployee(EmployeeID);
    global_database->removeEmployee(EmployeeID);
}

 void DataBase::acquireCompany(const long long AquirerID, const long long TargetID, double Factor)
 {
    companies_database->uniteCompanies(AquirerID,TargetID,Factor);
 }

 void DataBase::employeeSalaryIncrease(const long long EmployeeID, const long long SalaryIncrease)
 {
    shared_ptr<Employee> employee = global_database->getEmployee(EmployeeID);
    if (employee == nullptr)
    {
        throw Failure();
    }
    
    
    if (employee->getSalary() == 0)
    {  
        employee->raiseSalary(SalaryIncrease);
        employee->getMagicBox()->salaryIncrease(EmployeeID,false);
        global_database->salaryIncrease(EmployeeID,false);
    }

    else
    {
        employee->getMagicBox()->getTree()->remove(employee);
        global_database->getTree()->remove(employee);
        employee->raiseSalary(SalaryIncrease);
        employee->getMagicBox()->getTree()->insert(employee);
        global_database->getTree()->insert(employee);
    }
 }

 void DataBase::promoteEmployee(const long long EmployeeID,const long long BumpGrade)
 {
     shared_ptr<Employee> employee = global_database->getEmployee(EmployeeID);
    if (employee == nullptr)
    {
        throw Failure();
    }
    if (BumpGrade <= 0)
    {
        return;
    }
    
    employee->raiseGrade(BumpGrade);
    
    if (employee->getSalary() == 0)
    {
        employee->getMagicBox()->promoteEmployee(EmployeeID,BumpGrade,false);
        global_database->promoteEmployee(EmployeeID,BumpGrade,false);
    } 

    else
    {
        employee->getMagicBox()->promoteEmployee(EmployeeID,BumpGrade,true);
        global_database->promoteEmployee(EmployeeID,BumpGrade,true);
    }
 }

    long long DataBase::sumOfBumpGradeBetweenTopWorkersByGroup(const long long companyID,const long long m)
    {
        if (companyID == 0)
        {
            return global_database->getAllGradesAboveRank(m);
        }
        MagicBox* company = companies_database->find(companyID);
        return company->getAllGradesAboveRank(m);
    }
    double  DataBase::averageBumpGradeBetweenSalaryByGroup(const long long companyID,const long long LowerSalary,const long long HigherSalary)
    {
        if (companyID == 0)
        {
            return global_database->averageBumpGradeBetweenSalaryByGroup(LowerSalary,HigherSalary);
        }
        MagicBox* company = companies_database->find(companyID);
        return company->averageBumpGradeBetweenSalaryByGroup(LowerSalary,HigherSalary);
    }
    double DataBase::companyValue(const long long companyID)
    {
        return companies_database->findValue(companyID);
    }
    void DataBase::Quit()
    {
        global_database->freeMemory();
        companies_database->freeMemory();
        delete companies_database;
        delete global_database;
        companies_database = nullptr;
        global_database = nullptr;
    }
 