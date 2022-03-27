#ifndef FACULTY_H_
#define FACULTY_H_

#include "Skill.h"
#include "Employee.h"
#include "exceptions.h"
#include <iostream>

namespace mtm {

//Condition to be overridden
class Condition
{
public:
    virtual bool operator()(Employee* employee) = 0;
};


template<class T>
class Faculty
{
    int id;
    Skill skill;
    int points;
    T* condition;

public:
    //C'tors, d'tor
    Faculty(int id, const Skill& skill, int points, T* condition);
    Faculty(const Faculty& faculty) = default;
    ~Faculty() = default;

    //Methods
    void teach(Employee* employee);

    //Getters
    Skill getSkill() const;
    int getId() const;
    int getAddedPoints() const;
};

//Class templates
template<class T>
Faculty<T>::Faculty(int id, const Skill& skill, int points, T* condition) : id(id),
     skill(skill), points(points), condition(condition)
{}

template<class T>
Skill Faculty<T>::getSkill() const
{
    return skill;
}

template<class T>
int Faculty<T>::getId() const
{
    return id;
}

template<class T>
int Faculty<T>::getAddedPoints() const
{
    return points;
}

template<class T>
void Faculty<T>::teach(Employee* employee)
{
    bool temp = this->condition->operator()(employee);
    if (!temp)
    {
        throw EmployeeNotAccepted();
    }
    employee->learnSkill(this->skill);
    employee->setScore(this->points);
}

}
#endif /* FACULTY_H_ */