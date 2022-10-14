#ifndef SKILL_H_
#define SKILL_H_

#include <string>
#include <iostream>

namespace mtm {

class Skill
{
    int id;
    std::string name;
    int points_to_acquire_skill;

public:
    //C'tors, d'tor
    Skill() = default;
    Skill(int id, std::string name, int points_to_acquire_skill);
    Skill(const Skill& skill) = default;
    ~Skill() = default;

    //Getters
    int getId() const;
    std::string getName() const;
    int getRequiredPoints() const;

    //Operators
    Skill& operator+= (const int points); 
    Skill& operator= (const Skill& skill);
    Skill operator++(int);
    bool operator< (const Skill skill_to_compare_with) const;
    bool operator> (const Skill skill_to_compare_with) const;
    bool operator== (const Skill skill_to_compare_with) const;
    bool operator<= (const Skill skill_to_compare_with) const;
    bool operator>= (const Skill skill_to_compare_with) const;
    bool operator!= (const Skill skill_to_compare_with) const;

    //Class friends
    friend std::ostream& operator<< (std::ostream& os, const Skill skill);
};

std::ostream& operator<<(std::ostream& os, const Skill skill);
Skill operator+ (const Skill& skill, const int points); //Needs to be a symmetrical operator
Skill operator+ (const int points, const Skill& skill); //Needs to be a symmetrical operator

}
#endif /* SKILL_H_ */