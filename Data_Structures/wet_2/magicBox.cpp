#include "magicBox.h"

long long MagicBox::getNumOfEmployees() const 
{
    return num_of_employees;
}

long long MagicBox::getNumOfEmployeesWithOutSalary() const
{
    return num_of_employees_without_salary;
}

long long MagicBox::getSumOfGradesOfEmployessWithOutSalary() const
{
    return sum_of_grades_of_employees_without_salaries;
}

void MagicBox::raiseCompanyTreeSize(const long long new_num)
{
    company_reverse_tree_size += new_num;
}

void MagicBox::setReverseCompanyTree(shared_ptr<Node<Company>> company_root)
{
    company_reverse_tree = company_root;
}

long long MagicBox::getCompanyReverseTreeSize()
{
    return company_reverse_tree_size;
}

EmployeesHashTable* MagicBox::getEmployeeTable ()
{
    return employee_table;
}

void MagicBox::resetEmployeeTable()
{
    delete employee_table;
    employee_table = nullptr;
}

void MagicBox::setEmployeeTable(EmployeesHashTable* table)
{
    this->employee_table = table;
}

void MagicBox::setCompanyReverseTreeSize(const long long value)
{
    company_reverse_tree_size = value;
}
shared_ptr<Node<Company>> MagicBox::getRootOfReverseTree()
{
    return company_reverse_tree;
}

void MagicBox::resetReverseTree()
{
    company_reverse_tree = nullptr;
}

void MagicBox::raiseNumOfEmployees(const long long new_num_of_employees)
{
    num_of_employees += new_num_of_employees;
}
void MagicBox::raiseNumOfEmployeesWithOutSalary(const long long new_num_of_employees)
{
    num_of_employees_without_salary += new_num_of_employees;
}
void MagicBox::raiseSumOfGradesOfEmployessWithOutSalary(const long long new_sum)
{
    sum_of_grades_of_employees_without_salaries += new_sum;
}

static void raiseAllParam (MagicBox* acquire,  MagicBox* purchased)
{
    acquire->raiseCompanyTreeSize(purchased->getCompanyReverseTreeSize());
    acquire->raiseNumOfEmployees(purchased->getNumOfEmployees());
    acquire->raiseNumOfEmployeesWithOutSalary(purchased->getNumOfEmployeesWithOutSalary());
    acquire->raiseSumOfGradesOfEmployessWithOutSalary(purchased->getSumOfGradesOfEmployessWithOutSalary());
}


static Tree<Employee,CompareBySalary>* buildingEmptyEmployeeSalaryTree()
{
    CompareBySalary compare;
    Tree<Employee,CompareBySalary>* tree = new Tree<Employee,CompareBySalary>(compare);
    return tree;
}



void MagicBox::mergeEmployeeTrees(MagicBox* other)
{
    Tree<Employee, CompareBySalary>* merged_emp_tree = buildingEmptyEmployeeSalaryTree();
    merged_emp_tree -> mergeTrees(this->employees_with_salary,other->employees_with_salary);
    if (merged_emp_tree != nullptr)
    {
        delete this->employees_with_salary;
        this->employees_with_salary = merged_emp_tree;
    }
}


void MagicBox::updateMagicBoxForEmployee()
{
    employee_table->updateMagicBox(this);
}



//acquire smaller than purchased (B <= A)
MagicBox* merge_acquire_to_purchased(MagicBox* acquirer,  MagicBox* purchased, double factor, double purchased_old_real_value)
{
    acquirer->getRootOfReverseTree()->insertFather(purchased->getRootOfReverseTree());
    acquirer->getRootOfReverseTree()->resetMagicBox();

    double purchased_old_relative_value = purchased->getRootOfReverseTree()->getData()->getRelativeValue();
    acquirer->getRootOfReverseTree()->getData()->
                        raiseRelativeValue(purchased_old_real_value*factor - purchased_old_relative_value);
    
    purchased->mergeEmployeeTrees(acquirer);
    acquirer->resetReverseTree();

    EmployeesHashTable* temp = mergeHashTables(acquirer->getEmployeeTable(),purchased->getEmployeeTable());
    acquirer->resetEmployeeTable();
    purchased->resetEmployeeTable();
    purchased->setEmployeeTable(temp);

    purchased->updateMagicBoxForEmployee();
    purchased->setRealValue(purchased->getRealValue()*factor+acquirer->getRealValue());

    raiseAllParam(purchased,acquirer);
    
    return purchased;
}



//acquire smaller than purchased (B > A)
MagicBox* merge_purchased_to_acquirer(MagicBox* acquirer,  MagicBox* purchased, double factor, double purchased_old_real_value)
{
    purchased->getRootOfReverseTree()->insertFather(acquirer->getRootOfReverseTree());
    purchased->getRootOfReverseTree()->resetMagicBox();
    acquirer->getRootOfReverseTree()->getData()->
                        raiseRelativeValue(purchased_old_real_value*factor);
    
    double acquirer_new_relative_value = acquirer->getRootOfReverseTree()->getData()->getRelativeValue();
    purchased->getRootOfReverseTree()->getData()->raiseRelativeValue(-acquirer_new_relative_value);
    
    acquirer->mergeEmployeeTrees(purchased);
    purchased->resetReverseTree();

    EmployeesHashTable* temp = mergeHashTables(acquirer->getEmployeeTable(),purchased->getEmployeeTable());
    acquirer->resetEmployeeTable();
    purchased->resetEmployeeTable();
    acquirer->setEmployeeTable(temp);

    acquirer->updateMagicBoxForEmployee();
    raiseAllParam(acquirer,purchased);
    acquirer->raiseRealValue(purchased->getRealValue()*factor);
 
    return acquirer;
}


