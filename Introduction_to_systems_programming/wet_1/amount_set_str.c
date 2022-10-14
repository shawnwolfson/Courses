#include "amount_set_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NULL_POINTER -1

//Linked List
typedef struct node {
	char* element;
    double quantity;
	struct node* next;
} *Node;

//Set
struct AmountSet_t {
    Node head;
    Node current; //(iterator)
};



//***static functions for internal use***//

static Node createNode()
{
    Node new_node=malloc(sizeof(*new_node));
    if (new_node==NULL){
        return NULL;
    }
    new_node->element=NULL;
    new_node->quantity=0;
    new_node->next=NULL;
    return new_node;
}

static void destroyList(Node list)
{
	while(list){
		Node toDelete=list;
		list=list->next;
        toDelete->next=NULL;
        free(toDelete->element);
		free(toDelete);
	}
}

static void moveTail(Node* tail, Node* new_node)
{
    (*tail)->next=*new_node;
    *tail=*new_node;
}

static void fillNode(Node node_to_fill, char* str, int quantity, Node next_node)
{
    node_to_fill->element=str;
    node_to_fill->quantity=quantity;
    node_to_fill->next=next_node;
}

static char* copyString(const char* str)
{
    char* newStr = malloc(strlen(str) + 1);
    if (newStr==NULL){
        return NULL;
    }
    return strcpy(newStr, str);
}

static Node duplicateList(Node list)
{
    Node dummy=createNode();
    Node tail=dummy;
    while (list!=NULL){
        Node new_node=createNode();
        char* element_copy=copyString(list->element);
        if (new_node==NULL || element_copy==NULL){
            return NULL;
        }
        fillNode(new_node, element_copy, list->quantity, NULL);
        moveTail(&tail, &new_node);
        list=list->next;
    }
    Node return_node=dummy->next;
    free(dummy);
    return return_node;
}

