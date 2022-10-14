#ifndef _COMPARE_FUNCTOR_H_
#define _COMPARE_FUNCTOR_H_
#include <iostream>
#include <string>
#include "employee.h"
#include <memory>
#include "rankTree.h"
using std::shared_ptr;

template <class T>
class CompareById
{
    public:
    CompareResult operator() (shared_ptr<T> target ,shared_ptr<Node<T>> node)
    {
        if(node->getData()->getId() > target->getId())
        {
            return SMALLER_THAN;
        }
        
        else if(node->getData()->getId() < target->getId())
        {
            return LARGER_THAN;
        }

        return EQUAL;
    }
    CompareResult operator() (long long id ,shared_ptr<Node<T>> node)
    {
        if(node->getData()->getId() > id)
        {
            return SMALLER_THAN;
        }
        
        else if(node->getData()->getId() < id)
        {
            return LARGER_THAN;
        }

        return EQUAL;
    }
    
    CompareResult operator() (shared_ptr<T> first_element ,shared_ptr<T> second_element)
    {
        if(second_element->getId() > first_element->getId())
        {
            return SMALLER_THAN;
        }
        
        else if(second_element->getId() < first_element->getId())
        {
            return LARGER_THAN;
        }

        return EQUAL;
    }
        
};

class CompareBySalary
{
    public:
    CompareResult operator() (shared_ptr<Employee> target,shared_ptr<Node<Employee>> node)
    {
        if(node->getData()->getSalary() > target->getSalary())
        {
            return SMALLER_THAN;
        }
        
        else if(node->getData()->getSalary() < target->getSalary())
        {
            return LARGER_THAN;
        }

        CompareById<Employee> id_compare;
        CompareResult compare_by_id_res = id_compare(target,node);
        switch (compare_by_id_res)
        {
        case SMALLER_THAN:
            return SMALLER_THAN;
            break;
        
        case LARGER_THAN:
            return LARGER_THAN;
            break;
        case EQUAL:
            return EQUAL;
            break;
        }
        return EQUAL;
    } 
    CompareResult operator() (long long salary, long long id ,shared_ptr<Node<Employee>> node)
    {
        if(node->getData()->getSalary() > salary)
        {
            return SMALLER_THAN;
        }
        
        else if(node->getData()->getSalary() < salary)
        {
            return LARGER_THAN;
        }

        CompareById<Employee> id_compare;
        CompareResult compare_by_id_res = id_compare(id,node);
        switch (compare_by_id_res)
        {
        case SMALLER_THAN:
            return SMALLER_THAN;
            break;
        
        case LARGER_THAN:
            return LARGER_THAN;
            break;
        case EQUAL:
            return EQUAL;
            break;
        }
        return EQUAL;
        
    } 
     CompareResult operator() (long long salary ,shared_ptr<Node<Employee>> node)
    {
        if(node->getData()->getSalary() > salary)
        {
            return SMALLER_THAN;
        }
        
        else if(node->getData()->getSalary() < salary)
        {
            return LARGER_THAN;
        }
        return EQUAL;
        
    } 

    CompareResult operator() (shared_ptr<Employee> emp1,shared_ptr<Employee> emp2)
    {
        if(emp1->getSalary() < emp2->getSalary())
        {
            return SMALLER_THAN;
        }
        
        else if(emp1->getSalary() > emp2->getSalary())
        {
            return LARGER_THAN;
        }

        CompareById<Employee> id_compare;
        CompareResult compare_by_id_res = id_compare(emp1,emp2);
        switch (compare_by_id_res)
        {
        case SMALLER_THAN:
            return SMALLER_THAN;
            break;
        
        case LARGER_THAN:
            return LARGER_THAN;
            break;
        case EQUAL:
            return EQUAL;
            break;
        }
        return EQUAL;
    } 
};

#endif //_COMPARE_FUNCTOR_H_
