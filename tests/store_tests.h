#include <check.h>
#include <stdlib.h>
#include "../src/store.h"

START_TEST(store_make)
{
    struct store s = make_store();

    ck_assert_ptr_eq(s.first, NULL);
}
END_TEST

START_TEST(store_find)
{
    struct store s = make_store();

    struct tuple t = { 3, { NODE_I(1), NODE_S("a"), NODE_I(3) } };
    append_store(&s, t);
}
END_TEST
