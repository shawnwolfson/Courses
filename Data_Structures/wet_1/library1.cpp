#include "library1.h"
#include "dataStructure.h"

#define MINIMUM_FACTOR 1.00
void *Init()
{
    try
    {
        CompareById<Employee> emp_id_cmp;
        CompareBySalary salary_cmp;
        CompareById<Company> comp_id_cmp;
        DataStructure* DS = new DataStructure(emp_id_cmp, salary_cmp, comp_id_cmp);
        return (void*)DS; 
    }
    catch(const std::bad_alloc&)
    {
        return NULL;
    }
}


StatusType AddCompany(void *DS, int CompanyID, int Value)
{
    if (DS == NULL || CompanyID <= 0 || Value <= 0)
    {
        return INVALID_INPUT;
    }
    
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->addCompany(CompanyID,Value);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeAlreadyExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    if (DS == NULL || CompanyID <= 0 || EmployeeID <= 0 || Salary <= 0 || Grade < 0)
        {
            return INVALID_INPUT;
        }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->addEmployee(EmployeeID, CompanyID, Salary, Grade);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeAlreadyExists&)
    {
        return FAILURE;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveCompany(void *DS, int CompanyID)
{
    if (DS == NULL || CompanyID <= 0)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->removeCompany(CompanyID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const HasEmployees&)
    {
        return FAILURE;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if (DS == NULL || EmployeeID <= 0)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->removeEmployee(EmployeeID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees)
{
    if (DS == NULL || CompanyID <= 0 || Value == NULL || NumEmployees == NULL)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        shared_ptr<Company> company = realDS->findCompany(CompanyID);
        if (!company)
        {
            return FAILURE;
        }
        *Value = company->getValue();
        *NumEmployees = company->getNumOfEmployees();
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    if (DS == NULL || EmployeeID <= 0 || EmployerID == NULL || Salary == NULL || Grade == NULL)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        shared_ptr<Employee> employee = realDS->getEmployee(EmployeeID);
        if (!employee)
        {
            return FAILURE;
        }
        *EmployerID = employee->getCompanyId();
        *Salary = employee->getSalary();
        *Grade = employee->getGrade();
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease)
{
    if (DS == NULL || CompanyID <= 0 || ValueIncrease <= 0)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->increaseCompanyValue(CompanyID,ValueIncrease);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch (const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    if (DS == NULL || EmployeeID <= 0 || SalaryIncrease <= 0)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->promoteEmployee(EmployeeID,SalaryIncrease,BumpGrade);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID)
{
    if (DS == NULL || EmployeeID <= 0 || NewCompanyID <= 0)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->hireEmployee(EmployeeID,NewCompanyID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    catch(const NodeAlreadyExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if (DS == NULL || AcquirerID <= 0 || TargetID <= 0 || TargetID == AcquirerID || Factor < MINIMUM_FACTOR)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->acquireCompany(AcquirerID,TargetID,Factor);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    catch(const NodeAlreadyExists&)
    {
        return FAILURE;
    }
    catch(const NotValuableEnough&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID)
{
    if (DS == NULL || CompanyID == 0 || EmployeeID == NULL)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->getHighestEarner(CompanyID,EmployeeID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    catch(const NoMatchingEmployee&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees)
{
    if (DS == NULL || CompanyID == 0 || NumOfEmployees == NULL || Employees == NULL)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->getAllEmployeesBySalary(CompanyID,Employees,NumOfEmployees);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    catch(const NoMatchingEmployee&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees)
{
    if (DS == NULL || NumOfCompanies <= 0 || Employees == NULL)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        realDS->getHighestEarnerInEachCompany(NumOfCompanies,Employees);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NotEnoughCompanies&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
            int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    if (DS == NULL || TotalNumOfEmployees == NULL || NumOfEmployees == NULL || CompanyID == 0 || 
            MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || MinEmployeeID > MaxEmployeeId)
    {
        return INVALID_INPUT;
    }
        
    try
    {
        DataStructure* realDS = (DataStructure*)DS;
        if (CompanyID > 0)
        {
            realDS->GetNumEmployeesMatchingInCompany(CompanyID,MinEmployeeID,MaxEmployeeId,MinSalary,MinGrade,
                                                        TotalNumOfEmployees,NumOfEmployees);
        }
        
        else
        {
            realDS->GetNumEmployeesMatchingGlobal(MinEmployeeID,MaxEmployeeId,MinSalary,MinGrade,
                                                        TotalNumOfEmployees,NumOfEmployees);
        }
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const NotEnoughEmployees&)
    {
        return FAILURE;
    }
    catch(const NodeNotExists&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void** DS)
{
    if (DS == NULL)
    {
        return;
    }
    try
    {
        DataStructure* realDS = (DataStructure*)*DS;
        realDS -> quit();
        delete realDS;
    }
    catch(const std::bad_alloc&)
    {
        return;
    }

    *DS = NULL;
}
