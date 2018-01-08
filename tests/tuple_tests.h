#include <check.h>
#include <stdlib.h>
#include "../src/tuple.h"
#include "../src/node.h"

START_TEST(tuple_make)
{
    struct tuple t = make_tuple();
    ck_assert_int_eq(t.size, 0);
}
END_TEST

START_TEST(tuple_append_one)
{
    struct tuple t = make_tuple();
    struct node n = make_int_node(5);
    int ret = tuple_append(&t, n);
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(t.size, 1);
    ck_assert_int_eq(t.elems[0].int_value, 5);
}
END_TEST

START_TEST(tuple_max_size)
{
    struct tuple t = make_tuple();
    struct node n = make_string_node("ff", 2);
    int i;
    for(i = 0; i < 5; i++)
        tuple_append(&t, n);
    ck_assert_int_eq(tuple_append(&t, n), -1);
    ck_assert_int_eq(t.size, 5);
    ck_assert_str_eq(t.elems[4].str_value, "ff");
}
END_TEST


struct match_tuple_case {
    struct tuple pattern;
    struct tuple value;
    int expected;
} match_tuple_cases[] = {
    {
        { 3, { NODE_I(1), NODE_S("a*"), NODE_IM(3, GREATER) } },
        { 3, { NODE_I(1), NODE_S("abc"), NODE_I(5) } },
        true
    },
    {
        { 3, { NODE_I(1), NODE_S("a*"), NODE_IM(3, GREATER) } },
        { 3, { NODE_I(2), NODE_S("abc"), NODE_I(5) } },
        false
    },
    {
        { 3, { NODE_I(1), NODE_I(2), NODE_I(3) } },
        { 2, { NODE_I(1), NODE_I(2) } },
        false
    }
};
const int N_TUPLE_CASES = sizeof(match_tuple_cases) / sizeof(struct match_tuple_case);

START_TEST(tuple_match_tuple)
{
    int result = match_tuple(
        &match_tuple_cases[_i].pattern,
        &match_tuple_cases[_i].value
    );
    ck_assert_int_eq(result, match_tuple_cases[_i].expected);
}
END_TEST
