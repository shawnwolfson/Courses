#include "employee.h"
#include "company.h"

int Employee::getSalary() const 
{
    return salary;
}
int Employee::getId() const 
{
    return id;
}
Company* Employee::getCompany() const
{
    return company;
}

int Employee::getGrade() const
{
    return grade;
}

int Employee::getCompanyId() const
{
    return company_id;
}

void Employee::setCompany(Company* company)
{
    this-> company = company;
    this->company_id = company->getId();
}