#include <check.h>
#include <stdlib.h>
#include "../src/node.h"

START_TEST(node_make_int)
{
    struct node n = make_int_node(5);
    ck_assert_int_eq(n.type, INTEGER);
    ck_assert_int_eq((int)n.value, 5);
}
END_TEST

START_TEST(node_make_string)
{
    struct node n = make_string_node("abcd", 4);
    ck_assert_int_eq(n.type, STRING);
    ck_assert_str_eq((char *)n.value, "abcd");
}
END_TEST

START_TEST(node_destroy_string)
{
    struct node s = make_string_node("ABC", 3);
    destroy_node(&s);
    ck_assert_ptr_eq((char *)s.value, NULL);
}
END_TEST

START_TEST(node_destroy_int)
{
    struct node i = make_int_node(4);
    destroy_node(&i);
    ck_assert_ptr_eq(i.value, NULL);
}
END_TEST

struct pattern_matches_case {
    const char *pattern;
    const char *value;
    int expected;
} pattern_matches_cases[] = {
    { "", "", 1 },
    { "abc", "abc", 1 },
    { "*", "abc", 1 },
};
const int N_PATTERN_CASES = sizeof(pattern_matches_cases) / sizeof(struct pattern_matches_case);

START_TEST(node_pattern_matches)
{
    int result = pattern_matches(
        pattern_matches_cases[_i].pattern,
        pattern_matches_cases[_i].value
    );
    ck_assert_int_eq(result, pattern_matches_cases[_i].expected);
}
END_TEST
