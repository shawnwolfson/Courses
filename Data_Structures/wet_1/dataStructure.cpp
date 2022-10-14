#include "dataStructure.h"

void DataStructure::insertToGlobalEmployee(shared_ptr<Employee> emp)
{
    global_employees_tree_by_id->insert(emp);
    global_employees_tree_by_salary->insert(emp);
}
void DataStructure::removeFromGlobalEmployee(shared_ptr<Employee> emp)
{
    global_employees_tree_by_id->remove(emp);
    global_employees_tree_by_salary->remove(emp);
}

shared_ptr<Company> DataStructure::findCompany(const int id) const
{
    
    return all_companies_tree->findWithId(id);
}

shared_ptr<Employee> DataStructure::getEmployee(const int id)
{
    return global_employees_tree_by_id->findWithId(id);
}

static shared_ptr<Company> createCompany (const int id, const int value)
{
    CompareById<Employee> compare_emp_id;
    CompareBySalary compare_emp_sal;
    shared_ptr<Company> temp(new Company(id,value,compare_emp_id,compare_emp_sal)); 
    return temp;
}

void DataStructure::setMostEarningEmployee(const int id, const int salary)
{
    most_earning_employee_salary = salary;
    most_earning_employee_id = id;
}

void DataStructure::updateMostEarningEmployee()
{
    if (global_employees_tree_by_id->getSize() == 0)
    {
        setMostEarningEmployee(0,0);
        return;
    }
    shared_ptr<Node<Employee>> temp = global_employees_tree_by_salary->getMaximumNode(global_employees_tree_by_salary->getHead());
    shared_ptr<Employee> highest_earner = temp->getData();
    setMostEarningEmployee(highest_earner->getId(),highest_earner->getSalary());
}

void DataStructure::addCompany(int id, int value)
{
    if (all_companies_tree->findWithId(id) != nullptr)
    {
        throw NodeAlreadyExists();
    }
    all_companies_tree->insert(createCompany(id,value));
}

static shared_ptr<Employee> createEmployee(const int EmployeeID, const int companyID, const int salary, const int grade, Company* company)
{
    shared_ptr<Employee> emp(new Employee(EmployeeID, companyID, salary, grade, company));
    return emp;
}

void DataStructure::addEmployee(const int EmployeeID, const int companyID,  const int salary, const int grade)
{
    if (global_employees_tree_by_id->findWithId(EmployeeID) != nullptr)
    {
        throw NodeAlreadyExists();
    }
    shared_ptr<Company> company = all_companies_tree->findWithId(companyID);

    if (company == nullptr)
    {
        throw NodeNotExists();
    }
    
    if (company->getNumOfEmployees() == 0)
    {
        companies_with_employees_tree->insert(company);
    }

    shared_ptr<Employee> emp = createEmployee(EmployeeID,companyID,salary,grade,company.get());
    insertToGlobalEmployee(emp);
    company->addEmployee(emp);

    if (salary > most_earning_employee_salary)
    {
        updateMostEarningEmployee();
    }
    if (salary == most_earning_employee_salary && EmployeeID < most_earning_employee_id)
    {
        updateMostEarningEmployee();
    }
}

void DataStructure::removeEmployee(const int EmployeeID)
{
    shared_ptr<Employee> employee_to_delete = global_employees_tree_by_id->findWithId(EmployeeID);
    if (employee_to_delete == nullptr)
    {
        throw NodeNotExists();
    }
 
    Company* company = employee_to_delete->getCompany();
    removeFromGlobalEmployee(employee_to_delete);
    company->removeEmployee(EmployeeID);
    if (EmployeeID == most_earning_employee_id)
    {
        updateMostEarningEmployee();
    }

    if (company ->getNumOfEmployees() == 0)
    {
        shared_ptr<Company> company_to_remove = findCompany(company->getId());
        companies_with_employees_tree->remove(company_to_remove);
    }
}

void DataStructure::removeCompany(const int CompanyID)
{
    shared_ptr<Company> company_to_delete = findCompany(CompanyID);
    if(company_to_delete == nullptr)
    {
        throw NodeNotExists();
    }
    else if(company_to_delete->getNumOfEmployees() > 0)
    {
        throw HasEmployees();
    }

    all_companies_tree->remove(company_to_delete);
}

void DataStructure::increaseCompanyValue(const int companyID, const int valueIncrease)
{
    shared_ptr<Company> company_to_modify = findCompany(companyID);
    if(company_to_modify == nullptr)
    {
        throw NodeNotExists();
    }
    company_to_modify->raiseValue(valueIncrease);
}

