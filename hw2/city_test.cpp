#include <iostream>
#include "Employee.h"
#include "Manager.h"
#include "WorkPlace.h"
#include "City.h"
#include <assert.h>
#include "exceptions.h"
using namespace mtm;
using std::cout;
using std::endl;
bool facultyCond1(Employee* emp){
    return emp->getId()>0;
}
bool facultyCond2(Employee* emp){
    return emp->hasSkill(1);
}
class HiringCondition {
public:
    bool operator()(Employee *emp) {
        return emp->getScore() > 5;
    }
};

int main() {
    City city ("TLV");
    Skill skill1(1,"Programming with c++",0);
    Skill skill2(2,"Programming with c",10);
    city.addEmployee(11, "John", "Williams", 2002);
    city.addEmployee(12, "Alex", "Martinez", 2000);
    city.addEmployee(13, "Lionel", "Smith", 2000);
    city.addManager(104,"Mohamad","Masarwa",1998);
    city.addFaculty(1001, skill1, 10, facultyCond1);
    city.addFaculty(1002, skill2, 15, facultyCond2);
    city.teachAtFaculty(11,1001);
    city.teachAtFaculty(11,1002);
    city.teachAtFaculty(12,1001);
    city.teachAtFaculty(13,1001);
    city.createWorkPlace(10001, "Meta", 10000, 20000);
    city.hireManagerAtWorkplace(104,10001);
    HiringCondition hiringCondition;
    city.hireEmployeeAtWorkplace(hiringCondition, 11, 104, 10001);
    city.hireEmployeeAtWorkplace(hiringCondition, 12, 104, 10001);
    city.hireEmployeeAtWorkplace(hiringCondition, 13, 104, 10001);
    city.fireEmployeeAtWorkplace(12,104,10001);
    cout << "printAllAboveSalary output: " << endl;
    city.printAllAboveSalary(cout,1000);
    cout << endl << "printAllEmployeesWithSkill output" << endl;
    city.printAllEmployeesWithSkill(cout, 1);
    city.fireManagerAtWorkplace(104,10001);
    cout << "printAllAboveSalary output: " << endl;
    city.printAllAboveSalary(cout,1000);
    cout << endl << "printAllEmployeesWithSkill output" << endl;
    city.printAllEmployeesWithSkill(cout, 1);
    return 0;
}
