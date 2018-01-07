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

START_TEST(parser_node_1)
{
    struct node n;
    int i = 0;
    int r = parse_node(&n, &i, "\"abc\"");
    ck_assert_int_eq(r, 0);
    ck_assert_str_eq(n.str_value, "abc");
}
END_TEST

START_TEST(parser_node_2)
{
    struct node n;
    int i = 0;
    int r = parse_node(&n, &i, ">= 123");
    ck_assert_int_eq(n.int_value, 123);
    ck_assert_int_eq(n.matcher, GREATER_OR_EQUAL);
}
END_TEST

START_TEST(parser_single_tuple_1)
{
    struct parse_result pr = parse("read(123) 20");
    ck_assert_int_eq(pr.error, 0);
    ck_assert_int_eq(pr.tuple.size, 1);
    ck_assert_int_eq(pr.tuple.elems[0].int_value, 123);
    ck_assert_int_eq(pr.operation, READ);
    ck_assert_int_eq(pr.timeout_ms, 20);
}
END_TEST

START_TEST(parser_single_tuple_2)
{
    struct parse_result pr = parse("input (*) 13");
    ck_assert_int_eq(pr.error, 0);
    ck_assert_int_eq(pr.tuple.elems[0].matcher, ANY_INTEGER);
    ck_assert_int_eq(pr.operation, INPUT);
    ck_assert_int_eq(pr.timeout_ms, 13);
}
END_TEST

START_TEST(parser_multiple_tuples_1)
{
    struct parse_result pr = parse("output (>\"ddd\", <=555) 7");
    ck_assert_int_eq(pr.error, 0);
    ck_assert_int_eq(pr.operation, OUTPUT);
    ck_assert_int_eq(pr.tuple.size, 2);
    ck_assert_int_eq(pr.tuple.elems[0].matcher, GREATER);
    ck_assert_str_eq(pr.tuple.elems[0].str_value, "ddd");
    ck_assert_int_eq(pr.tuple.elems[1].matcher, LESSER_OR_EQUAL);
    ck_assert_int_eq(pr.tuple.elems[1].int_value, 555);
    ck_assert_int_eq(pr.timeout_ms, 7);
}
END_TEST

START_TEST(parser_multiple_tuples_2)
{
    struct parse_result pr = parse(" output (  >= \"*\", < \"buo\" , \"hehe\" ) 2137 ");
    ck_assert_int_eq(pr.error, 0);
    ck_assert_int_eq(pr.operation, OUTPUT);
    ck_assert_int_eq(pr.tuple.size, 3);
    ck_assert_int_eq(pr.tuple.elems[0].matcher, GREATER_OR_EQUAL);
    ck_assert_str_eq(pr.tuple.elems[0].str_value, "*");
    ck_assert_int_eq(pr.tuple.elems[1].matcher, LESSER);
    ck_assert_str_eq(pr.tuple.elems[1].str_value, "buo");
    ck_assert_int_eq(pr.tuple.elems[2].matcher, EQUAL);
    ck_assert_str_eq(pr.tuple.elems[2].str_value, "hehe");
    ck_assert_int_eq(pr.timeout_ms, 2137);
}
END_TEST

START_TEST(parser_too_many_tuples)
{
    struct parse_result pr = parse("read(1,1,1,1,1,1)");
    ck_assert_int_eq(pr.error, 15);
}
END_TEST
