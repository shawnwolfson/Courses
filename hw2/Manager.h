#ifndef MANAGER_H_
#define MANAGER_H_

#include "Citizen.h"
#include "Employee.h"
#include <set>
#include <iostream>
#include <vector>

namespace mtm {

/**
 * setCompareFunctionEmployee: A function object that tells the compiler how to order the Employee set in Manager Class 
 *
 * @param employee1 - Employee pointer for first Citizen
 * @param employee2 - Employee pointer for second Citizen
 * @return
 *      true - if employee1's id is greater than employee1's id;
 *      false - if employee2's id is less than employee2's id; 
 */
class setCompareFunctionEmployee
{
public:
    bool operator()(const Employee* employee1, const Employee* employee2) const {
        return ((*employee1).getId() < (*employee2).getId());
    }
};


class Manager : public Citizen
{
    int salary;
    std::set<Employee*, setCompareFunctionEmployee> employees_group;

public:
    //C'tors, d'tor
    Manager(int id, std::string first_name, std::string last_name, int birth_year);
    Manager(const Manager& manager) = default;
    ~Manager() = default;

    //Methods
    void addEmployee(Employee* employee);
    void removeEmployee(int id);

    //Getters and setters
    int getSalary() const;
    void setSalary(int wage);

    //**Functions for internal use**//
/**
 * setEmployeeSalary: A function that allows the user to set(add or decrease) an Employee's salary from Manager class.
 *
 * @param employee_id - The id of the specified Employee.
 * @param wage - The wage that needs to be added or decreased from salary.
 */
    void setEmployeeSalary(int employee_id, int wage);

/**
 * getEmployeesId: A function that gets all the id's of the Employees that work under this manager. Returns it in a form of a vector.
 * @return
 *      vector - Returns a vector of int's. The values in the vector are the requested id's.
 */
    std::vector<int> getEmployeesId() const;

/**
 * hasEmployees: A function that checks wether or not this Manager has Employees.
 * @return
 *      true - If Employees set is not empty.
 *      false - If the Employees set is empty.
 */
    bool hasEmployees() const;

    //Prints
    std::ostream& printShort(std::ostream& os) override;
    std::ostream& printLong(std::ostream& os) override;

    //Clone
    Manager* clone() const override;
};

}
#endif /* MANAGER_H_ */