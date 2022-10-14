#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_
#include <iostream>
#include <memory>
#include "node.h"

using std::shared_ptr;

class Company;

class Employee
{
    private:
        int id;
        int company_id;
        int salary;
        int grade;
        Company* company;

    public:
        Employee(int id, int company_id, int salary, int grade, Company* company) : id(id),company_id(company_id),
                                                                                    salary(salary),grade(grade),
                                                                                    company(company){};
        int getId() const;
        int getCompanyId() const;
        int getSalary() const;
        int getGrade() const;

        void setCompany(Company* company);

        Company* getCompany() const;
};

class EmployeeLocal : public Employee
{
    Employee* global;
};


#endif //_EMPLOYEE_H_