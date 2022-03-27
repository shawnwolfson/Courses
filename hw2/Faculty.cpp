#include <iostream>
#include "Faculty.h"
using namespace mtm;
using std::cout;
using std::endl;
#include <vector>
using std::vector;

class FacultyCondition1: public Condition{
bool operator()(Employee* employee) override{
cout << "Condition 1 Called" << endl;
return employee->getId() > 0;
}
};
class FacultyCondition2: public Condition{
bool operator()(Employee* employee) override{
cout << "Condition 2 Called" << endl;
return employee->getId() > 3;
}
};
int main() {
vector<Faculty<Condition>> Faculties;
FacultyCondition1 fc1;
FacultyCondition2 fc2;
Skill skill1(1,"Programming with c++",0);
Skill skill2(2,"Programming with c",10);
Faculty<Condition> faculty1(1,skill1,10,&fc1);
Faculty<Condition> faculty2(3,skill2,10,&fc2);
Employee e1(10, "John", "Williams", 2002);
Employee e2(20, "Alex", "Martinez", 2000);
Faculties.push_back(faculty1);
Faculties.push_back(faculty2);
for(Faculty<Condition> faculty:Faculties){
faculty.teach(&e1);
faculty.teach(&e2);
}
return 0;
}