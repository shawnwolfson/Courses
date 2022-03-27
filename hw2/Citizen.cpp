#include "Citizen.h"
using namespace mtm;
using std::endl;
using std::string;

Citizen::Citizen(int id, string first_name, string last_name, int birth_year) : 
    id(id), first_name(first_name), last_name(last_name), birth_year(birth_year)
{}

//Getters
int Citizen::getId() const
{
    return id;
}

string Citizen::getFirstName() const
{
    return first_name;
}

string Citizen::getLastName() const
{
    return last_name;
}

int Citizen::getBirthYear() const
{
    return birth_year;
}

//Operators
bool Citizen::operator<(const Citizen& citizen) const
{
    return ((getId() < citizen.getId()) ? true : false);
}

bool Citizen::operator>(const Citizen& citizen) const
{
    return ((getId() > citizen.getId()) ? true : false);
}

bool Citizen::operator==(const Citizen& citizen) const
{
    return ((getId() == citizen.getId()) ? true : false);
}

bool Citizen::operator<=(const Citizen& citizen) const
{
    return ((getId() <= citizen.getId()) ? true : false);
}

bool Citizen::operator>=(const Citizen& citizen) const
{
    return ((getId() >= citizen.getId()) ? true : false);
}

bool Citizen::operator!=(const Citizen& citizen) const
{
    return ((getId() != citizen.getId()) ? true : false);
}