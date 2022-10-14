#include "library2.h"
#include "dataBase.h"
#include "compare_functors.h"

void *Init(int k)
{
    if (k <= 0)
    {
        return NULL;
    }
    
    try
    {
        CompareBySalary salary_cmp;
        DataBase* DS = new DataBase(k,salary_cmp);
        return (void*)DS;
    }
    
    catch(const std::bad_alloc&)
    {
        return NULL;
    }
    catch(const Failure&)
    {
        return NULL;
    }
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade)
{
    if (DS == NULL || employeeID <=0 || companyID <= 0 || grade < 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;

    if (realDS->getNumOfCompanies() < companyID)
    {
        return INVALID_INPUT;
    }
    
    try
    {
        realDS -> addEmployee(employeeID, companyID,grade);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int employeeID)
{
    if (DS == NULL || employeeID <=0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;

    try
    {
        realDS -> removeEmployee(employeeID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor)
{
    if (DS == NULL || companyID1 <=0 || companyID2 <= 0 || companyID1 == companyID2 || factor <=0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    if (realDS->getNumOfCompanies() < companyID1 || realDS->getNumOfCompanies() < companyID2)
    {
        return INVALID_INPUT;
    } 

    try
    {
        realDS -> acquireCompany(companyID1,companyID2,factor);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease)
{

    if (DS == NULL || employeeID <= 0 || salaryIncrease <= 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    try
    {
        realDS -> employeeSalaryIncrease(employeeID,salaryIncrease);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade)
{

    if (DS == NULL || employeeID <= 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    try
    {
        realDS -> promoteEmployee(employeeID,bumpGrade);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m)
{
    
    if (DS == NULL || companyID < 0 || m <= 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    if (realDS->getNumOfCompanies() < companyID)
    {
        return INVALID_INPUT;
    }

    long long standing = 0;
    try
    {
        standing = realDS -> sumOfBumpGradeBetweenTopWorkersByGroup(companyID,m);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", standing);
    //printf("SumOfBumpGradeBetweenTopWorkersByGroup: %1.f\n",floor(10 * standing + 0.5f) / 10 );
    return SUCCESS;
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary)
{

    if (DS == NULL || higherSalary < 0 || lowerSalary < 0 || lowerSalary > higherSalary || companyID < 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    if (realDS->getNumOfCompanies() < companyID)
    {
        return INVALID_INPUT;
    }

    double standing = 0;
    try
    {
        standing = realDS -> averageBumpGradeBetweenSalaryByGroup(companyID,lowerSalary,higherSalary);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return FAILURE;
    }
    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", standing);
    //printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", floor(10 * standing + 0.5f) / 10);
    return SUCCESS;
}

StatusType CompanyValue(void *DS, int companyID)
{

     if (DS == NULL || companyID <= 0)
    {
        return INVALID_INPUT;
    }

    DataBase* realDS = (DataBase*)DS;
    if (realDS->getNumOfCompanies() < companyID)
    {
        return INVALID_INPUT;
    }

    double standing = 0;
    try
    {
        standing = realDS -> companyValue(companyID);
    }
    catch(const std::bad_alloc&)
    {
        return ALLOCATION_ERROR;
    }
    catch(const Failure&)
    {
        return INVALID_INPUT;
    }
    printf("CompanyValue: %.1f\n", standing);
    //printf("CompanyValue: %.1f\n", floor(10 * standing + 0.5f) / 10);
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
        DataBase* realDS = (DataBase*)*DS;
        realDS -> Quit();
        delete realDS;
    }
    catch(const std::bad_alloc&)
    {
        return;
    }
    catch(const Failure&)
    {
        return;
    }
    *DS = NULL;
}
