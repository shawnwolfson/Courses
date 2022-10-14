#ifndef _TREEDATA_H_
#define _TREEDATA_H_
#define NEGATIVE_HEIGHT -1
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include "node.h"
#include "library1.h"

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
        int num_of_nodes;

        /********************************************************************************/
        /********************************************************************************/
        /*                           Auxiliary functions                                */
        /********************************************************************************/
        /********************************************************************************/

         /**
         * @brief recursive in-order travel. inserts the data of the tree to an array
         * @param array - an array with size of the num-of-node of the tree. the data of the
         *                  tree will be inserted to the array
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* inOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index) const
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

        /**
         * @brief limited recursive in-order travel.
         * @param array - an array with size of the num-of-node of the tree. the data of the
         *                  tree will be inserted to the array
         * @param limit - num of nodes that will be inserted to array.
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* limitedInOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index ,int limit) const
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

        /**
         * @brief recursive in-order travel. gets an array and inserts the data in the array to the tree
         * @param array - an array with size of the num-of-node of the tree. Includes the data that
         *                  will be inserted to the tree.
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* inOrderInsertRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index) const
        {
            if (current == nullptr)
            {
                return array;
            }
            array = inOrderInsertRecursion(array, current->left_son,index);
            current->setData(array[*index]);
            (*index) +=1 ;
            array = inOrderInsertRecursion(array, current->right_son,index);
            return array;
        }

        /**
         * @brief recursive in-order travel. an aux func for making empty tree. after building full tree,
         *          the function deletes num_to_delete leaves from the right to the left.
         * @param num_to_delete - num of leaves to delete
         * @return shared pointer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> revereseInOrderDeletionRecursion (shared_ptr<Node<T>> current, int* num_to_delete)
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
            return current;
        }

         /**
         * @brief recursive reverse-in-order travel. inserts the data of the tree to an array
         * @param array - an array with size of the num-of-node of the tree. the data of the
         *                  tree will be inserted to the array
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* reverseInOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index) const
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

         /**
         * @brief recursive post-order travel. inserts the data of the tree to an array
         * @param array - an array with size of the num-of-node of the tree. the data of the
         *                  tree will be inserted to the array
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* postOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index) const
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

         /**
         * @brief recursive pre-order travel. inserts the data of the tree to an array
         * @param array - an array with size of the num-of-node of the tree. the data of the
         *                  tree will be inserted to the array
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>* preOrderRecursion (shared_ptr<T>* array, shared_ptr<Node<T>> current, int* index) const
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

         /**
         * @brief recursive in-order travel, between 2 nodes in the tree. counts how many nodes there are from 
         *          one node to another
         * @param start - the smaller node 
         * @param end - the bigger node 
         * @return the num of the nodes
         */
        int countingSubTreeOrderRecursion (shared_ptr<Node<T>> current,  shared_ptr<T> start, shared_ptr<T> end)
        {
            if (current == nullptr)
            {
                return 0;
            }
            
            int left_value = 0;
            int current_value = 0;
            int right_value = 0;
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

        /**
         * @brief recursive in-order travel, between 2 nodes in the tree. inserts the data of the relevant nodes to an array
         * @param array - an array with size of the num-of-node between the 2 nodes. the data of the
         *                 tree will be inserted to the array
         * @param start - the smaller node 
         * @param end - the bigger node 
         * @return shared pointer to the the filled array
         */
        shared_ptr<T>*  fillSubTreeOrderRecursion (shared_ptr<T>* array, int* index , shared_ptr<Node<T>> current,  shared_ptr<T> start, shared_ptr<T> end)
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

        /**
         * @brief recursive pre-order travel, that resets all the nodes of the tree
         * @return nullptr
         */
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
         * @return shared pointer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> LL_rotate(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> new_root = current->left_son;
            current->left_son = new_root->right_son;
            new_root->right_son = current;
            current-> setHeight();
            new_root-> setHeight();
            if (current == head)
            {
                head = new_root; 
            }
            return new_root;
        }

        /**
         * @brief **O(1)** Making Right-Right rotation in accordance with the balance-factor (BF)
         * @return shared pointer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> RR_rotate(shared_ptr<Node<T>> current)
        {
            shared_ptr<Node<T>> new_root = current->right_son;
            current->right_son = new_root->left_son;
            new_root->left_son = current;
            current-> setHeight();
            new_root-> setHeight();
            if (current == head)
            {
                head = new_root; 
            }
            return new_root;
        }

        /**
         * @brief **O(1)** Making Left-Right rotation in accordance with the balance-factor (BF)
         * @return shared pointer to the head of the sub-tree that has done the rolling
         */
        shared_ptr<Node<T>> LR_rotate(shared_ptr<Node<T>> current)
        {
            current->left_son = RR_rotate(current->left_son);
            return LL_rotate(current);
        }

        /**
         * @brief **O(1)** Making Right-Left rotation in accordance with the balance-factor (BF)
         * @return shared pointer to the head of the sub-tree that has done the rolling
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
            return balanceFactorRotating(current);
        }

        /**
         * @brief recursive pre-order travel, that initialized empty full tree in a given height. 
         *          one node to another.
         * @return shared pointer to the head of the tree.
         */
        shared_ptr<Node<T>> createEmptyTreeRecursion(shared_ptr<Node<T>> current, int depth_counter, const int height)
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
         * @return shared pointer to the head of the tree
         */
        shared_ptr<Node<T>> getHead() const
        {
            return head;
        }
        /**
         * @brief O(1)
         * @return tree's number of nodes
         */
        int getSize() const
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

        /**
         * @brief O[log(num_of_nodes)]
         * @return finds and returns the required data. Functor needs to have Id-Node comparison
         * @return nullptr if not found
         */
        shared_ptr<T> findWithId(const int id)
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

        /**
         * @brief O[log(num_of_nodes)]
         * @return finds and returns the required data. Functor needs to have Salary-Id-Node comparison
         * @return nullptr if not found
         */
        shared_ptr<T> findWithSalary(const int salary, const int id)
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
        shared_ptr<T>* allTreeTravel (OrderType order, int limit = 0) const
        {
            shared_ptr<T>* t_arr = new shared_ptr<T>[num_of_nodes];
            int index =0;
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
         * @brief O[log(num_of_nodes) + nodes_between_start_to_stop]
         * @return return how many nodes there are between 2 different nodes in the tree
         */
        int countSubTreeTravel (shared_ptr<T> start,shared_ptr<T> stop)
        {
            return countingSubTreeOrderRecursion(head,start,stop);
        }

        /**
         * @brief Calling the fillSubTreeOrderRecursion from the head of the tree.
         * @return  a filled array of the sub tree content.
         */
        shared_ptr<T>* fillSubTreeTravel (shared_ptr<T> start,shared_ptr<T> stop, const int size) 
        {
            int index = 0;
            shared_ptr<T>* arr = new shared_ptr<T>[size];
            return fillSubTreeOrderRecursion(arr,&index, head,start,stop);
        }
            
        /**
         * @brief O(num_of_nodes)
         * @return deletes the tree. head is null at the end.
         */
        void destory ()
        {
            destroyRecursive(head);
            head = nullptr;
        }

        /**
         * @brief inserts a data to the tree
         */
        void insert (shared_ptr<T> current)
        {   
            insertRecursion(current, head);
        }

        /**
         * @brief removes a data to the tree
         */
        void remove (shared_ptr<T> current)
        {   
            removeRecursion(current, head);
        }

        /**
         * @brief Calling the inOrderInsertRecursion from the head of the tree, and index 0.
         *         fills the empty tree with from the content of array.
         */
        void fillInOrderEmptyTree(shared_ptr<T>* array)
        {
            int index = 0;
            inOrderInsertRecursion(array,head,&index);
        }
        
        /**
         * @brief creates an empty tree with a given num of nodes.
         */
        void createEmptyAlmostCompleteTree(const int merged_array_size)
        {
            int height = ceil(log2(merged_array_size+1)) - 1;
            int depth_counter = 0;
            int nodes_to_delete = pow(2, ceil(log2(merged_array_size+1))) - 1 - merged_array_size;
            createEmptyTreeRecursion(head,depth_counter,height);
            revereseInOrderDeletionRecursion(head,&nodes_to_delete);
        }

        /**
         * @return compareFunc.
         */
        CompareFunc getCompareFunc() const
        {
            return compare;
        }

        /**
         * @brief merge 2 trees into the cuurent tree (this).
         */
        void mergeTrees (Tree<T,CompareFunc>* tree1, Tree<T,CompareFunc>* tree2)
        {
            int size_of_tree_1 = tree1->getSize();
            int size_of_tree_2 = tree2->getSize();
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

        /**
         * @brief gets the closest from beneath data in the tree
         * @return shared pointer to the relevant data
         */
        shared_ptr<T> find_infimum_of_node_in_tree(const int data)
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

        /**
         * @brief gets the closest from above data in the tree
         * @return shared pointer to the relevant data
         */
        shared_ptr<T> find_supremum_of_node_in_tree(const int data)
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
};

/**
* @brief gets 2 sorted arrays with tree datas, and merge them to a single new array. 
* @return pointer to the merged array (needed to be deleted outside).
*/
template <class T, class CompareFunc>
shared_ptr<T>* mergeTwoArrays(shared_ptr<T>* array1,const int size1, shared_ptr<T>* array2,const int size2, CompareFunc compare)
{
    shared_ptr<T>* merge = new shared_ptr<T>[size1+size2];
    int arr1_index = 0, arr2_index = 0, merge_index = 0;
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