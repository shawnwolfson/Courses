#ifndef _COMPANY_H_
#define _COMPANY_H_
#include <iostream>
#include "employee.h"
#include "rankTree.h"
#include "compare_functors.h"
#include "Exceptions.h"
using std::shared_ptr;

class Company
{
    private:
        int id;
        double relative_value;
        
    public:
        Company(const int id, const double value) : id(id),relative_value(value){};  
        int getId() const;
        int getRelativeValue() const;
        
        void setRelativeValue(const double value);
        void raiseRelativeValue(const double raise);
        void setID(const int id); 
};



#endif //_COMPANY_H_