void DataStructure::promoteEmployee(const int EmployeeID,const int SalaryIncrease,const int BumpGrade)
{
    shared_ptr<Employee> emp_to_promote = global_employees_tree_by_id->findWithId(EmployeeID);
    if(emp_to_promote == nullptr)
    {
        throw NodeNotExists();
    }
    int companyID = emp_to_promote->getCompanyId();
    int salary = emp_to_promote->getSalary() + SalaryIncrease;
    int grade = emp_to_promote->getGrade();
    Company* company = emp_to_promote->getCompany();
    if (BumpGrade > 0)
    {
        grade++;
    }
    shared_ptr<Employee> raised_emp = createEmployee(EmployeeID,companyID,salary,grade,company);
    
    removeFromGlobalEmployee(emp_to_promote);
    insertToGlobalEmployee(raised_emp);

    company->setPromotedEmployee(raised_emp);
    if (raised_emp->getSalary() > most_earning_employee_salary)
    {
        updateMostEarningEmployee();
    }
    else if (raised_emp->getSalary() == most_earning_employee_salary && EmployeeID < most_earning_employee_id)
    {
        updateMostEarningEmployee();
    }
}

void DataStructure::hireEmployee(const int EmployeeID, const int NewCompanyID)
{
    shared_ptr<Employee> emp = global_employees_tree_by_id->findWithId(EmployeeID);
    shared_ptr<Company> company = all_companies_tree->findWithId(NewCompanyID);
    if (!emp || !company)
    {
        throw NodeNotExists();
    }
    if (emp->getCompanyId() == NewCompanyID)
    {
        throw NodeAlreadyExists();
    }
    
    int grade = emp->getGrade();
    int salary = emp->getSalary();
    removeEmployee(EmployeeID);
    addEmployee(EmployeeID, NewCompanyID, salary, grade);
}

static Tree<Employee,CompareById<Employee>>* buildingEmptyEmployeeIDTree()
{
    CompareById<Employee> compare;
    Tree<Employee,CompareById<Employee>>* tree = new Tree<Employee,CompareById<Employee>>(compare);
    return tree;
}


static Tree<Employee,CompareBySalary>* buildingEmptyEmployeeSalaryTree()
{
    CompareBySalary compare;
    Tree<Employee,CompareBySalary>* tree = new Tree<Employee,CompareBySalary>(compare);
    return tree;
}

void DataStructure::acquireCompany(const int AcquirerID,const int TargetID,const double Factor)
{
    shared_ptr<Company> acquirer_company = findCompany(AcquirerID);
    shared_ptr<Company> target_company = findCompany(TargetID);
    if (acquirer_company == nullptr || target_company == nullptr)
    {
        throw NodeNotExists();
    }

    int acquirer_val = acquirer_company->getValue();
    int target_val = target_company->getValue();
    if(acquirer_val < (VALUE_FACTOR * target_val))
    {
        throw NotValuableEnough();
    }
    
    int new_value = (acquirer_val + target_val)  * Factor;

    Tree<Employee, CompareById<Employee>>* merged_emp_tree_by_id = buildingEmptyEmployeeIDTree();
    Tree<Employee, CompareBySalary>* merged_emp_tree_by_salary = buildingEmptyEmployeeSalaryTree();

    merged_emp_tree_by_id->mergeTrees(acquirer_company->getIdTree(),target_company->getIdTree());
    merged_emp_tree_by_salary->mergeTrees(acquirer_company->getSalaryTree(),target_company->getSalaryTree());


    target_company->destroyIdTree();
    target_company->destroySalaryTree();

    companies_with_employees_tree->remove(target_company);
    all_companies_tree->remove(target_company);
    
    acquirer_company->setIdTree(merged_emp_tree_by_id);
    acquirer_company->setSalaryTree(merged_emp_tree_by_salary);
    acquirer_company->setValue(new_value);

    acquirer_company->updateCompanyForEmployees();
    shared_ptr<Company> temp = companies_with_employees_tree->findWithId(AcquirerID);
    if(!temp && acquirer_company->getNumOfEmployees() != 0)
    {
        companies_with_employees_tree->insert(acquirer_company);
    }
}

void DataStructure::getHighestEarner(int CompanyID, int *EmployeeID) const
{
    if(CompanyID > 0 )
    {
        shared_ptr<Company> company = findCompany(CompanyID);
        if(!company)
        {
            throw NodeNotExists();
        }
        int most_earning_employee = company->getMostEarningID();
        if(most_earning_employee == NO_MATCHING_EMPLOYEE)
        {
            throw NoMatchingEmployee();
        }
        *(EmployeeID) = most_earning_employee; 
    }
    else if(CompanyID < 0)
    {
        if(most_earning_employee_id == NO_MATCHING_EMPLOYEE)
        {
            throw NoMatchingEmployee();
        }
        *(EmployeeID) = most_earning_employee_id;
    }
}

