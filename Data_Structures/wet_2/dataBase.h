#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_
#include <iostream>
#include "employee.h"
#include "rankTree.h"
#include "compare_functors.h"
#include "company.h"
#include "Exceptions.h"
#include "UnionFind.h"
#include "magicBox.h"
#include "HashTable.h"

class DataBase
{
    private:
        UnionFind* companies_database;
        MagicBox* global_database;
        long long num_of_companies;

    public:
        DataBase(const long long num_of_companies, CompareBySalary& cmp) : companies_database(new UnionFind(num_of_companies)) , global_database(new MagicBox(cmp)),
                                                                            num_of_companies(num_of_companies){};
        ~DataBase()
        {
            delete companies_database;
            delete global_database;
        }
        
        void addEmployee(const long long EmployeeID,const long long companyID, const long long grade);
        void removeEmployee(const long long EmployeeID);
        void acquireCompany(const long long AquirerID, const long long TargetID, double Factor);
        void employeeSalaryIncrease(const long long EmployeeID, const long long SalaryIncrease);
        void promoteEmployee(const long long EmployeeID,const long long BumpGrade);
        long long sumOfBumpGradeBetweenTopWorkersByGroup(const long long companyID,const long long m);
        double averageBumpGradeBetweenSalaryByGroup(const long long companyID,const long long LowerSalary,const long long HigherSalary);
        double companyValue(const long long companyID);
        void Quit();
        long long getNumOfCompanies() const;

};


#endif //_DATA_STRUCTURE_H_