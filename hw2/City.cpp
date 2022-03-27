#include "City.h"

using namespace mtm;
using std::set;
using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

//Private functions for internal use
Employee* City::findEmployee(int employee_id) const
{
    Employee temp_employee(employee_id, "", "", 0);
    if (!employees.count(&temp_employee)) {
        throw EmployeeDoesNotExist();
    }
    return *(employees.find(&temp_employee));
}

Manager* City::findManager(int manager_id) const
{
    Manager temp_manager(manager_id, "", "", 0);
    if (!managers.count(&temp_manager)) {
        throw ManagerDoesNotExist();
    }
    return *(managers.find(&temp_manager));
}

Workplace* City::findWorkplace(int workplace_id) const
{
    Workplace temp_workplace(workplace_id, "", 0, 0);
    if (!workplaces.count(&temp_workplace)) {
        throw WorkplaceDoesNotExist();
    }
    return *(workplaces.find(&temp_workplace));
}

Faculty<Condition>* City::findFaculty(int faculty_id) const
{
    Skill temp;
    Faculty<Condition> temp_faculty(faculty_id, temp, 0, NULL);
    if (!faculties.count(&temp_faculty)) {
        throw FacultyDoesNotExist();
    }
    return *(faculties.find(&temp_faculty));
}
             //////////////////


//**C'tor's and d'tor implementation
City::City(string city_name ) : name(city_name), employees() , managers(), faculties(), workplaces()
{}

City::~City()
{
    set<Employee*>::iterator employee_it;
    for (employee_it=employees.begin(); employee_it!=employees.end(); ++employee_it) {
        delete *employee_it;
    }

    set<Manager*>::iterator manager_it;
    for (manager_it=managers.begin(); manager_it!=managers.end(); ++manager_it) {
        delete *manager_it;
    }

    set<Workplace*>::iterator workplace_it;
    for (workplace_it=workplaces.begin(); workplace_it!=workplaces.end(); ++workplace_it) {
        delete *workplace_it;
    }

    set<Faculty<Condition>*>::iterator faculty_it;
    for (faculty_it=faculties.begin(); faculty_it!=faculties.end(); ++faculty_it) {
        delete *faculty_it;
    }
}

//Methods 
void City::addEmployee(int id, string first_name, string last_name, int birth_year)
{
    Manager temp_manager(id, "", "", 0);
    Employee* new_employee = new Employee(id, first_name, last_name, birth_year);
    if (employees.count(new_employee) || managers.count(&temp_manager)) {
        delete new_employee;
        throw CitizenAlreadyExists();
    }
    employees.insert(new_employee);
}

void City::addManager(int id, string first_name, string last_name, int birth_year)
{
    Employee temp_employee(id, "", "", 0);
    Manager* new_manager = new Manager(id, first_name, last_name, birth_year);
    if (managers.count(new_manager) || employees.count(&temp_employee)) {
        delete new_manager;
        throw CitizenAlreadyExists();
    }
    managers.insert(new_manager);
}

void City::addFaculty(int id, const Skill& skill, int points, Condition* condition)
{
    Faculty<Condition>* new_faculty = new Faculty<Condition>(id, skill, points, condition);
    if (faculties.count(new_faculty)) {
        delete new_faculty;
        throw FacultyAlreadyExists();
    }
    faculties.insert(new_faculty);
}

void City::createWorkplace(int id, string name, int employee_salary, int manager_salary)
{
    Workplace* new_workplace = new Workplace(id, name, employee_salary, manager_salary);
    if (workplaces.count(new_workplace)) {
        delete new_workplace;
        throw WorkplaceAlreadyExists();
    }
    workplaces.insert(new_workplace);
}

void City::teachAtFaculty(int employee_id, int faculty_id)
{
    Employee* temp_employee = findEmployee(employee_id);
    Faculty<Condition>* temp_faculty = findFaculty(faculty_id);
    set<Employee*>::iterator employee_it = employees.find(temp_employee);
    set<Faculty<Condition>*>::iterator faculty_it = faculties.find(temp_faculty);
    (*faculty_it)->teach(*employee_it);
}

