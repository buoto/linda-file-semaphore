#include <check.h>
#include <stdlib.h>
#include "../src/file.h"

START_TEST(file_make_linda_file_filename_copied)
{
    char name[] = "valid_path";

    struct file f = make_linda_file(name, sizeof name);
    name[1] = 'x';

    ck_assert_str_eq((char *)f.path, "valid_path");
}
END_TEST
