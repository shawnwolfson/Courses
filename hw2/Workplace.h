#ifndef WORKPLACE_H_
#define WORKPLACE_H_
#include "exceptions.h"
#include "Manager.h"
#include <iostream>

namespace mtm {

/**
 * setCompareFunctionManager: A function object that tells the compiler how to order the Manager set in Workplace Class 
 *
 * @param manager1 - Manager pointer for first Manager
 * @param manager2 - Manager pointer for second Manager
 * @return
 *      true - if manager1's id is greater than manager2's id;
 *      false - if manager1's id is less than manager2's id; 
 */
class setCompareFunctionManager
{
public:
    bool operator()(const Manager* manager1, const Manager* manager2) const {
        return ((*manager1).getId() < (*manager2).getId());
    }
};


class Workplace 
{
    int id;
    std::string name;
    int employee_salary;
    int manager_salary;
    std::set<Manager*, setCompareFunctionManager> managers_group;

public:
    //C'tors, d'tor
    Workplace(int id, std::string name, int employee_salary, int manager_salary);
    Workplace(const Workplace& workplace) = default;
    ~Workplace() = default;
    
    //Getters
    int getId() const;
    std::string getName() const;
    int getWorkersSalary() const;
    int getManagersSalary() const;

    //Methods
    template<class T>
    void hireEmployee(T condition, Employee* employee, int manager_id);
    void hireManager(Manager* Manager);
    void fireEmployee(int employee_id, int manager_id);
    void fireManager(int id);

    //**Functions for internal use**//
 /**
 * emptyEmployeeGroup: A function that deletes all the Employees from a Manager's Employee set.
 *
 * @param manager_id - The id of the Manager that we need to clear his set of Employees.
 */   
    void emptyEmployeeGroup(int manager_id);

 /**
 * findManagerInWorkplace: A function that searches for a Manager in this Workplace.
 *
 * @param id - The id of the Manager that we need to find in the Workplace.
 * @return
 *      Manager* - returns a pointer to the Manager if the Manager exists in the Workplace.
 * @exception
 *      ManagerIsNotHired() - throws this exception if the Manager does not exist in the Workplace.
 */ 
    Manager* findManagerInWorkplace(int id);

 /**
 * changeSalaryForAllEmployeesUnderManager: If a Manager has Employees, this function updates the salary of his Employees, 
 *                                          in situations where the Manager is hired or fired from a Workplace.
 *
 * @param manager - The id of the Manager that the Employees work under.
 * @param wage - The wage that the Employees get in this Workplace.
 * @exception
 *      ManagerIsNotHired() - throws this exception if the Manager does not exist in the Workplace.
 */ 
    void changeSalaryForAllEmployeesUnderManager(Manager* manager, int wage);

/**
 * getManagersId: A function that gets all the id's of the Managers that work in this Workplace. Returns it in a form of a vector.
 * @return
 *      vector - Returns a vector of int's. The values in the vector are the requested id's.
 */
    std::vector<int> getManagersId() const;

    //class friends
    friend std::ostream& operator<<(std::ostream& os, const Workplace workplace);
};

std::ostream& operator<<(std::ostream& os, const Workplace workplace);

template<class T>
void Workplace::hireEmployee(T condition, Employee* employee, int manager_id)
{
    if(!(condition.operator()(employee)))
    {
        throw EmployeeNotSelected();
    }
    Manager* manager = findManagerInWorkplace(manager_id);
    manager->addEmployee(employee);
    employee->setSalary(employee_salary);
}

}
#endif /* WORKPLACE_H_ */