#include <check.h>
#include <stdlib.h>
#include "../src/parser.h"

START_TEST(parser_wrong)
{
    struct parse_result pr;
    int res = parse(&pr, "trash");
    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(parser_empty)
{
    struct parse_result pr;
    int res = parse(&pr, "");
    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(parser_bracket_error)
{
    struct parse_result pr;
    int res = parse(&pr, "input  d");
    ck_assert_int_eq(res, 7);
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
    struct parse_result pr;
    int res = parse(&pr, "read(123) 20");
    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(pr.tuple.size, 1);
    ck_assert_int_eq(pr.tuple.elems[0].int_value, 123);
    ck_assert_int_eq(pr.operation, READ);
    ck_assert_int_eq(pr.timeout_ms, 20);
}
END_TEST

START_TEST(parser_single_tuple_2)
{
    struct parse_result pr;
    int res = parse(&pr, "input (*) 13");
    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(pr.tuple.elems[0].matcher, ANY);
    ck_assert_int_eq(pr.operation, INPUT);
    ck_assert_int_eq(pr.timeout_ms, 13);
}
END_TEST

START_TEST(parser_multiple_tuples_1)
{
    struct parse_result pr;
    int res = parse(&pr, "output (>\"ddd\", <=555) 7");
    ck_assert_int_eq(res, 0);
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
    struct parse_result pr;
    int res = parse(&pr, " output (  >= \"*\", < \"buo\" , \"hehe\" ) 2137 ");
    ck_assert_int_eq(res, 0);
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

START_TEST(parser_tuple_no_timeout_provided)
{
    struct parse_result pr;
    int res = parse(&pr, "input()");
    ck_assert_int_eq(res, 8);
    ck_assert_int_eq(pr.tuple.size, 0);
    ck_assert_int_eq(pr.operation, INPUT);
    ck_assert_int_eq(pr.timeout_ms, 0);
}
END_TEST

START_TEST(parser_too_many_tuples)
{
    struct parse_result pr;
    int res = parse(&pr, "read(1,1,1,1,1,1)");
    ck_assert_int_eq(res, 14);
}
END_TEST

START_TEST(parser_deserialize_tuple)
{
    struct tuple t = make_tuple();
    int res = deserialize_tuple(&t, "(\"buo*tes\")");
    ck_assert_int_eq(res, 0);
    ck_assert_str_eq(t.elems[0].str_value, "buo*tes");
}
END_TEST
