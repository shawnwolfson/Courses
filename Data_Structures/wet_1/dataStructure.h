#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_
#include <iostream>
#include "employee.h"
#include "treeData.h"
#include "compare_functors.h"
#include "company.h"
#include "Exceptions.h"

#define VALUE_FACTOR 10
#define NO_MATCHING_EMPLOYEE 0
class DataStructure
{
    private:
        Tree<Employee, CompareById<Employee>>* global_employees_tree_by_id;
        Tree<Employee, CompareBySalary>* global_employees_tree_by_salary;
        Tree<Company, CompareById<Company>>* all_companies_tree;
        Tree<Company, CompareById<Company>>* companies_with_employees_tree;
        int most_earning_employee_id;
        int most_earning_employee_salary;

    public:
        DataStructure(const CompareById<Employee>& employee_id_compare, const CompareBySalary& salary_compare,
                                                                const CompareById<Company>& company_id_compare) :
                                global_employees_tree_by_id(new Tree<Employee, CompareById<Employee>>(employee_id_compare)),
                                global_employees_tree_by_salary(new Tree<Employee, CompareBySalary>(salary_compare)),
                                all_companies_tree(new Tree<Company, CompareById<Company>>(company_id_compare)),
                                companies_with_employees_tree(new Tree<Company, CompareById<Company>>(company_id_compare)),
                                most_earning_employee_id(0), most_earning_employee_salary(0)
                                {};
        ~DataStructure()
        {
            delete global_employees_tree_by_id;
            delete global_employees_tree_by_salary;
            delete all_companies_tree;
            delete companies_with_employees_tree;
        }
        void addCompany(int id, int value);
        void addEmployee(const int EmployeeID,const int companyID,  const int salary, const int grad);

        void removeEmployee(const int EmployeeID);
        void removeCompany(const int CompanyID);

        shared_ptr<Company> findCompany(const int id) const;
        shared_ptr<Employee> getEmployee(const int id);

        void updateMostEarningEmployee();
        void setMostEarningEmployee(const int id, const int salary);

        void insertToGlobalEmployee(shared_ptr<Employee> emp);
        void removeFromGlobalEmployee(shared_ptr<Employee> emp);

        void increaseCompanyValue(const int companyID, const int valueIncrease);
        void promoteEmployee(const int EmployeeID,const int SalaryIncrease,const int BumpGrade);

        void hireEmployee(const int EmployeeID, const int NewCompanyID);
        void acquireCompany(const int AcquirerID,const int TargetID,const double Factor);
        void getHighestEarner(int CompanyID, int *EmployeeID) const;
        void getAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees);
        void getHighestEarnerInEachCompany(int NumOfCompanies, int **Employees);
        void GetNumEmployeesMatchingInCompany(int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary,
                                    int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees);
        void GetNumEmployeesMatchingGlobal(int MinEmployeeID, int MaxEmployeeId, int MinSalary,
                                    int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees);
        void quit();
};


#endif //_DATA_STRUCTURE_H_