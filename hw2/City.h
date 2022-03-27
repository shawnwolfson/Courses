#ifndef CITY_H_
#define CITY_H_

#include "exceptions.h"
#include "Faculty.h"
#include "Workplace.h"
#include <string>
#include <iostream>
#include <set>
#include <vector>

namespace mtm {

/**
 * setCompareFunctionCitizen: A function object that tells the compiler how to order the Citizen container
 *
 * @param citizen1 - Citizen pointer for first Citizen
 * @param citizen2 - Citizen pointer for second Citizen
 * @return
 *     true - if citizen1's id is greater than citizen2's id;
 *     false - if citizen1's id is less than citizen2's id; 
 */
class setCompareFunctionCitizen
{
public:
    bool operator()(const Citizen* citizen1, const Citizen* citizen2) const {
        return ((*citizen1).getId() < (*citizen2).getId());
    }
};

/**
 * setCompareFunctionfaculty: A function object that tells the compiler how to order the Faculty container
 *
 * @param faculty1 - Faculty<condition> pointer for first Faculty
 * @param faculty2 - Faculty<condition> pointer for second Faculty
 * @return
 *     true - if faculty1's id is greater than faculty2's id;
 *     false - if faculty1's id is less than faculty2's id; 
 */
class setCompareFunctionfaculty
{
public:
    bool operator()(const Faculty<Condition>* faculty1, const Faculty<Condition>* faculty2) const {
        return ((*faculty1).getId() < (*faculty2).getId());
    }
};
/**
 * setCompareFunctionWorkplace: A function object that tells the compiler how to order the Workplace container
 *
 * @param work1 - Workplace pointer for first Workplace
 * @param work2 - Workplace pointer for second Workplace
 * @return
 *     true - if first Workplace id is greater than second's Workplace id;
 *     false - if first Workplace id is less than second's Workplace id;
 */
class setCompareFunctionWorkplace
{
public:
    bool operator()(const Workplace* work1, const Workplace* work2) const {
        return ((*work1).getId() < (*work2).getId());
    }
};


class City
{
    //Private arguments and containers
    std::string name;
    std::set<Employee*, setCompareFunctionEmployee> employees;
    std::set<Manager*, setCompareFunctionManager> managers;
    std::set<Faculty<Condition>*, setCompareFunctionfaculty> faculties;
    std::set<Workplace*, setCompareFunctionWorkplace> workplaces;
    
    /**
     * findEmployee: Searches the city for the specified Employee.
     *
     * @param employee_id - The id of the requested Employee.
     * @return
     *     Employee* - returns a pointer to the Employee if the Employee exists in the City.
     * @exception
     *  EmployeeDoesNotExist() - throws this exception if the Employee does not exist in the City.
     */
    Employee* findEmployee(int employee_id) const;

    /**
     * findManager: Searches the city for the specified Manager.
     *
     * @param manager_id - The id of the requested Manager.
     * @return
     *     Manager* - returns a pointer to the Manager if the Manager exists in the city.
     * @exception
     *  ManagerDoesNotExist() - throws this exception if the Manager does not exist in the city.
     */
    Manager* findManager(int manager_id) const;

    /**
     * findWorkplace: Searches the city for the specified Workplace.
     *
     * @param workplace_id - The id of the requested Workplace.
     * @return
     *     Workplace* - returns a pointer to the Workplace if the Workplace exists in the city.
     * @exception
     *  WorkplaceDoesNotExist() - throws this exception if the Workplace does not exist in the city.
     */
    Workplace* findWorkplace(int workplace_id) const;

    /**
     * findFaculty: Searches the city for the specified Faculty.
     *
     * @param faculty_id - The id of the requested Faculty.
     * @return
     *     Faculty* - returns a pointer to the Faculty if the Faculty exists in the city.
     * @exception
     *  FacultyDoesNotExist() - throws this exception if the Faculty does not exist in the city.
     */
    Faculty<Condition>* findFaculty(int faculty_id) const;

public:
    //C'tors, d'tor
    City(std::string city_name);
    City(const City& city) = default;
    ~City();

    //Methods
    void addEmployee(int id, std::string first_name, std::string last_name, int birth_year);
    void addManager(int id, std::string first_name, std::string last_name, int birth_year);
    void addFaculty(int id, const Skill& skill, int points, Condition* condition);
    void createWorkplace(int id, std::string name, int employee_salary, int manager_salary);
    void teachAtFaculty(int employee_id, int faculty_id);
    template<class T>
    void hireEmployeeAtWorkplace(T condition, int employee_id, int manager_id, int workplace_id);
    void hireManagerAtWorkplace(int manager_id, int workplace_id);
    void fireEmployeeAtWorkplace(int employee_id, int manager_id, int workplace_id);
    void fireManagerAtWorkplace(int manager_id, int workplace_id);
    bool isWorkingInTheSameWorkplace(const int id_1, const int id_2);

    //Getters
    int getAllAboveSalary(std::ostream& os, int above_salary) const;

    //Printing functions
    std::ostream& printAllEmployeesWithSkill(std::ostream& os, int skill_id) const;
};

//Template class for hiring employee at Workplace. Because it is a template it is implemented in the header file.
template<class T>
void City::hireEmployeeAtWorkplace(T condition, int employee_id, int manager_id, int workplace_id)
{
    Employee* temp_employee = findEmployee(employee_id);
    findManager(manager_id); //This line checks if the manager is present in the city or not.
    Workplace* temp_workplace = findWorkplace(workplace_id);
    std::set<Employee*>::iterator employee_it = employees.find(temp_employee);
    std::set<Workplace*>::iterator workplace_it = workplaces.find(temp_workplace);
    (*workplace_it)->hireEmployee(condition, *employee_it, manager_id);
}

}
#endif /* CITY_H_ */