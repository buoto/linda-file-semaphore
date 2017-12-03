#include <check.h>
#include <stdlib.h>
#include "../src/tuple.h"
#include "../src/node.h"

START_TEST(tuple_make)
{
    tuple t = make_tuple();
    ck_assert_int_eq(t.size, 0);
}
END_TEST

START_TEST(tuple_append_one)
{
    tuple t = make_tuple();
    node n = make_int_node(5);
    int ret = tuple_append(&t, n);
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(t.size, 1);
    ck_assert_int_eq((int)t.elems[0].value, 5);
}
END_TEST

START_TEST(tuple_max_size)
{
    tuple t = make_tuple();
    node n = make_string_node("ff", 2);
    int i;
    for(i = 0; i < 5; i++)
        tuple_append(&t, n);
    ck_assert_int_eq(tuple_append(&t, n), -1);
    ck_assert_int_eq(t.size, 5);
    ck_assert_str_eq((char*)t.elems[4].value, "ff");
}
END_TEST
