#include <check.h>
#include <stdlib.h>
#include "../src/time_utils.h"

START_TEST(add_ms_one)
{
    struct timespec base = {
        .tv_sec = 0,
        .tv_nsec = 0,
    };
    struct timespec res = add_ms(&base, 1);
    ck_assert_int_eq(res.tv_sec, 0);
    ck_assert_int_eq(res.tv_nsec, 1 * NS_IN_MS);
}
END_TEST

START_TEST(add_ms_overflow)
{
    struct timespec base = {
        .tv_sec = 123,
        .tv_nsec = 2 * NS_IN_S,
    };
    struct timespec res = add_ms(&base, 1);
    ck_assert_int_eq(res.tv_sec, 125);
    ck_assert_int_eq(res.tv_nsec, 1 * NS_IN_MS);
}
END_TEST

START_TEST(add_ms_overflow_int)
{
    struct timespec base = {
        .tv_sec = 0,
        .tv_nsec = 0,
    };
    struct timespec res = add_ms(&base, 1000000000);
    ck_assert_int_eq(res.tv_sec, 1000000);
    ck_assert_int_eq(res.tv_nsec, 0);
}
END_TEST

