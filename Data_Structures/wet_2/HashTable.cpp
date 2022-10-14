#include "HashTable.h"

long long EmployeesHashTable::moduluCal(long long id)
{
    return id % (this->size);
}

bool EmployeesHashTable::hasEmployee (const long long id)
{
    if (tree[moduluCal(id)]->findWithId(id) != nullptr)
    {
        return true;
    }
    return false;
}

shared_ptr<Employee>* employeesInTree(Tree<Employee,CompareById<Employee>>* tree)
{
    return tree->allTreeTravel(INORDER);
} 

void EmployeesHashTable::expandTableSize()
{
    long long old_size = size;
    size = size * 2;
    current_capacity = 0;

    Tree<Employee,CompareById<Employee>>** new_table = new Tree<Employee,CompareById<Employee>>*[size];
    CompareById<Employee> compare_func;
    for(long long i = 0; i < size; i++)
    {
        new_table[i] = new Tree<Employee,CompareById<Employee>>(compare_func);

    }
    Tree<Employee,CompareById<Employee>>** old_table = tree;
    tree = new_table;

    for (long long j = 0 ; j < old_size ; j++)
    {
        shared_ptr<Employee>* employee_table = employeesInTree(old_table[j]);
        for (long long k =0; k <old_table[j]->getSize();k++)
        {
            addEmployee(employee_table[k]);
        }
        delete[] employee_table;
    }

    for(long long i = 0; i < old_size; i++)
    {
        delete old_table[i];
    }
    delete[] old_table;
}


void EmployeesHashTable::addEmployee(shared_ptr<Employee> employee)
{
    if(hasEmployee(employee->getId()))
    {
        throw Failure();
    }
    if(current_capacity + 1 >= (size))
    {
        expandTableSize();
    }
    tree[moduluCal(employee->getId())]->insert(employee);
    current_capacity++;
}

void EmployeesHashTable::collapseTableSize(const long long forbidden_id)
{
    long long old_size = size;
    size = size / 2;
    current_capacity = 0;

    Tree<Employee,CompareById<Employee>>** new_table = new Tree<Employee,CompareById<Employee>>*[size];
    CompareById<Employee> compare_func;
    for(long long i = 0; i < size; i++)
    {
        new_table[i] = new Tree<Employee,CompareById<Employee>>(compare_func);

    }
    Tree<Employee,CompareById<Employee>>** old_table = tree;
    tree = new_table;

    for (long long j = 0 ; j < old_size ; j++)
    {
        shared_ptr<Employee>* employee_table =  employeesInTree(old_table[j]);
        for (long long k =0; k <old_table[j]->getSize();k++)
        {
            if (forbidden_id != employee_table[k] -> getId())
            {
                addEmployee(employee_table[k]);
            } 
        }
        delete[] employee_table;
    }

    for(long long i = 0; i < old_size; i++)
    {
        delete old_table[i];
    }
    delete[] old_table;
}

void EmployeesHashTable::removeEmployee(const long long id_to_remove)
{
    if(!hasEmployee(id_to_remove))
    {
        throw Failure();
    }
    if(current_capacity - 1 <= (size / 4) && size >= 4)
    {
        collapseTableSize(id_to_remove);
    }
    else
    {
        shared_ptr<Employee> employee = tree[moduluCal(id_to_remove)]->findWithId(id_to_remove);
        tree[moduluCal(id_to_remove)]->remove(employee);
        current_capacity--;
    }
}

shared_ptr<Employee> EmployeesHashTable::getEmployee(const long long id)
{
    return tree[moduluCal(id)]->findWithId(id);
}

EmployeesHashTable* mergeHashTables (EmployeesHashTable* table1, EmployeesHashTable* table2)
{
    long long new_size = table1->size + table2->size;
    EmployeesHashTable* new_table = new EmployeesHashTable(new_size);
    for (long long j = 0 ; j < table1->size ; j++)
    {
        shared_ptr<Employee>* employee_table = employeesInTree(table1->tree[j]);
        for (long long k =0; k <table1->tree[j]->getSize();k++)
        {
            new_table->addEmployee(employee_table[k]);
        }
        delete[] employee_table;
    }
    for (long long j = 0 ; j < table2->size ; j++)
    {
        shared_ptr<Employee>* employee_table = employeesInTree(table2->tree[j]);
        for (long long k =0; k <table2->tree[j]->getSize();k++)
        {
            new_table->addEmployee(employee_table[k]);
        }
        delete[] employee_table;
    }
    return new_table;
}


void EmployeesHashTable::updateMagicBox(MagicBox* magic_box)
{
    for (long long j = 0 ; j < size ; j++)
    {
        shared_ptr<Employee>* employee_table = employeesInTree(tree[j]);
        for(long long k =0; k <tree[j]->getSize();k++)
        {
            employee_table[k]->setMagicBox(magic_box);
        }
        delete[] employee_table;
    }
}