#include <check.h>
#include <stdlib.h>
#include "../src/store.h"

struct store_node {
    struct store_node *next;
    struct tuple element;
};

START_TEST(store_make)
{
    struct store s = make_store();

    ck_assert_ptr_eq(s.first, NULL);

    destroy_store(&s);
}
END_TEST

START_TEST(store_append_tuple)
{
    struct store s = make_store();

    struct tuple t = { 3, { NODE_I(1), NODE_I(3) } };
    store_append(&s, t);

    ck_assert_ptr_ne(s.first, NULL);
    ck_assert_int_eq(s.first->element.elems[0].int_value, 1);
    ck_assert_int_eq(s.first->element.elems[1].int_value, 3);

    ck_assert_ptr_eq(s.first->next, NULL);

    destroy_store(&s);
}
END_TEST

START_TEST(store_append_multiple)
{
    struct store s = make_store();

    store_append(&s, (struct tuple) { 1, { NODE_I(1) } });
    store_append(&s, (struct tuple) { 1, { NODE_I(2) } });
    store_append(&s, (struct tuple) { 1, { NODE_I(3) } });

    ck_assert_int_eq(s.first->element.elems[0].int_value, 1);
    ck_assert_int_eq(s.first->next->element.elems[0].int_value, 2);
    ck_assert_int_eq(s.first->next->next->element.elems[0].int_value, 3);
    ck_assert_ptr_eq(s.first->next->next->next, NULL);

    destroy_store(&s);
}
END_TEST

START_TEST(store_find)
{
    struct store s = make_store();

    struct tuple t = { 3, { NODE_I(1), NODE_S("a"), NODE_I(3) } };
    store_append(&s, t);

    struct tuple *result = find_in_store(&s, t, match_tuple);

    ck_assert_ptr_ne(result, NULL);
    ck_assert_int_eq(result->elems[0].int_value, 1);
    ck_assert_str_eq(result->elems[1].str_value, "a");
    ck_assert_int_eq(result->elems[2].int_value, 3);

    destroy_store(&s);
}
END_TEST

START_TEST(store_find_but_missing)
{
    struct store s = make_store();

    struct tuple pattern =  { 1, { NODE_I(2) } };

    struct tuple t = { 3, { NODE_I(2), NODE_S("a"), NODE_I(3213) } };
    store_append(&s, t);

    struct tuple *result = find_in_store(&s, pattern, match_tuple);

    ck_assert_ptr_eq(result, NULL);

    destroy_store(&s);
}
END_TEST

START_TEST(store_pop)
{
    struct store s = make_store();

    struct tuple pattern =  { 1, { NODE_I(2) } };
    store_append(&s, (struct tuple) { 1, { NODE_I(1) } });
    store_append(&s, (struct tuple) { 1, { NODE_I(2) } });
    store_append(&s, (struct tuple) { 1, { NODE_I(3) } });

    struct tuple *result = pop_in_store(&s, pattern, match_tuple);

    ck_assert_ptr_ne(result, NULL);
    ck_assert_int_eq(s.first->element.elems[0].int_value, 1);
    ck_assert_int_eq(s.first->next->element.elems[0].int_value, 3);
    ck_assert_ptr_eq(s.first->next->next, NULL);

    destroy_store(&s);
}
END_TEST
