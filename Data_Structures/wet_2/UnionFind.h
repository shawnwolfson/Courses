#ifndef _UNIONFIND_H_
#define _UNIONFIND_H_
#include <iostream>
#include <string>
#include "rankTree.h"
#include "node.h"
#include <memory>
#include "employee.h"
#include "compare_functors.h"
#include "magicBox.h"
#include "company.h"
using std::shared_ptr;




class UnionFind
{
  
    private:
        shared_ptr<Node<MagicBox>>* base;
        shared_ptr<Node<Company>>* all_companies;
  
    public:
        UnionFind(long long k) : base(new shared_ptr<Node<MagicBox>>[k]), all_companies(new shared_ptr<Node<Company>>[k]) 
        {
            init_arrays(k);
        }

        ~UnionFind()
        {
            delete[] base;
            delete[] all_companies;
        }

        MagicBox* find(const long long company_id);
        MagicBox* uniteCompanies(const long long acquire_id, const long long purchased_id, double factor);
        void init_arrays(const long long k);
        double findValue(const long long company_id);
        void freeMemory();
        
};


#endif //_UNIONFIND_H_