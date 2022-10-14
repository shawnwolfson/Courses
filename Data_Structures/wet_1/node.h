#ifndef _NODE_H_
#define _NODE_H_
#define NEGATIVE_HEIGHT -1
#include <iostream>
#include <string>
#include <memory>
#include <math.h>

using std::shared_ptr;
using std::max;

enum RollingCase {CASE_RR, CASE_LL, CASE_RL, CASE_LR};
enum NumOfChilds {NONE,ONE_RIGHT,ONE_LEFT,TWO};

template <class T>
class Node
{
    private:
        shared_ptr<T> data;
        int height = 0;
        shared_ptr<Node> left_son;
        shared_ptr<Node> right_son;
        
    
    public:
        Node(shared_ptr<T> data) : data(data), height(0), left_son(nullptr), right_son(nullptr) {};
        Node() : data(nullptr), height(0), left_son(nullptr), right_son(nullptr) {};

        template <class S, class CompareFunc> friend class Tree;

        void insertLeftSon(shared_ptr<Node> son)
        {
            left_son = son;
        }

        void insertRightSon(shared_ptr<Node> son)
        {
            right_son = son;
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
            int balance_factor = heightCal(left_son) - heightCal(right_son);
            int left_son_bf = 0;
            int right_son_bf = 0;
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
        
        int getHeight() const
        {
            return this->height;
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
int heightCal(shared_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return NEGATIVE_HEIGHT;
    }
    return node->getHeight();
}

#endif //_NODE_H_

