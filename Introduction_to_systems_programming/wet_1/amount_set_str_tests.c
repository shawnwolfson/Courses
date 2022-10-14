#include "amount_set_str_tests.h"
#include "amount_set_str.h"
#include "tests/test_utilities.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ASSERT_OR_DESTROY(expr) ASSERT_TEST_WITH_FREE((expr), asDestroy(set))

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

bool testCreate()
{
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    asDestroy(set);
    return true;
}

bool testDestroy()
{
    AmountSet set = asCreate();
    asDestroy(set);
    /* try to elicit undefined behavior in case matamikyaDestroy has bugs */
    AmountSet set2 = asCreate();
    ASSERT_TEST(set2 != NULL);
    asDestroy(set2);
    return true;
}

bool testRegister()
{
    AmountSet set = asCreate();
    char str1[] = "dogs";
    char str2[] = "cats";
    //try NULL set
    ASSERT_OR_DESTROY(asRegister(NULL, str1) == AS_NULL_ARGUMENT);

    ASSERT_OR_DESTROY(asRegister(set, str1) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asRegister(set, str2) == AS_SUCCESS);

    ASSERT_OR_DESTROY(asRegister(set, str1) == AS_ITEM_ALREADY_EXISTS);
    ASSERT_OR_DESTROY(asRegister(set, str2) == AS_ITEM_ALREADY_EXISTS);

    //check head was changed
    ASSERT_OR_DESTROY(strcmp(set->head->element, str2) == 0);

    asDestroy(set);
    return true;
}

static void makeSet(AmountSet set)
{
    asRegister(set, "dogs");
    asRegister(set, "cats");
    asRegister(set, "otters");
    asRegister(set, "fishes");
    asRegister(set, "giraffes");
}

bool testChangeAmount() 
{
    AmountSet set = asCreate();
    makeSet(set);

    ASSERT_OR_DESTROY(asChangeAmount(set,"squirrels", 0) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_OR_DESTROY(asRegister(set, "squirrels") == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set,"squirrels", 5) == AS_SUCCESS);

    ASSERT_OR_DESTROY(asChangeAmount(set, "giraffes", -5) == AS_INSUFFICIENT_AMOUNT);

    ASSERT_OR_DESTROY(asChangeAmount(set, "giraffes", 5) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, "fishes", 3) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, "otters", 11) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, "cats", 15) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, "dogs", 120) == AS_SUCCESS);

    Node iterator = set->current;
    ASSERT_OR_DESTROY(asChangeAmount(set, "giraffes", 5) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, "otters", -20) == AS_INSUFFICIENT_AMOUNT);
    ASSERT_OR_DESTROY(asChangeAmount(set, "dogs", 2) == AS_SUCCESS);
    ASSERT_OR_DESTROY(iterator == set->current);

    asDestroy(set);
    return true;
}

bool testDelete()
{
    AmountSet set = asCreate();
    makeSet(set);

    ASSERT_OR_DESTROY(asDelete(NULL, "dogs") == AS_NULL_ARGUMENT);

    ASSERT_OR_DESTROY(asDelete(set, "dogs") == AS_SUCCESS);
    ASSERT_OR_DESTROY(asDelete(set, "dogs") == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_OR_DESTROY(asDelete(set, "fishes") == AS_SUCCESS);
    ASSERT_OR_DESTROY(asDelete(set, "fishes") == AS_ITEM_DOES_NOT_EXIST);

    //changed the head?
    ASSERT_OR_DESTROY(strcmp(set->head->element, "cats") == 0);

    asDestroy(set);
    return true;

}

bool testContains()
{
    AmountSet set = asCreate();
    makeSet(set);

    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);
    ASSERT_OR_DESTROY(asContains(set, "cats"));

    Node iterator = set->current;
    ASSERT_OR_DESTROY(asContains(set, "dogs"));
    //check iterator not touched
    ASSERT_OR_DESTROY(iterator == set->current);

    ASSERT_OR_DESTROY(asContains(set, "giraffes"));
    asDelete(set, "giraffes");
    ASSERT_OR_DESTROY(!asContains(set, "giraffes"));

    asDestroy(set);
    return true;
}

bool testGetSize()
{
    AmountSet set = asCreate();
    makeSet(set);

    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);

    Node iterator = set->current;
    ASSERT_OR_DESTROY(asGetSize(set) == 5);
    ASSERT_OR_DESTROY(iterator == set->current);

    ASSERT_OR_DESTROY(asDelete(set, "fishes") == AS_SUCCESS);
    ASSERT_OR_DESTROY(asGetSize(set) == 4);

    asDestroy(set);
    return true;
}

bool testCopy()
{
    AmountSet set = asCreate();
    makeSet(set);

    AmountSet set_copy = asCopy(set);

    ASSERT_OR_DESTROY(asGetSize(set) == asGetSize(set_copy));

    ASSERT_OR_DESTROY(asContains(set_copy, "cats"));
    ASSERT_OR_DESTROY(asContains(set_copy, "otters"));
    
    asDestroy(set);
    asDestroy(set_copy);
    return true;

}

bool testGetAmount() {
    AmountSet set = asCreate();
    makeSet(set);

    double amount = 0;
    ASSERT_OR_DESTROY(asGetAmount(NULL, "dogs", &amount) == AS_NULL_ARGUMENT);
    ASSERT_OR_DESTROY(amount == 0);

    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);

    Node iterator = set->current;
    ASSERT_OR_DESTROY(asChangeAmount(set, "giraffes", 5) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asGetAmount(set, "abc", &amount) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_OR_DESTROY(asGetAmount(set, "giraffes", &amount) == AS_SUCCESS);
    ASSERT_OR_DESTROY(amount == 5.0);

    //iterator is not touched
    ASSERT_OR_DESTROY(iterator == set->current);

    asDestroy(set);

    return true;
}



bool testClear()
{
    AmountSet set = asCreate();
    makeSet(set);

    ASSERT_OR_DESTROY(asClear(NULL) == AS_NULL_ARGUMENT);

    ASSERT_OR_DESTROY(asClear(set) == AS_SUCCESS);
    ASSERT_OR_DESTROY(!asContains(set, "dogs"));
    ASSERT_OR_DESTROY(!asContains(set, "cats"));

    asDestroy(set);
    return true;

}


bool testGetFirst()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(asGetFirst(set) == NULL);
    makeSet(set);

    ASSERT_OR_DESTROY(asGetFirst(NULL) == NULL);
    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);
    ASSERT_OR_DESTROY(asDelete(set, "cats") == AS_SUCCESS);
    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "dogs") == 0);

    asDestroy(set);
    return true;
}

bool testGetNext()
{
    AmountSet set = asCreate();
    ASSERT_OR_DESTROY(asGetNext(set) == NULL);
    makeSet(set);
    ASSERT_OR_DESTROY(asGetNext(NULL) == NULL);

    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);
    ASSERT_OR_DESTROY(strcmp(asGetFirst(set), "cats") == 0);
    ASSERT_OR_DESTROY(strcmp(asGetNext(set), "dogs") == 0);

    asDestroy(set);
    return true;

}