#include "amount_set_str_tests.h"
#include "tests/test_utilities.h"

int main()
{
    RUN_TEST(testCreate);
    RUN_TEST(testDestroy);
    RUN_TEST(testRegister);
    RUN_TEST(testChangeAmount);
    RUN_TEST(testDelete);
    RUN_TEST(testContains);
    RUN_TEST(testGetSize);
    RUN_TEST(testCopy);
    RUN_TEST(testGetAmount);
    RUN_TEST(testClear);
    RUN_TEST(testGetFirst);
    RUN_TEST(testGetNext);

    return 0;
}
