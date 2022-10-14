#include "company.h"
int Company::getId() const
{
    return id;
}

int Company::getValue() const
{
    return value;
}

 int Company::getMostEarningID() const
 {
     return most_earning_employee_id;
 }

int Company::getMostEarningSalary() const
{
    return most_earning_employee_salary;
}

int Company::getNumOfEmployees () const
{
    return num_of_employees;
}
void Company::raiseValue(const int raise)
{
    value += raise;
}
void Company::setValue(const int value)
{
    this->value = value;
}
void Company::setMostEarningEmployee(const int id, const int salary)
{
    most_earning_employee_salary = salary;
    most_earning_employee_id = id;
}

void Company::updateMostEarningEmployee()
{
    if (num_of_employees == 0)
    {
        setMostEarningEmployee(0,0);
        return;
    }
    shared_ptr<Node<Employee>> temp = local_employees_tree_by_salary->getMaximumNode(local_employees_tree_by_salary->getHead());
    shared_ptr<Employee> highest_earner = temp->getData();
    setMostEarningEmployee(highest_earner->getId(),highest_earner->getSalary());
}

void Company::addEmployee(shared_ptr<Employee> emp)
{
    if (local_employees_tree_by_id->findWithId(emp->getId()) != nullptr)
    {
        throw NodeAlreadyExists();
    }
    local_employees_tree_by_id->insert(emp);
    local_employees_tree_by_salary->insert(emp);
    num_of_employees++;

    if (emp->getSalary() > most_earning_employee_salary)
    {
        updateMostEarningEmployee();
    }
    else if (emp->getSalary() == most_earning_employee_salary && emp->getId() < most_earning_employee_id)
    {
        updateMostEarningEmployee();
    }

}


void Company::removeEmployee(const int id)
{
    shared_ptr<Employee> temp = local_employees_tree_by_id->findWithId(id);
    if(temp == nullptr)
    {
        throw NodeNotExists();
    }
    local_employees_tree_by_id->remove(temp);
    local_employees_tree_by_salary->remove(temp);
    num_of_employees--;
    if (id == most_earning_employee_id)
    {
        updateMostEarningEmployee();
    }
}

void Company::setNumOfEmployees(const int new_num_of_employees)
{
    num_of_employees = new_num_of_employees;
}

void Company::setPromotedEmployee(shared_ptr<Employee> employee)
{
    this->removeEmployee(employee->getId());
    this->addEmployee(employee);
}

Tree<Employee, CompareById<Employee>>* Company::getIdTree()
{
    return local_employees_tree_by_id;
}
Tree<Employee, CompareBySalary>* Company::getSalaryTree()
{
    return local_employees_tree_by_salary;
}
void Company::destroyIdTree()
{
    local_employees_tree_by_id->destory();
    num_of_employees = 0;
}

void Company::destroySalaryTree()
{
    local_employees_tree_by_salary->destory();
    num_of_employees = 0;
}

void Company::setIdTree(Tree<Employee, CompareById<Employee>>* tree)
{
    this->destroyIdTree();
    delete local_employees_tree_by_id;
    local_employees_tree_by_id = tree;
    num_of_employees = tree->getSize();
}
void Company::setSalaryTree(Tree<Employee,CompareBySalary>* tree)
{
    this->destroySalaryTree();
    delete local_employees_tree_by_salary;
    local_employees_tree_by_salary = tree;
    num_of_employees = tree->getSize();
    updateMostEarningEmployee();
} 

void Company::updateCompanyForEmployees()
{
    shared_ptr<Employee>* employees = local_employees_tree_by_id->allTreeTravel(INORDER);
    for(int index = 0; index < local_employees_tree_by_id->getSize(); index++)
    {
        employees[index]->setCompany(this);
    }
    delete[] employees;
}

shared_ptr<Employee> Company::getMaxEmployeeMatching(const int id) const
{
    return local_employees_tree_by_id->find_infimum_of_node_in_tree(id);
}
shared_ptr<Employee> Company::getMinEmployeeMatching(const int id) const
{
    return local_employees_tree_by_id->find_supremum_of_node_in_tree(id);
}


int Company::getNumOfEmployeesMatchingInCompany(shared_ptr<Employee> start,shared_ptr<Employee> stop) const
{
    return local_employees_tree_by_id->countSubTreeTravel(start, stop);
}

shared_ptr<Employee>* Company::fillNumOfEmployeesMatchingInCompany(shared_ptr<Employee> start,shared_ptr<Employee> stop, const int size)
{
    return local_employees_tree_by_id->fillSubTreeTravel(start, stop, size);
}
