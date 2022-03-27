#include "Workplace.h"
#include "exceptions.h"

using namespace mtm;
using std::vector;
using std::ostream;
using std::string;
using std::set;
using std::endl;

//C'tor
Workplace::Workplace(int id, string name, int employee_salary, int manager_salary) :
    id(id), name(name), employee_salary(employee_salary), manager_salary(manager_salary)
{}

//Getters
int Workplace::getId() const
{
    return id;
}

string Workplace::getName() const
{
    return name;
}

int Workplace::getWorkersSalary() const
{
    return employee_salary;
}

int Workplace::getManagersSalary() const
{
    return manager_salary;
}

//Methods
void Workplace::hireManager(Manager* manager)
{
    if(managers_group.count(manager))
    {
        throw ManagerAlreadyHired();
    }
    if((manager->getSalary()) > 0)
    {
        throw CanNotHireManager();
    }
    changeSalaryForAllEmployeesUnderManager(manager, employee_salary);
    manager->setSalary(manager_salary);
    managers_group.insert(manager);
}

void Workplace::fireEmployee(int employee_id, int manager_id)
{
    Manager* manager = findManagerInWorkplace(manager_id);
    manager->setEmployeeSalary(employee_id, -employee_salary);
    manager->removeEmployee(employee_id);
}

void Workplace::fireManager(int id)
{
    Manager* manager = findManagerInWorkplace(id);
    changeSalaryForAllEmployeesUnderManager(manager, -employee_salary);
    manager->setSalary(-manager_salary);
    managers_group.erase(manager);
}

//Functions for internal use
vector<int> Workplace::getManagersId() const
{
    set<Manager*>::iterator iterator;
    vector<int> id_vector;
    for(iterator = managers_group.begin(); iterator != managers_group.end(); iterator++)
    {
      id_vector.push_back((*iterator)->getId());

    }
    return id_vector;
}

void Workplace::emptyEmployeeGroup(int manager_id)
{
    Manager* manager = findManagerInWorkplace(manager_id);
    vector<int> id_array = manager->getEmployeesId();
    for(unsigned int index = 0; index < id_array.size(); ++index)
    {
        fireEmployee(id_array[index], manager_id);
    }
}

Manager* Workplace::findManagerInWorkplace(int id)
{
    Manager temp_manager(id, " ", " ", 0);
    set<Manager*>::iterator managers_it;
    managers_it = managers_group.find(&temp_manager);
    if(managers_it == managers_group.end())
    {
        throw ManagerIsNotHired();
    }
    return (*managers_it);
}

void Workplace::changeSalaryForAllEmployeesUnderManager(Manager* manager, int wage)
{
    if(manager->hasEmployees())
    {
        vector<int> id_array = manager->getEmployeesId();
        for(unsigned int index = 0; index < id_array.size(); ++index)
        {
            manager->setEmployeeSalary(id_array[index], wage);
        }
    } 
}

//Printing function
ostream& mtm::operator<<(ostream& os, const Workplace workplace)
{
    set<Manager*>::iterator managers_it = workplace.managers_group.begin();
    if(managers_it == workplace.managers_group.end())
    {
        os << "Workplace name - " << workplace.getName() << endl;
        return os;
    }
    os << "Workplace name - " << workplace.getName() << " Groups:" << endl;
    for(; managers_it != workplace.managers_group.end(); ++managers_it)
    {
        os << "Manager ";
        (*managers_it)->printLong(os);
    }
    return os;
}