void MagicBox::addEmployee(shared_ptr<Employee> employee)
{
    employee_table->addEmployee(employee);
    num_of_employees++;
    num_of_employees_without_salary++;
    sum_of_grades_of_employees_without_salaries += employee->getGrade();
}

void MagicBox::removeEmployee(const long long id)
{
    shared_ptr<Employee> temp = employee_table->getEmployee(id);
    if (temp == nullptr)
    {
        throw Failure();
    }
    
    employee_table->removeEmployee(id);
    num_of_employees--;

    if (temp->getSalary() > 0)
    {
        employees_with_salary->remove(temp);
    }

    else
    {
        num_of_employees_without_salary--;
        sum_of_grades_of_employees_without_salaries -= temp->getGrade();
    }
    
}

shared_ptr<Employee> MagicBox::getEmployee(const long long id)
{
    return employee_table->getEmployee(id);
}

long long MagicBox::getAllGradesAboveRank(long long m)
{
    long long rank = (employees_with_salary->getSize()-m);
    if (rank < 0 )
    {
        throw Failure();
    }
    return employees_with_salary->getSumOfGradesAboveNodeInRank(rank);
}

void MagicBox::salaryIncrease(const long long id, bool hadSalary)
{
    shared_ptr<Employee> employee = employee_table->getEmployee(id);
    if (employee == nullptr || id <= 0)
    {
        throw Failure();
    }

    if (!hadSalary)
    {
        employees_with_salary->insert(employee);
        num_of_employees_without_salary--;
        sum_of_grades_of_employees_without_salaries -= employee->getGrade();
    }
}

void MagicBox::promoteEmployee(const long long id,const long long bump_grade, bool hadSalary)
{
    shared_ptr<Employee> employee = employee_table->getEmployee(id);
    if (employee == nullptr)
    {
        throw Failure();
    }

    if (!hadSalary)
    {
        sum_of_grades_of_employees_without_salaries += bump_grade;
    }
    else
    {
        employees_with_salary->remove(employee);
        employees_with_salary->insert(employee);
    }
}

double MagicBox::averageBumpGradeBetweenSalaryByGroup (const long long lowerSalary, const long long higherSalary)
{
    double avg_sum = 0;
    if (lowerSalary == 0)
    {
        shared_ptr<Employee> higher_salary_employee = employees_with_salary->improved_find_infimum_of_node_in_tree(higherSalary);


        if (higher_salary_employee == nullptr)
        {
            if (num_of_employees_without_salary == 0)
            {
                throw Failure();
            }
            return (sum_of_grades_of_employees_without_salaries / num_of_employees_without_salary);
        }
 
        long long higher_salary_employee_rank = employees_with_salary->getRankOfEmployee(higher_salary_employee);
        double sum_above_higher_salary = employees_with_salary->getSumOfGradesAboveNodeInRank(higher_salary_employee_rank);
        double sum_above_lower_salary = employees_with_salary->getSumOfGradesAboveNodeInRank(ALL_EMPLOYEES);

        double sum_of_employees_with_salary = sum_above_lower_salary - sum_above_higher_salary;
        double sum = sum_of_employees_with_salary + sum_of_grades_of_employees_without_salaries;
        avg_sum = sum / (higher_salary_employee_rank + num_of_employees_without_salary);
    
    }

    else
    {
        shared_ptr<Employee> lower_salary_employee = employees_with_salary->improved_find_supremum_of_node_in_tree(lowerSalary);
        shared_ptr<Employee> higher_salary_employee = employees_with_salary->improved_find_infimum_of_node_in_tree(higherSalary);
        if (lower_salary_employee == nullptr || higher_salary_employee == nullptr)
        {
            throw Failure();
        }
        
        long long higher_salary_employee_rank = employees_with_salary->getRankOfEmployee(higher_salary_employee);
        long long lower_salary_employee_rank = employees_with_salary->getRankOfEmployee(lower_salary_employee);
        double sum_above_higher_salary = employees_with_salary->getSumOfGradesAboveNodeInRank(higher_salary_employee_rank);
        double sum_above_lower_salary = employees_with_salary->getSumOfGradesAboveNodeInRank(lower_salary_employee_rank-1);

        double sum = sum_above_lower_salary - sum_above_higher_salary;
        
        
        avg_sum = sum / (higher_salary_employee_rank - lower_salary_employee_rank + 1);
    

    }

    return avg_sum;
}

void MagicBox::freeMemory()
{
    company_reverse_tree = nullptr;
    delete employee_table;
    delete employees_with_salary;
    employee_table = nullptr;
    employees_with_salary = nullptr;
}
double MagicBox::getRealValue()
{
    return real_value;
}

void MagicBox::setRealValue(const double val)
{
    real_value = val;
}
void MagicBox::raiseRealValue(const double raise)
{
    real_value += raise;
}