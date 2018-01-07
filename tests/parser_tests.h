#include <check.h>
#include <stdlib.h>
#include "../src/parser.h"

START_TEST(parser_wrong)
{
    struct parse_result pr = parse("abc");
    ck_assert_int_eq(pr.error, 1);
}
END_TEST

START_TEST(parser_empty)
{
    struct parse_result pr = parse("");
    ck_assert_int_eq(pr.error, 1);
}
END_TEST

START_TEST(parser_bracket_error)
{
    struct parse_result pr = parse("input  d");
    ck_assert_int_eq(pr.error, 8);
}
END_TEST

START_TEST(parser_single_tuple)
{
    struct parse_result pr = parse("read(123)");
    ck_assert_int_eq(pr.error, 0);
}
END_TEST