static Node getNode(Node list, const char* element)
{
    Node temp=list;
    while (temp!=NULL){
        if (!(strcmp((temp->element), element))){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}

static Node getOneNodeBefore(Node list, const char* element)
{
    while ((list->next)!=NULL) {
        if (!strcmp(((list->next)->element), element)){
            return list;
        }
        list=list->next;
    }
    return NULL;
}

static AmountSetResult insertNodeLast(Node last_node, char* element)
{
    Node tail=last_node;
    Node new_node=createNode();
    if (new_node==NULL){
        return AS_OUT_OF_MEMORY;
    }
    fillNode(new_node, element, 0, NULL);
    tail->next=new_node;
    return AS_SUCCESS;
} 

static AmountSetResult insertNodeBefore(AmountSet set, char* str1, char* str2, Node current)
{
    Node list=set->head;
    if (current==list){// need to be the new head
        Node new_node=createNode();
        if (new_node==NULL){
            return AS_OUT_OF_MEMORY;
        }
        fillNode(new_node, str1, 0, list);
        set->head=new_node;
        return AS_SUCCESS;
    }
    Node temp=list;
    while (((temp->next)->element) != str2){
        temp=temp->next;
    }
    Node new_node=createNode();
    if (new_node==NULL){
        return AS_OUT_OF_MEMORY;
    }
    fillNode(new_node, str1, 0, temp->next);
    temp->next=new_node;
    return AS_SUCCESS;
}
//*******************************************//

//***interface functions***//

//Allocates a new empty amount set.
AmountSet asCreate()
{
    AmountSet new_set=malloc(sizeof(*new_set));
    if (new_set==NULL){
        return NULL;
    }
    new_set->head=NULL;
    new_set->current=NULL;
    return new_set;
}

//Deallocates an existing amount set. Clears all elements.
void asDestroy(AmountSet set)
{
    if (set==NULL){
        return;
    }
    destroyList(set->head);
    free(set);
}

//Creates a copy of target set.
AmountSet asCopy(AmountSet set)
{
    if (set==NULL){
        return NULL;
    }   
    AmountSet set_copy=asCreate();
    if (set_copy==NULL){
        return NULL;
    }
    set_copy->head=duplicateList(set->head);
    if (set_copy->head==NULL){
        return NULL;
    }
    set_copy->current=NULL;
    return set_copy;
}

//Returns the number of elements in a set.
int asGetSize(AmountSet set)
{
    if (set==NULL){
        return NULL_POINTER;
    }
    Node list=set->head;
    int counter=0;
    while (list!=NULL){
        counter++;
        list=list->next;
    }
    return counter;
}

 //Checks if an element exists in the set.
bool asContains(AmountSet set, const char* element)
{
    if (set==NULL || element==NULL){
        return false;
    }
    Node list=set->head;
    while (list!=NULL){
        if (!(strcmp((list->element), element))){
            return true;
        }
        list=list->next;
    }
    return false;
}

//Returns the amount of an element in the set.
AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if (set==NULL || element==NULL || outAmount==NULL){
        return AS_NULL_ARGUMENT;
    }
    if (!(asContains(set, element))){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    else{
        Node list=set->head;
        while (list!=NULL){
            if (!(strcmp(list->element, element))){
                *outAmount=list->quantity;
                return AS_SUCCESS;
            }
            list=list->next;
        }

    }
    return AS_ITEM_DOES_NOT_EXIST;
}

//Add a new element into the set. insertion will be sorted.
AmountSetResult asRegister(AmountSet set, const char* element)
{
    if (set==NULL || element==NULL){
        return AS_NULL_ARGUMENT;
    }
    if (asContains(set, element)){
        return AS_ITEM_ALREADY_EXISTS;
    }
    char* element_copy=copyString(element);
    if (element_copy==NULL){
        return AS_OUT_OF_MEMORY;
    }
    if (set->head==NULL){  //edge case, if set is empty
        Node new_head=createNode();
        if (new_head==NULL){
            return AS_OUT_OF_MEMORY;
        }
        set->head=new_head;
        fillNode(new_head, element_copy, 0, NULL);
        return AS_SUCCESS;
    }//
    Node list_i=set->head;
    while (list_i!=NULL)
    {
        if (strcmp(element_copy, list_i->element)>0){
            if (list_i->next==NULL){  //edge case, if last in set
                AmountSetResult result1=insertNodeLast(list_i, element_copy);
                if (result1==AS_OUT_OF_MEMORY){
                    return AS_OUT_OF_MEMORY;
                }
                return AS_SUCCESS;
            }//
            else{
                list_i=list_i->next;
                continue;
            }
        }
        else{
            AmountSetResult result2=insertNodeBefore(set, element_copy, list_i->element, list_i);
            if (result2==AS_OUT_OF_MEMORY){
                return AS_OUT_OF_MEMORY;
            }
            return AS_SUCCESS;
        }
    }
    return AS_SUCCESS;
}

//Increase or decrease the amount of an element in the set.
AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
    if (set==NULL || element==NULL){
        return AS_NULL_ARGUMENT;
    }
    if (!(asContains(set, element))){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Node list_ptr=getNode(set->head, element);
    if (((list_ptr->quantity)+amount) < 0) //there is no such a thing negative total amount
    {
        return AS_INSUFFICIENT_AMOUNT;
    }
    list_ptr->quantity=(list_ptr->quantity)+amount;
    return AS_SUCCESS;
}

//Delete an element completely from the set.
AmountSetResult asDelete(AmountSet set, const char* element)
{
    if (set==NULL || element==NULL){
        return AS_NULL_ARGUMENT;
    }
    if (!(asContains(set, element))){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Node node_to_delete=getNode(set->head, element);
    if (!strcmp(node_to_delete->element, (set->head)->element)){ // need to delete the first node in the list
        set->head=node_to_delete->next;
        free(node_to_delete->element);
        free(node_to_delete);
        return AS_SUCCESS;
    }
    Node one_node_before= getOneNodeBefore(set->head, node_to_delete->element);
    one_node_before->next=node_to_delete->next;
    free(node_to_delete->element);
    free(node_to_delete);
    return AS_SUCCESS;
}

//Deletes all elements from target set.
AmountSetResult asClear(AmountSet set)
{
    if (set==NULL){
        return AS_NULL_ARGUMENT;
    }
    destroyList(set->head);
    set->head=NULL;
    set->current=NULL;
    return AS_SUCCESS;
}

//Sets the internal iterator (also called current) to the first element in the set
char* asGetFirst(AmountSet set)
{
    if ((set==NULL) || (set->head==NULL)){
        return NULL;
    }
    set->current=set->head;
    return ((set->head)->element);
}

//Advances the set iterator to the next element and returns it.
char* asGetNext(AmountSet set)
{
    if ((set==NULL) || (set->current==NULL)){
        return NULL;
    }
    set->current=(set->current)->next;
    return (set->current)->element;
}
