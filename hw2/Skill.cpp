#include "Skill.h"
#include "exceptions.h"
using namespace mtm;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

//C'tor
Skill::Skill(int id, string name, int points_to_acquire_skill):
        id(id), name(name), points_to_acquire_skill(points_to_acquire_skill)
{}

//Getters
int Skill::getId() const
{
    return this->id;
}

string Skill::getName() const
{
    return this->name;
}

int Skill::getRequiredPoints() const
{
    return this->points_to_acquire_skill;
}

//Operators
bool Skill::operator< (const Skill skill_to_compare_with) const
{
    return (this->getId() < skill_to_compare_with.getId() ? true : false);
}

bool Skill::operator> (const Skill skill_to_compare_with) const
{
    return (this->getId() > skill_to_compare_with.getId() ? true : false);
}

bool Skill::operator== (const Skill skill_to_compare_with) const
{
    return (this->getId() == skill_to_compare_with.getId()) ? true : false;
}

bool Skill::operator<= (const Skill skill_to_compare_with) const
{
    return (operator< (skill_to_compare_with)) || (operator== (skill_to_compare_with));
}

bool Skill::operator>= (const Skill skill_to_compare_with) const
{
    return (operator> (skill_to_compare_with)) || (operator== (skill_to_compare_with));
}

bool Skill::operator!= (const Skill skill_to_compare_with) const
{
    return !(operator== (skill_to_compare_with));
}

Skill Skill::operator++(int)
{
    Skill temp(*this);
    *this += 1;
    return temp;
}

Skill& Skill::operator+=(const int points) 
{
    if(points < 0)
    {
        throw NegativePoints();
    }
    points_to_acquire_skill += points;
    return *this;
}

Skill mtm::operator+ (const Skill& skill, const int points) 
{
    if(points < 0)
    {
        throw NegativePoints(); 
    }
    Skill temp_skill(skill.getId(), skill.getName(), (skill.getRequiredPoints() + points));
    return temp_skill;
}

Skill mtm::operator+ (const int points, const Skill& skill) 
{
    if(points < 0)
    {
        throw NegativePoints();
    }
    Skill result = skill + points;
    return result;
}

Skill& Skill::operator= (const Skill& skill)
{
    if(this == &skill)
    {
        return *this;
    }
    points_to_acquire_skill = skill.points_to_acquire_skill;
    return *this;
}

ostream& mtm::operator<< (ostream& os, const Skill skill)
{
    return os << skill.name << endl;
}

