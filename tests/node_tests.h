#include <check.h>
#include <stdlib.h>
#include "../src/node.h"

START_TEST(sum2test)
{
    fail_unless(3+ 2 == 5, "sum function borked");
}
END_TEST