void DataStructure::getAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees)
{
    if(CompanyID > 0 )
    {
        shared_ptr<Company> company = findCompany(CompanyID);
        if(!company)
        {
            throw NodeNotExists();
        }
        Tree<Employee, CompareBySalary>* emp_by_salary_tree = company->getSalaryTree();
        int size = emp_by_salary_tree->getSize();
        if (size == 0)
        {
            throw NoMatchingEmployee();
        }
        shared_ptr<Employee>* employees_arr = emp_by_salary_tree->allTreeTravel(REVERSE_INORDER);
        int* temp = (int*)malloc(sizeof(int) * size);
        for(int index = 0; index < size; index++)
        {
            temp[index] = employees_arr[index]->getId();
        }
        *(NumOfEmployees) = size;
        *(Employees) = temp;
        delete[] employees_arr;
    }
    else if(CompanyID < 0)
    {
        int size = global_employees_tree_by_salary->getSize();
        if (size == 0)
        {
            throw NoMatchingEmployee();
        }
        shared_ptr<Employee>* employees_arr = global_employees_tree_by_salary->allTreeTravel(REVERSE_INORDER);
        int* temp = (int*)malloc(sizeof(int) * size);
        for(int index = 0; index < size; index++)
        {
            temp[index] = employees_arr[index]->getId();
        }
        *(NumOfEmployees) = size;
        *(Employees) = temp;
        delete[] employees_arr;
    }
}

void DataStructure::getHighestEarnerInEachCompany(int NumOfCompanies, int **Employees)
{
    int number_of_companies_with_at_least_one_employee = companies_with_employees_tree->getSize();
    if(number_of_companies_with_at_least_one_employee < NumOfCompanies)
    {
        throw NotEnoughCompanies();
    }
    shared_ptr<Company>* companies_arr = companies_with_employees_tree->allTreeTravel(LIMITED_INORDER, NumOfCompanies);
    int* temp = (int*)malloc(sizeof(int) * NumOfCompanies);
    for(int index = 0; index < NumOfCompanies; index++)
    {
        temp[index] = companies_arr[index]->getMostEarningID();
    }
    *(Employees) = temp;
    delete[] companies_arr;
}

void DataStructure::GetNumEmployeesMatchingInCompany(int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary,
                                            int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    shared_ptr<Company> company = findCompany(CompanyID);
    if(!company)
    {
        throw NodeNotExists();
    }
    else if(company->getNumOfEmployees() == 0)
    {
        throw NotEnoughEmployees();
    }
    shared_ptr<Employee> minimum_employee_in_company = company->getMinEmployeeMatching(MinEmployeeID);
    shared_ptr<Employee> maximum_employee_in_company = company->getMaxEmployeeMatching(MaxEmployeeId);

    if (minimum_employee_in_company == nullptr || maximum_employee_in_company == nullptr)
    {
        *TotalNumOfEmployees = 0;
        *NumOfEmployees = 0;
        return;
    }
    
    int total_num_of_employees = company->getNumOfEmployeesMatchingInCompany(minimum_employee_in_company, 
                                                                            maximum_employee_in_company);
    *TotalNumOfEmployees = total_num_of_employees;

    shared_ptr<Employee>* employees_arr = company->fillNumOfEmployeesMatchingInCompany(minimum_employee_in_company,
                                                                 maximum_employee_in_company, total_num_of_employees);
    int num_of_employees = 0;
    for(int index = 0; index < total_num_of_employees; index++)
    {
        if((employees_arr[index]->getSalary() >= MinSalary) && (employees_arr[index]->getGrade() >= MinGrade))
        {
            num_of_employees++;
        }
    }
    *NumOfEmployees = num_of_employees;
    delete[] employees_arr;
}

void DataStructure::GetNumEmployeesMatchingGlobal(int MinEmployeeID, int MaxEmployeeId, int MinSalary,
                                int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    if(global_employees_tree_by_id->getSize() == 0)
    {
        throw NotEnoughEmployees();
    }
    shared_ptr<Employee> minimum_employee = global_employees_tree_by_id->find_supremum_of_node_in_tree(MinEmployeeID);
    shared_ptr<Employee> maximum_employee = global_employees_tree_by_id->find_infimum_of_node_in_tree(MaxEmployeeId);
    if (minimum_employee == nullptr || maximum_employee == nullptr)
    {
        *TotalNumOfEmployees = 0;
        *NumOfEmployees = 0;
        return;
    }

    int total_num_of_employees = global_employees_tree_by_id->countSubTreeTravel(minimum_employee, 
                                                                            maximum_employee);
    *TotalNumOfEmployees = total_num_of_employees;

    shared_ptr<Employee>* employees_arr = global_employees_tree_by_id->fillSubTreeTravel(minimum_employee,
                                                                 maximum_employee, total_num_of_employees);
    int num_of_employees = 0;
    for(int index = 0; index < total_num_of_employees; index++)
    {
        if((employees_arr[index]->getSalary() >= MinSalary) && (employees_arr[index]->getGrade() >= MinGrade))
        {
            num_of_employees++;
        }
    }
    *NumOfEmployees = num_of_employees;
    delete[] employees_arr;
}

void DataStructure::quit()
{
    shared_ptr<Employee>* all_employees_arr = global_employees_tree_by_id->allTreeTravel(INORDER);
    int size = global_employees_tree_by_id->getSize();
    for (int i=0; i<size ;i++)
    {
        Company* company_to_delete = all_employees_arr[i]->getCompany();
        company_to_delete->destroyIdTree();
        company_to_delete->destroySalaryTree();
    }
    global_employees_tree_by_id->destory();
    global_employees_tree_by_salary->destory();
    companies_with_employees_tree->destory();
    all_companies_tree->destory();
    delete[] all_employees_arr;
}
