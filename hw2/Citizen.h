#ifndef CITIZEN_H_
#define CITIZEN_H_

#include <string>
#include <iostream>

namespace mtm {

class Citizen
{
    int id;
    std::string first_name;
    std::string last_name;
    int birth_year;

public:
    //C'tors, d'tor
    Citizen(int id, std::string first_name, std::string last_name, int birth_year);
    Citizen() = default;
    Citizen(const Citizen& citizen) = default;
    virtual ~Citizen() = default;

    //Getters
    int getId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    int getBirthYear() const;

    //Operator overloading
    Citizen& operator=(const Citizen& citizen) = default;
    bool operator<(const Citizen& citizen) const;
    bool operator>(const Citizen& citizen) const;
    bool operator==(const Citizen& citizen) const;
    bool operator<=(const Citizen& citizen) const;
    bool operator>=(const Citizen& citizen) const;
    bool operator!=(const Citizen& citizen) const;

    //Virtual functions
    virtual std::ostream& printShort(std::ostream& os) = 0;
    virtual std::ostream& printLong(std::ostream& os) = 0;
    virtual Citizen* clone() const = 0;
};

}
#endif /* CITIZEN_H_ */