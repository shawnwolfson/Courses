#ifndef _HASH_H_
#define _HASH_H_
#include <iostream>
#include <string>
#include "rankTree.h"
#include "node.h"
#include <memory>
#include "employee.h"
#include "compare_functors.h"
#include "Exceptions.h"




class EmployeesHashTable
{
    long long size;
    long long current_capacity;
    Tree<Employee,CompareById<Employee>>** tree;

    public:
    EmployeesHashTable(long long size = 4): size(size), current_capacity(0), tree(new Tree<Employee,CompareById<Employee>>*[size])
    {
        CompareById<Employee> compare_func;
        for(long long i = 0; i < size; i++)
        {
            tree[i] = new Tree<Employee,CompareById<Employee>>(compare_func);

        }
 
    }
    ~EmployeesHashTable()
    {
        for(long long i = 0; i < size; i++)
        {
            delete tree[i];
        }
        delete[] tree;
    }

    long long moduluCal(long long id);
    bool hasEmployee (const long long id);
    void addEmployee(shared_ptr<Employee> employee);
    void removeEmployee(const long long id);
    shared_ptr<Employee> getEmployee(const long long id);
    void expandTableSize();
    void collapseTableSize(const long long forbidden_id);


    Tree<Employee,CompareById<Employee>>* getTree(long long i)
    {
        return tree[i];

    }

    void updateMagicBox(MagicBox* magic_box);

    friend EmployeesHashTable* mergeHashTables (EmployeesHashTable* table1, EmployeesHashTable* table2);
};

EmployeesHashTable* mergeHashTables (EmployeesHashTable* table1, EmployeesHashTable* table2);

#endif //_HASH_H_