void City::hireManagerAtWorkplace(int manager_id, int workplace_id)
{
    Manager* temp_manager = findManager(manager_id);
    Workplace* temp_workplace = findWorkplace(workplace_id);
    set<Manager*>::iterator manager_it = managers.find(temp_manager);
    set<Workplace*>::iterator workplace_it = workplaces.find(temp_workplace);
    (*workplace_it)->hireManager(*manager_it);
}

void City::fireEmployeeAtWorkplace(int employee_id, int manager_id, int workplace_id)
{
    findEmployee(employee_id);
    findManager(manager_id);
    Workplace* temp_workplace = findWorkplace(workplace_id);
    set<Workplace*>::iterator workplace_it = workplaces.find(temp_workplace);
    (*workplace_it)->fireEmployee(employee_id, manager_id);
}

void City::fireManagerAtWorkplace(int manager_id, int workplace_id)
{
    Manager* temp_manager = findManager(manager_id);
    Workplace* temp_workplace = findWorkplace(workplace_id);
    set<Manager*>::iterator manager_it = managers.find(temp_manager);
    set<Workplace*>::iterator workplace_it = workplaces.find(temp_workplace);
    (*workplace_it)->emptyEmployeeGroup(manager_id);
    (*workplace_it)->fireManager(manager_id);
}

int City::getAllAboveSalary(ostream& os, int above_salary) const
{
    set<Citizen*, setCompareFunctionCitizen> above_salary_set;
    set<Manager*>::iterator manager_it;
    for (manager_it=managers.begin(); manager_it!=managers.end(); ++manager_it)
    {
        if ((*manager_it)->getSalary() >= above_salary) {
           above_salary_set.insert(*manager_it);
        }
    }
    set<Employee*>::iterator employee_it;
    for (employee_it=employees.begin(); employee_it!=employees.end(); ++employee_it)
    {
        if ((*employee_it)->getSalary() >= above_salary) {
           above_salary_set.insert(*employee_it);
        }
    }
    set<Citizen*>::iterator citizen_it;
    for (citizen_it=above_salary_set.begin(); citizen_it!=above_salary_set.end(); ++citizen_it) {
        (*citizen_it)->printShort(os);
    }
    return above_salary_set.size();
}

bool City::isWorkingInTheSameWorkplace(const int id_1, const int id_2)
{
    findEmployee(id_1); //These two lines check if the employees are present in the city.
    findEmployee(id_2);
    set<Workplace*>::iterator workplace_it;
    for (workplace_it=workplaces.begin(); workplace_it!=workplaces.end(); ++workplace_it)
    {
        bool employee_1_at_workplace = false;
        bool employee_2_at_workplace = false;
        vector<int> id_managers = (*workplace_it)->getManagersId();
        for (unsigned int i_manager = 0; i_manager < id_managers.size(); ++i_manager)
        {
            Manager* temp = findManager(id_managers[i_manager]);
            vector<int> id_employees = temp->getEmployeesId();
            for (unsigned int i_employee = 0; i_employee < id_employees.size(); ++i_employee)
            {
                if (id_employees[i_employee] == id_1) {
                    employee_1_at_workplace = true;
                }
                if (id_employees[i_employee] == id_2) {
                    employee_2_at_workplace = true;
                }
            }
        }
        if (employee_1_at_workplace && employee_2_at_workplace) {
            return true;
        }
    }
    return false;
}

ostream& City::printAllEmployeesWithSkill(ostream& os, int skill_id) const
{
    set<Employee*>::iterator employee_it;
    for (employee_it = employees.begin(); employee_it != employees.end(); ++employee_it)
    {
        if ((*employee_it)->hasSkill(skill_id)) {
            (*employee_it)->printShort(os);
        }
    }
    return os;
}