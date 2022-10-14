#include "employee.h"
#include "company.h"

long long Employee::getSalary() const 
{
    return salary;
}
long long Employee::getId() const 
{
    return id;
}
MagicBox* Employee::getMagicBox() const
{
    return magic_box;
}

long long Employee::getGrade() const
{
    return grade;
}


void Employee::setMagicBox(MagicBox* magic_box)
{
    this-> magic_box = magic_box;
}

void Employee::raiseSalary(const long long raise)
{
    salary += raise;
}
void Employee::raiseGrade(const long long raise)
{
    grade += raise;
}