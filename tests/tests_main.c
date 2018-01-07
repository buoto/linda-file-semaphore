#include <check.h>
#include <stdlib.h>

//  test files:
#include "node_tests.h"
#include "tuple_tests.h"
#include "parser_tests.h"
//  ----------

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc);

    //  tests:
    tcase_add_test(tc, node_make_int);
    tcase_add_test(tc, node_make_string);
    tcase_add_test(tc, node_destroy_string);
    tcase_add_loop_test(tc, node_match_pattern, 0, N_PATTERN_CASES);

    tcase_add_test(tc, tuple_make);
    tcase_add_test(tc, tuple_append_one);
    tcase_add_test(tc, tuple_max_size);

    tcase_add_test(tc, parser_init);
    //  -----

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
