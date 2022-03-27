#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include "Skill.h"
#include "Citizen.h"
#include <set>
#include <iostream>

namespace mtm {

class Employee : public Citizen
{
    int salary;
    int score;
    std::set<Skill> skill_set;

public:
    //C'tors, d'tor
    Employee(int id, std::string first_name, std::string last_name, int birth_year);
    Employee(const Employee& employee) = default;
    ~Employee() = default;

    //Methods
    void learnSkill(const Skill skill);
    void forgetSkill(const int skill_id);
    bool hasSkill(const int skill_id);

    //Getters and setters
    int getSalary() const;
    int getScore() const;
    void setSalary(const int wage);
    void setScore(const int points);

    //Printing functions
    std::ostream& printShort(std::ostream& os) override;
    std::ostream& printLong(std::ostream& os) override;

    //Clone function
    Employee* clone() const override;
};

}
#endif /* EMPLOYEE_H_ */