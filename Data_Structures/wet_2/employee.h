#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_
#include <iostream>
#include <memory>
#include "node.h"

using std::shared_ptr;

class Company;
class MagicBox;

class Employee
{
    private:
        long long id;
        long long salary;
        long long grade;
        MagicBox* magic_box;

    public:
        Employee(long long id, long long grade, MagicBox* magic_box) : id(id), salary(0),grade(grade),
                                                                                    magic_box(magic_box){};
        long long getId() const;
        long long getSalary() const;
        long long  getGrade() const;
        void raiseSalary(const long long raise);
        void raiseGrade(const long long raise);
        void setMagicBox(MagicBox* MagicBox);

        MagicBox* getMagicBox() const;
};


#endif //_EMPLOYEE_H_