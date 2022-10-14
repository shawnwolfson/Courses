#ifndef _MAGIC_H_
#define _MAGIC_H_
#include <iostream>
#include <string>
#include "rankTree.h"
#include "node.h"
#include <memory>
#include "employee.h"
#include "compare_functors.h"
#include "HashTable.h"
#include "company.h"

#define ALL_EMPLOYEES 0

class MagicBox
{
        long long company_reverse_tree_size;
        long long num_of_employees;
        long long num_of_employees_without_salary;
        double sum_of_grades_of_employees_without_salaries;
        Tree<Employee,CompareBySalary>* employees_with_salary;
        shared_ptr<Node<Company>> company_reverse_tree;
        EmployeesHashTable* employee_table;
        double real_value;
        
        public:
            MagicBox(CompareBySalary& compare_func, double real_value=0) :  company_reverse_tree_size(1),num_of_employees(0),
                num_of_employees_without_salary(0), sum_of_grades_of_employees_without_salaries(0),
                employees_with_salary(new Tree<Employee,CompareBySalary>(compare_func)),company_reverse_tree(nullptr),employee_table(new EmployeesHashTable),
                real_value(real_value) {};

            ~MagicBox()
            {
                delete employee_table;
                delete employees_with_salary;
            }
            void setReverseCompanyTree(shared_ptr<Node<Company>> company_root);
            long long getCompanyReverseTreeSize();
            void setCompanyReverseTreeSize(const long long value);
            shared_ptr<Node<Company>> getRootOfReverseTree();
            void resetReverseTree();
            EmployeesHashTable* getEmployeeTable ();
            void resetEmployeeTable();
            void setEmployeeTable(EmployeesHashTable* table);

            long long getNumOfEmployees() const;
            long long getNumOfEmployeesWithOutSalary() const;
            long long getSumOfGradesOfEmployessWithOutSalary() const;
            
            void raiseCompanyTreeSize(const long long new_num);
            void raiseNumOfEmployees(const long long new_num_of_employees);
            void raiseNumOfEmployeesWithOutSalary(const long long new_num_of_employees_without_salary);
            void raiseSumOfGradesOfEmployessWithOutSalary(const long long new_sum);


            void mergeEmployeeTrees(MagicBox* other);
            void mergeHashTrees(MagicBox* other);
            void updateMagicBoxForEmployee();
            void addEmployee(shared_ptr<Employee> employee);
            void removeEmployee(const long long id);
            shared_ptr<Employee> getEmployee(const long long id);
            long long  getAllGradesAboveRank(long long rank);
            void salaryIncrease(const long long id, bool hadSalary);
            void promoteEmployee(const long long id,const long long bump_grade, bool hadSalary);
            double averageBumpGradeBetweenSalaryByGroup (const long long lowerSalary, const long long higherSalary);
            void freeMemory();
            double getRealValue();
            void raiseRealValue(const double raise);
            void setRealValue(const double val);

            Tree<Employee,CompareBySalary>* getTree ()
            {
                return employees_with_salary;
            }

};

MagicBox* merge_acquire_to_purchased(MagicBox* acquirer,  MagicBox* purchased, double factor, double purchased_old_real_value);
MagicBox* merge_purchased_to_acquirer(MagicBox* acquirer,  MagicBox* purchased, double factor, double purchased_old_real_value);

#endif //_MAGIC_H_