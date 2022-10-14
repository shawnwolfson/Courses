#ifndef _NODE_H_
#define _NODE_H_
#define NEGATIVE_HEIGHT -1
#define NULL_degree 0
#include <iostream>
#include <string>
#include <memory>
#include <math.h>

using std::shared_ptr;
using std::max;

class MagicBox;

enum RollingCase {CASE_RR, CASE_LL, CASE_RL, CASE_LR};
enum NumOfChilds {NONE,ONE_RIGHT,ONE_LEFT,TWO};

template <class T>
class Node
{
    private:
        shared_ptr<T> data;
        long long height;
        shared_ptr<Node> left_son;
        shared_ptr<Node> right_son;
        shared_ptr<Node> father;
        MagicBox* magicBox;
        long long degree;
        long long sum_of_grades_under;        

        
    
    public:
        Node(shared_ptr<T> data) : data(data), height(0), left_son(nullptr), right_son(nullptr),father(nullptr),magicBox(nullptr),degree(1), sum_of_grades_under(0){};
        Node() : data(nullptr), height(0), left_son(nullptr), right_son(nullptr), father(nullptr),magicBox(nullptr),degree(1),sum_of_grades_under(0) {};

        template <class S, class CompareFunc> friend class Tree;

        void insertLeftSon(shared_ptr<Node> son)
        {
            left_son = son;
        }

        void insertRightSon(shared_ptr<Node> son)
        {
            right_son = son;
        }

        void insertFather(shared_ptr<Node> father)
        {
            this->father = father;
        }

        shared_ptr<Node> getFather()
        {
            return father;
        }

        void insertMagicBox(MagicBox* magic_box)
        {
            this->magicBox = magic_box;
        }

        MagicBox* getMagicBox()
        {
            return magicBox;
        }
        void resetMagicBox()
        {
            magicBox=nullptr;
        }


        bool checkBF() const
        {
            if (heightCal(left_son) - heightCal(right_son) >= 2 || heightCal(left_son) - heightCal(right_son) <= -2)
            {
                return false;
            }
            return true;
        }

        RollingCase getRollingCase() const
        {
            long long balance_factor = heightCal(left_son) - heightCal(right_son);
            long long left_son_bf = 0;
            long long right_son_bf = 0;
            if(left_son == nullptr)
            {
                left_son_bf = NEGATIVE_HEIGHT;
            }

            else
            {
                left_son_bf = heightCal(left_son->left_son) - heightCal(left_son->right_son);
            }

            if(right_son == nullptr)
            {
                right_son_bf = NEGATIVE_HEIGHT;
            }

            else
            {
                right_son_bf = heightCal(right_son->left_son) - heightCal(right_son->right_son);
            }

            if(balance_factor == 2 && left_son_bf >= 0)
            {
                return CASE_LL;
            }

            else if(balance_factor == 2 && left_son_bf == -1)
            {
                return CASE_LR;
            }

            else if(balance_factor == -2 && right_son_bf <= 0)
            {
                return CASE_RR;
            }
            else
            {
                return CASE_RL;
            }
        }

        shared_ptr<Node> getLeftSon() const
        {
            return left_son;
        }

        shared_ptr<Node> getRightSon() const
        {
            return right_son;
        }

        shared_ptr<T> getData() const
        {
            return data;
        }
        
        long long getHeight() const
        {
            return this->height;
        }
        long long getDegree() const
        {
            return this->degree;
        }

        long long  getSumOfGradesUnder() const
        {
            return this->sum_of_grades_under;
        }

        void setSumOfGradesUnder()
        {
            sum_of_grades_under = sumOfGradesUnderCalIncludingMe(left_son) + sumOfGradesUnderCalIncludingMe(right_son);
           
        }

        void setDegree()
        {
            degree = degreeCal(left_son) + degreeCal(right_son) + 1;
        }

        void setHeight()
        {
            height = max(heightCal(right_son), heightCal(left_son)) + 1;
        }

        NumOfChilds getNumOfChilds() const
        {
            if (left_son == nullptr && right_son == nullptr)
            {
                return NONE;
            }

            else if (left_son == nullptr || right_son == nullptr)
            {
                if (left_son != nullptr)
                {
                    return ONE_LEFT;
                }

                return ONE_RIGHT;
            }

            return TWO;
        }

        void setData(shared_ptr<T> data)
        {
            this->data = data;
        }
        
        bool checkIfNodeIsLeaf() const
        {
            return (this-> right_son == nullptr && this->left_son == nullptr) ? true : false;
        }
       
};

template<class T>
long long heightCal(shared_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return NEGATIVE_HEIGHT;
    }
    return node->getHeight();
    
}

template<class T>
long long degreeCal(shared_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return NULL_degree;
    }
    return node->getDegree();
}

template<class T>
long long sumOfGradesUnderCalIncludingMe(shared_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return NULL_degree;
    }
    return node->getSumOfGradesUnder() + node->getData()->getGrade();
}

#endif //_NODE_H_

