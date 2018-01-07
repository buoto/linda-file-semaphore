#include <check.h>
#include <stdlib.h>
#include "../src/node.h"

START_TEST(node_make_int)
{
    struct node n = make_int_node(5);
    ck_assert_int_eq(n.type, INTEGER);
    ck_assert_int_eq(n.int_value, 5);
}
END_TEST

START_TEST(node_make_string)
{
    struct node n = make_string_node("abcd", 4);
    ck_assert_int_eq(n.type, STRING);
    ck_assert_str_eq(n.str_value, "abcd");
}
END_TEST

START_TEST(node_destroy_string)
{
    struct node s = make_string_node("ABC", 3);
    destroy_node(&s);
    ck_assert_ptr_eq(s.str_value, NULL);
}
END_TEST

struct match_pattern_case {
    const char *pattern;
    const char *value;
    int expected;
} match_pattern_cases[] = {
    { "", "", 1 },
    { "abc", "", 0 },
    { "", "abc", 0 },
    { "abc", "abc", 1 },
    { "abc", "de", 0 },
    { "*", "abc", 1 },
    { "a*", "abc", 1 },
    { "*c", "abc", 1 },
    { "ab*c", "abc", 1 },
    { "a*d*g", "abcdefg", 1 },
    { "*x", "abc", 0 },
    { "a*x", "abc", 0 },
    { "a*d*x", "abcdef", 0 },
    { "a**c", "axxxxxxxc", 1 },
    { "a**c", "axxxxxxxx", 0 },
    { "*a*", "xxxxaxxx", 1 },
    { "*a*", "xxxxxxxx", 0 },
};
const int N_PATTERN_CASES = sizeof(match_pattern_cases) / sizeof(struct match_pattern_case);

START_TEST(node_match_pattern)
{
    int result = match_pattern(
        match_pattern_cases[_i].pattern,
        match_pattern_cases[_i].value
    );
    ck_assert_int_eq(result, match_pattern_cases[_i].expected);
}
END_TEST
