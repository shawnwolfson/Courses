#include "UnionFind.h"


void UnionFind::init_arrays(long long k)
{
    CompareBySalary cmpFunc;
    for (long long i=1 ; i <= k; i++)
    {
        shared_ptr<Company> temp_company (new Company(i,i));
        shared_ptr<MagicBox> temp_magic_box (new MagicBox(cmpFunc,i));
        
        shared_ptr<Node<MagicBox>> magic_box_node(new Node<MagicBox>);
        shared_ptr<Node<Company>> company_node(new Node<Company>);
        

        company_node->setData(temp_company);
        magic_box_node->setData(temp_magic_box);
        base[i-1] = magic_box_node;
        magic_box_node->getData()->setReverseCompanyTree(company_node);
        company_node->insertMagicBox(magic_box_node->getData().get());
        all_companies[i-1] = company_node;
    }
}


MagicBox* UnionFind::find(const long long company_id)
{
    shared_ptr<Node<Company>> curr_root = nullptr;
    shared_ptr<Node<Company>> curr_company = all_companies[company_id - 1];
    long long sumValues = 0;
    long long toSubstract = 0;
    while(curr_company->getMagicBox() == nullptr)
    {
        sumValues += curr_company->getData()->getRelativeValue();
        curr_company = curr_company->getFather();
    }
  
    curr_root = curr_company;
    curr_company = all_companies[company_id - 1];

    while(curr_company->getMagicBox() == nullptr)
    {
        shared_ptr<Node<Company>> temp_father = curr_company->getFather();
        long long current_value = curr_company->getData()->getRelativeValue();
        curr_company->getData()->setRelativeValue(sumValues - toSubstract);
        toSubstract += current_value;
        curr_company->insertFather(curr_root);
        curr_company = temp_father;
    }

    return curr_root->getMagicBox();
}


double UnionFind::findValue(const long long company_id)
{
    find(company_id);
    shared_ptr<Node<Company>> curr_company = all_companies[company_id - 1];
    double curr_value = curr_company->getData()->getRelativeValue();
    if(curr_company->getMagicBox() == nullptr)
    {
        return curr_value += curr_company->getFather()->getData()->getRelativeValue();;
    }

    return curr_value;
}

MagicBox* UnionFind::uniteCompanies(const long long acquire_id, const long long purchased_id, double factor)
{
    
    MagicBox* acquire_comp = find (acquire_id);
    MagicBox* purchased_comp = find (purchased_id);
    if (acquire_comp == purchased_comp)
    {
        throw Failure();
    }
    if (acquire_comp->getCompanyReverseTreeSize() > purchased_comp->getCompanyReverseTreeSize())
    {
        return merge_purchased_to_acquirer(acquire_comp,purchased_comp,factor,purchased_comp->getRealValue());
    }

        return merge_acquire_to_purchased(acquire_comp,purchased_comp,factor,purchased_comp->getRealValue());
}

void UnionFind::freeMemory()
{
    delete[] base;
    delete[] all_companies;
    base = nullptr;
    all_companies = nullptr;
}