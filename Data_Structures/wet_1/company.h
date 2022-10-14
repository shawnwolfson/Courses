#ifndef _COMPANY_H_
#define _COMPANY_H_
#include <iostream>
#include "employee.h"
#include "treeData.h"
#include "compare_functors.h"
#include "Exceptions.h"
using std::shared_ptr;

class Company
{
    private:
        int id;
        int value;
        int num_of_employees;
        int most_earning_employee_id;
        int most_earning_employee_salary;
        Tree<Employee, CompareById<Employee>>* local_employees_tree_by_id;
        Tree<Employee, CompareBySalary>* local_employees_tree_by_salary;   

    public:
        Company(const int id, const int value,const CompareById<Employee>& compare_id,const CompareBySalary& compare_salary) :
                                                id(id),value(value),num_of_employees(0),most_earning_employee_id(0),
                                                most_earning_employee_salary(0),
                                                local_employees_tree_by_id(new Tree<Employee, CompareById<Employee>>(compare_id)),
                                                local_employees_tree_by_salary(new Tree<Employee, CompareBySalary>(compare_salary))
                                                {};  
        ~Company()
        {
            delete local_employees_tree_by_id;
            delete local_employees_tree_by_salary;
        } 
        int getId() const;
        int getValue() const;
        int getMostEarningID() const;
        int getMostEarningSalary() const;
        int getNumOfEmployees () const;
        Tree<Employee, CompareById<Employee>>* getIdTree();
        Tree<Employee, CompareBySalary>* getSalaryTree();  

        void setValue(const int value);
        void raiseValue(const int raise);
        void setNumOfEmployees(const int new_num_of_employees);

        void addEmployee(shared_ptr<Employee> emp);  
        void removeEmployee(const int id);

        void updateMostEarningEmployee();
        void setMostEarningEmployee(const int id, const int salary);

        void setPromotedEmployee(shared_ptr<Employee> employee);
        void setIdTree(Tree<Employee, CompareById<Employee>>* tree);
        void setSalaryTree(Tree<Employee,CompareBySalary>* tree);

        void destroyIdTree();
        void destroySalaryTree();

        void updateCompanyForEmployees();

        shared_ptr<Employee> getMaxEmployeeMatching(const int id) const;
        shared_ptr<Employee> getMinEmployeeMatching(const int id) const;   

        int getNumOfEmployeesMatchingInCompany(shared_ptr<Employee> start,shared_ptr<Employee> stop) const;
        shared_ptr<Employee>* fillNumOfEmployeesMatchingInCompany(shared_ptr<Employee> start,shared_ptr<Employee> stop, const int size);        
};



#endif //_COMPANY_H_