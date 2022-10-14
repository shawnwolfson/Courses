#ifndef _TREEDATA_H_
#define _TREEDATA_H_
#define NEGATIVE_HEIGHT -1
#define NEGATIVE_RANK -1
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include "node.h"

using std::shared_ptr;
using std::max;
using std::ceil;
using std::pow;

enum CompareResult {LARGER_THAN, SMALLER_THAN, EQUAL};
enum OrderType {INORDER, POSTORDER, PREORDER, REVERSE_INORDER, LIMITED_INORDER};


template <class T , class CompareFunc>
class Tree
{
    private:
        shared_ptr<Node<T>> head;
        CompareFunc compare;
        long long num_of_nodes;

        /********************************************************************************/
        /********************************************************************************/
        /*                           Auxiliary functions                                */
        /********************************************************************************/
        /********************************************************************************/

        shared_ptr<T>* inOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array = inOrderRecursion(array, current->left_son,index);
            array[*index] = current->getData();
            (*index) +=1 ;
            array = inOrderRecursion(array, current->right_son,index);
            return array;
        }

        shared_ptr<T>* limitedInOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index ,long long limit) const
        {
            if (current == nullptr || *index == limit)
            {
                return array;
            }
            array = inOrderRecursion(array, current->left_son,index);
            array[*index] = current->getData();
            (*index) +=1 ;
            array = inOrderRecursion(array, current->right_son,index);
            return array;
        }

        shared_ptr<T>* inOrderInsertRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array = inOrderInsertRecursion(array, current->left_son,index);
            current->setData(array[*index]);
            (*index) +=1 ;
            array = inOrderInsertRecursion(array, current->right_son,index);
            current->setSumOfGradesUnder();
            return array;
        }

        shared_ptr<Node<T>> revereseInOrderDeletionRecursion (shared_ptr<Node<T>> current, long long* num_to_delete)
        {
            if (*num_to_delete == 0 || current == nullptr)
            {
                return current;
            }
            current->right_son = revereseInOrderDeletionRecursion(current->right_son,num_to_delete);
            if (current->checkIfNodeIsLeaf())
            {
                current.reset();
                *(num_to_delete) -= 1;
                num_of_nodes--;
                return nullptr;
            }
        
            current->left_son = revereseInOrderDeletionRecursion(current->left_son,num_to_delete);
            current->setHeight();
            current->setDegree();
            //current->setSumOfGradesUnder();
            return current;
        }

        shared_ptr<T>* reverseInOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array = reverseInOrderRecursion(array, current->right_son,index);
            array[*index] = current->getData();
            (*index) +=1 ;
            array = reverseInOrderRecursion(array, current->left_son,index);
            return array;
        }

        shared_ptr<T>* postOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array = postOrderRecursion(array, current->left_son,index);
            array = postOrderRecursion(array, current->right_son,index);
            array[*index] = current->getData();
            (*index)+=1;
            return array;
        }
        shared_ptr<T>* preOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, long long* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array[*index] = current->getData();
            (*index) +=1;
            array = preOrderRecursion(array, current->left_son,index);
            array = preOrderRecursion(array, current->right_son,index);
            return array;
        }

        long long countingSubTreeOrderRecursion (shared_ptr<Node<T>> current,  shared_ptr<T> start, shared_ptr<T> end)
        {
            if (current == nullptr)
            {
                return 0;
            }
            
            long long left_value = 0;
            long long current_value = 0;
            long long right_value = 0;
            CompareResult compare_start_to_current =  compare(start,current);
            CompareResult compare_end_to_current =  compare(end,current);

            if (compare_start_to_current == SMALLER_THAN)
            {   
        
              left_value = countingSubTreeOrderRecursion(current->left_son, start,end);
                
            }

            if (compare_start_to_current != LARGER_THAN && compare_end_to_current != SMALLER_THAN)
            {
                current_value = 1;
            }

            if (compare_end_to_current == LARGER_THAN)
            {
                   right_value = countingSubTreeOrderRecursion(current->right_son, start,end);  
            }
            return left_value + current_value + right_value;
        }
        shared_ptr<T>*  fillSubTreeOrderRecursion (shared_ptr<T>* array, long long* index , shared_ptr<Node<T>> current,  shared_ptr<T> start, shared_ptr<T> end)
        {
            if (current == nullptr)
            {
                return array;
            }
           
            CompareResult compare_start_to_current =  compare(start,current);
            CompareResult compare_end_to_current =  compare(end,current);

            if (compare_start_to_current == SMALLER_THAN)
            {   
        
              array = fillSubTreeOrderRecursion(array,index,current->left_son, start,end);
                
            }

            if (compare_start_to_current != LARGER_THAN && compare_end_to_current != SMALLER_THAN)
            {
                array[*index] = current->getData();
                (*index) += 1;
            }

            if (compare_end_to_current == LARGER_THAN)
            {
                array = fillSubTreeOrderRecursion(array,index,current->right_son, start,end);  
            }
            return array;
        }

        shared_ptr<Node<T>> destroyRecursive(shared_ptr<Node<T>> current)
        {
            if (current == nullptr)
            {
                return nullptr;
            }

            current->left_son = destroyRecursive(current->left_son);
            current->right_son = destroyRecursive(current->right_son);
            current.reset();
            num_of_nodes--;
            return nullptr;
        }

        /**
         * @brief **O(1)** Making Left-Left rotation in accordance with the balance-factor (BF)
         * @return shared polong longer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> LL_rotate(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> new_root = current->left_son;
            current->left_son = new_root->right_son;
            new_root->right_son = current;
            current-> setHeight();
            current->setDegree();
            current->setSumOfGradesUnder();
            new_root-> setHeight();
            new_root->setDegree();
            new_root->setSumOfGradesUnder();
            if (current == head)
            {
                head = new_root; 
            }
            return new_root;
        }

        /**
         * @brief **O(1)** Making Right-Right rotation in accordance with the balance-factor (BF)
         * @return shared polong longer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> RR_rotate(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> new_root = current->right_son;
            current->right_son = new_root->left_son;
            new_root->left_son = current;
            current-> setHeight();
            current->setDegree();
            current->setSumOfGradesUnder();
            new_root-> setHeight();
            new_root->setDegree();
            new_root->setSumOfGradesUnder();
            if (current == head)
            {
                head = new_root; 
            }
            return new_root;
        }

        /**
         * @brief **O(1)** Making Left-Right rotation in accordance with the balance-factor (BF)
         * @return shared polong longer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> LR_rotate(shared_ptr<Node<T>> current)
        {
            current->left_son = RR_rotate(current->left_son);
            return LL_rotate(current);
        }

        /**
         * @brief **O(1)** Making Right-Left rotation in accordance with the balance-factor (BF)
         * @return shared polong longer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> RL_rotate(shared_ptr<Node<T>> current)
        {
           current->right_son = LL_rotate(current->right_son);
           return RR_rotate(current);
        }

        /**
         * @brief O(1)
         * @return remove node with the one left son. returns current left son
         */
        shared_ptr<Node<T>> removeOneSonLeft(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> temp = nullptr;
            temp = current->left_son;
            if (current == head)
            {
                head = temp;
            }
            current.reset();
            num_of_nodes--;
            return temp;
        }

         /**
         * @brief O(1)
         * @return remove node with the one right son. returns current right son
         */
        shared_ptr<Node<T>> removeOneSonRight(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> temp = nullptr;
            temp = current->right_son;
            if (current == head)
            {
                head = temp;
            }
            current.reset();
            num_of_nodes--;
            return temp;
        }

         /**
         * @brief O(1)
         * @return remove node with no sons. returns nullptr
         */
        shared_ptr<Node<T>> removeNoSons(shared_ptr<Node<T>> current)
        {
            if (current == head)
            {
                head = nullptr;
            }
            current.reset();
            num_of_nodes--;
            return nullptr;
        }

        /**
         * @brief O[log(num_of_nodes)]
         * @return remove node with two sons.
         */
        void removeTwoSons(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> temp = getMinimumNode(current->right_son);
            current->data = temp->getData();
            current->right_son = removeRecursion(temp->getData(), current->right_son);
        }

         /**
         * @brief O[log(num_of_nodes)] 
         * @return Recursive Insertion of a new Node. In the end of the recursion, returns the haed of the tree
         */
        shared_ptr<Node<T>> insertRecursion(shared_ptr<T> data, shared_ptr<Node<T>> current)
        {
            if(current == NULL)
            {
                shared_ptr<Node<T>> temp(new Node<T>());
                temp->setData(data);
                if (current == head)
                {
                head = temp; 
                }
                current = temp;
                num_of_nodes++;
                return current;
            }
            else if(compare(data, current) == SMALLER_THAN)
            {
                current->left_son = insertRecursion(data, current->left_son);
            }
            else if(compare(data, current) == LARGER_THAN)
            {
                current->right_son = insertRecursion(data, current->right_son);
            }

            current = balanceFactorRotating(current);
            current -> setHeight();
            current->setDegree();
            current->setSumOfGradesUnder();
            return current;
        }
         /**
         * @brief O[log(num_of_nodes)]
         * @return remove node with the required data. returns the head of the tree.
         */
        shared_ptr<Node<T>> removeRecursion(shared_ptr<T> data, shared_ptr<Node<T>> current)
        {
            if(current == nullptr)
            {
                return nullptr;
            }
            if(compare(data, current) == SMALLER_THAN)
            {
                current->left_son = removeRecursion(data, current->left_son);
            }
            else if(compare(data, current) == LARGER_THAN)
            {
                current->right_son = removeRecursion(data, current->right_son);
            }
            else //reached the node to be deleted
            {
                NumOfChilds childs_type = current->getNumOfChilds();
                switch (childs_type)
                {
                    case NONE:
                        return removeNoSons(current);
                        break;
                    case ONE_RIGHT:
                        return removeOneSonRight(current);
                        break;
                    case ONE_LEFT:
                        return removeOneSonLeft(current);
                        break;
                    case TWO:
                        removeTwoSons(current);
                        break;
                }
            }
            current-> setHeight();
            current->setDegree();
            current->setSumOfGradesUnder();
            return balanceFactorRotating(current);
        }

        shared_ptr<Node<T>> createEmptyTreeRecursion(shared_ptr<Node<T>> current, long long depth_counter, const long long height)
        {
            if(current == nullptr)
            {
                shared_ptr<Node<T>> node(new Node<T>);
                if (depth_counter == 0)
                {
                    head = node;
                }
                current = node;
                num_of_nodes++;
            }

            if(depth_counter == height)
            {
                return current;
            }
            
            current->left_son = createEmptyTreeRecursion(current->left_son, depth_counter + 1 , height);
            current->right_son = createEmptyTreeRecursion(current->right_son, depth_counter + 1 , height);
            current -> setHeight();
            current->setDegree();
            //current->setSumOfGradesUnder();
            return current;
        }


        /********************************************************************************/
        /********************************************************************************/
        /*                                  Methods                                     */
        /********************************************************************************/
        /********************************************************************************/

    public:
        /**
         * @brief O(1)
         * @param func - compare function for the Nodes
         * @return Construct a new AVL Tree object
         */
        Tree(const CompareFunc& func) : head(nullptr) , compare(func), num_of_nodes(0){};

        /**
         * @brief O(1)
         * @return shared polong longer to the head of the tree
         */
        shared_ptr<Node<T>> getHead() const
        {
            return head;
        }
        /**
         * @brief O(1)
         * @return tree's number of nodes
         */
        long long getSize() const
        {
            return num_of_nodes;
        }

        /**
         * @brief O(1)
         * @return checks BF and do rotation as needed. return 'current' or the head of the sub tree that did rolling.
         */
        shared_ptr<Node<T>> balanceFactorRotating (shared_ptr<Node<T>> current)
        {
            if(!(current->checkBF()))
                {
                    RollingCase roll_case = current->getRollingCase();
                    switch (roll_case)
                    {
                        case CASE_LL:
                            current = LL_rotate(current);
                            break ;
                        case CASE_LR:
                            current = LR_rotate(current);
                            break;
                        case CASE_RR:
                            current = RR_rotate(current);
                            break;
                        case CASE_RL:
                            current = RL_rotate(current);
                            break;
                    }
                }
            return current;
        }

       

        /**
         * @brief O[log(num_of_nodes)]
         * @return gets the left-most son of the tree
         */
        shared_ptr<Node<T>> getMinimumNode(shared_ptr<Node<T>> current)
        {
            while(current->left_son != nullptr)
            {
                current = current->left_son;
            }
            return current;
        }

        /**
         * @brief O[log(num_of_nodes)]
         * @return gets the right-most son of the tree
         */
        shared_ptr<Node<T>> getMaximumNode(shared_ptr<Node<T>> current)
        {
            while(current->right_son != nullptr)
            {
                current = current->right_son;
            }
            return current;
        }

        /**
         * @brief O[log(num_of_nodes)]
         * @return finds and returns the Node with the required data.
         * @return nullptr if not found
         */
        shared_ptr<Node<T>> find(const T& data)
        {
            shared_ptr<Node<T>> temp = head;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    return temp;
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    temp = temp->right_son;
                }
            }

            return nullptr;
        }


        shared_ptr<T> findWithId(const long long id)
        {
            shared_ptr<Node<T>> temp = head;
            while(temp != nullptr)
            {
                if(compare(id, temp) == EQUAL)
                {
                    return temp->getData();
                }
                else if(compare(id, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    temp = temp->right_son;
                }
            }

            return nullptr;
        }

        shared_ptr<T> selectRank(long long rank)
        {
            if (rank <= 0 || rank >= num_of_nodes+1)
            {
                return nullptr;
            } 
            shared_ptr<Node<T>> temp = head;
            while(temp != nullptr)
            {
                if(degreeCal(temp->left_son) == rank-1)
                {
                    return temp->getData();
                }
                else if(degreeCal(temp->left_son) > rank-1)
                {
                    temp = temp->left_son;
                }
                else
                {
                    rank = rank- degreeCal(temp->left_son) - 1;
                    temp = temp->right_son;
                }
            }

            return nullptr;
        }
        shared_ptr<Node<T>> getNodeInRank(long long rank)
        {
            if (rank <= 0 || rank >= num_of_nodes+1)
            {
                return nullptr;
            } 
            shared_ptr<Node<T>> temp = head;
            while(temp != nullptr)
            {
                if(degreeCal(temp->left_son) == rank-1)
                {
                    return temp;
                }
                else if(degreeCal(temp->left_son) > rank-1)
                {
                    temp = temp->left_son;
                }
                else
                {
                    rank = rank- degreeCal(temp->left_son) - 1;
                    temp = temp->right_son;
                }
            }

            return nullptr;
        }
        long long  getSumOfGradesAboveNodeInRank(long long rank)
        {
            if (rank < 0 || rank >= num_of_nodes+1)
            {
                return 0;
            }
            long long  grades_above = 0;
            shared_ptr<Node<T>> temp = head;
            if (rank == 0)
            {
                return sumOfGradesUnderCalIncludingMe(temp);
            }
            while(temp != nullptr)
            {
                if(degreeCal(temp->left_son) == rank-1)
                {
                    return (grades_above + sumOfGradesUnderCalIncludingMe(temp->right_son));
                }
                else if(degreeCal(temp->left_son) > rank-1)
                {
                    grades_above += (sumOfGradesUnderCalIncludingMe(temp->right_son) + temp->getData()->getGrade());
                    temp = temp->left_son;
                }
                else
                {
                    rank = rank- degreeCal(temp->left_son) - 1;
                    temp = temp->right_son;
                }
            }

            return grades_above;
        }

        long long getRankOfEmployee(const shared_ptr<T> data)
        {
            shared_ptr<Node<T>> temp = head;
            long long temp_value = 0;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    return temp_value + degreeCal(temp->left_son) + 1;
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    temp_value = temp_value + degreeCal(temp->left_son) + 1;
                    temp = temp->right_son;
                }
            }

            return NEGATIVE_RANK;
        }

        shared_ptr<T> findWithSalary(const long long salary, const long long id)
        {
            shared_ptr<Node<T>> temp = head;
            while(temp != nullptr)
            {
                if(compare(salary, id, temp) == EQUAL)
                {
                    return temp->getData();
                }
                else if(compare(salary,id, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    temp = temp->right_son;
                }
            }

            return nullptr;
        }

    
        /**
         * @brief O(num_of_nodes)
         * @return return a new array containing node's data, according the required order
         */
        shared_ptr<T>* allTreeTravel (OrderType order, long long limit = 0) const
        {
            shared_ptr<T>* t_arr = new shared_ptr<T>[num_of_nodes];
            long long index =0;
            switch (order)
            {
            case INORDER:
                return inOrderRecursion(t_arr,head,&index);
                break;
            case PREORDER:
                return preOrderRecursion(t_arr,head,&index);
                break;
            case POSTORDER:
                return postOrderRecursion(t_arr,head,&index);
                break;
            case REVERSE_INORDER:
                return reverseInOrderRecursion(t_arr,head,&index);
                break;
            case LIMITED_INORDER:
                return limitedInOrderRecursion(t_arr,head,&index,limit);
                break;
            
            default:
                delete[] t_arr;
                return nullptr;
                break;
            }
        }

       
        /**
         * @brief O[log(num_of_nodes) + nodes_between_current_to_stop]
         * @return return how many nodes there are between 2 different nodes in the tree
         */
        long long countSubTreeTravel (shared_ptr<T> start,shared_ptr<T> stop)
        {
            return countingSubTreeOrderRecursion(head,start,stop);
        }

        shared_ptr<T>* fillSubTreeTravel (shared_ptr<T> start,shared_ptr<T> stop, const long long size) 
        {
            long long index = 0;
            shared_ptr<T>* arr = new shared_ptr<T>[size];
            return fillSubTreeOrderRecursion(arr,&index, head,start,stop);
        }
            
        /**
         * @brief O(num_of_nodes) -> delete node O(1) * num_of_nodes
         * @return deletes the tree. head is null at the end.
         */
        void destory ()
        {
        destroyRecursive(head);
        head = nullptr;
        }

        void insert (shared_ptr<T> current)
        {   
            insertRecursion(current, head);
        }

        void remove (shared_ptr<T> current)
        {   
            removeRecursion(current, head);
        }



        void fillInOrderEmptyTree(shared_ptr<T>* array)
        {
            long long index = 0;
            inOrderInsertRecursion(array,head,&index);
        }
        
        void createEmptyAlmostCompleteTree(const long long merged_array_size)
        {
            long long height = ceil(log2(merged_array_size+1)) - 1;
            long long depth_counter = 0;
            long long nodes_to_delete = pow(2, ceil(log2(merged_array_size+1))) - 1 - merged_array_size;
            createEmptyTreeRecursion(head,depth_counter,height);
            revereseInOrderDeletionRecursion(head,&nodes_to_delete);
        }

        CompareFunc getCompareFunc() const
        {
            return compare;
        }

        void mergeTrees (Tree<T,CompareFunc>* tree1, Tree<T,CompareFunc>* tree2)
        {
            long long size_of_tree_1 = tree1->getSize();
            long long size_of_tree_2 = tree2->getSize();
            if (size_of_tree_1 == 0 && size_of_tree_2 ==0)
            {
                return;
            }
            shared_ptr<T>* array1 = tree1->allTreeTravel(INORDER);

            shared_ptr<T>* array2 = tree2->allTreeTravel(INORDER);

            shared_ptr<T>* merged_array = mergeTwoArrays(array1,size_of_tree_1,array2,size_of_tree_2,tree1->getCompareFunc());
            this->createEmptyAlmostCompleteTree(size_of_tree_1 + size_of_tree_2);
            this->fillInOrderEmptyTree(merged_array);

            delete[] array1;
            delete[] array2;
            delete[] merged_array;
        }       

         shared_ptr<T> find_infimum_of_node_in_tree(const long long data)
        {
            shared_ptr<Node<T>> temp = head;
            shared_ptr<Node<T>> candidate = nullptr;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    return temp->getData();
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    candidate = temp;
                    temp = temp->right_son;
                }
            }
            if (candidate == nullptr)
            {
                return nullptr;
            }
            
            return candidate->getData();
        }

        shared_ptr<T> improved_find_infimum_of_node_in_tree(const long long data)
        {
            shared_ptr<Node<T>> temp = head;
            shared_ptr<Node<T>> candidate = nullptr;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    candidate = temp;
                    temp = temp->right_son;
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    temp = temp->left_son;
                }
                else
                {
                    candidate = temp;
                    temp = temp->right_son;
                }
            }
            if (candidate == nullptr)
            {
                return nullptr;
            }
            
            return candidate->getData();
        }

        shared_ptr<T> find_supremum_of_node_in_tree(const long long data)
        {
            shared_ptr<Node<T>> temp = head;
            shared_ptr<Node<T>> candidate = nullptr;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    return temp->getData();
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    candidate = temp;
                    temp = temp->left_son;
                }
                else
                {
                    temp = temp->right_son;
                }
            }
            if (candidate == nullptr)
            {
                return nullptr;
            }
            

            return candidate->getData();
        }
        shared_ptr<T> improved_find_supremum_of_node_in_tree(const long long data)
        {
            shared_ptr<Node<T>> temp = head;
            shared_ptr<Node<T>> candidate = nullptr;
            while(temp != nullptr)
            {
                if(compare(data, temp) == EQUAL)
                {
                    candidate = temp;
                    temp = temp->left_son;
                }
                else if(compare(data, temp) == SMALLER_THAN)
                {
                    candidate = temp;
                    temp = temp->left_son;
                }
                else
                {
                    temp = temp->right_son;
                }
            }
            if (candidate == nullptr)
            {
                return nullptr;
            }
            

            return candidate->getData();
        }
};

template <class T, class CompareFunc>
shared_ptr<T>* mergeTwoArrays(shared_ptr<T>* array1,const long long size1, shared_ptr<T>* array2,const long long size2, CompareFunc compare)
{
    shared_ptr<T>* merge = new shared_ptr<T>[size1+size2];
    long long arr1_index = 0, arr2_index = 0, merge_index = 0;
    while (arr1_index < size1 && arr2_index < size2)
    {
        if (compare((array1[arr1_index]),(array2[arr2_index])) == SMALLER_THAN)
        {
            merge[merge_index] = array1[arr1_index];
            merge_index++;
            arr1_index++;
        }
        else
        {
            merge[merge_index] = array2[arr2_index];
            merge_index++;
            arr2_index++;
        }
    }

    while (arr1_index < size1)
    {
        merge[merge_index] = array1[arr1_index];
        merge_index++;
        arr1_index++;
    }
    
     while (arr2_index < size2)
    {
        merge[merge_index] = array2[arr2_index];
        merge_index++;
        arr2_index++;
    }

    return merge;
}





#endif //_TREEDATA_H_