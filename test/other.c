#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../riggerd/lock.h"
#include "../riggerd/store.h"

static void lock_file_call_fn(void **state) {
    lock_override("/tmp/dnssec0123456789", 21);
    lock_acquire();
    lock_release();
    (void) state; /* unused */
}

static void lock_file_check_file_presence(void **state) {
    const char *name = "/tmp/dnssec0002";
    lock_override(name, 15);
    lock_acquire();
    assert_true(access(name, F_OK) == 0);
    lock_release();
    (void) state; /* unused */
}

static void lock_file_check_file_permissions(void **state) {
    const char *name = "/tmp/dnssec0002";
    lock_override(name, 15);
    lock_acquire();
    assert_true(access(name, R_OK) == 0);
    assert_true(access(name, W_OK) == 0);
    assert_true(access(name, X_OK) == -1);
    // TODO: check that the file is indeed locked
    lock_release();
    assert_true(access(name, R_OK) == 0);
    assert_true(access(name, W_OK) == 0);
    assert_true(access(name, X_OK) == -1);
    (void) state; /* unused */
}

/* test in the function name is to avoid name conflicts */
static void store_macro_creation(void **state) {
    struct store s = STORE_INIT("test");
    assert_true(strcmp(s.dir, "/var/run/dnssec-trigger") == 0);
    assert_true(strcmp(s.path, "/var/run/dnssec-trigger/test") == 0);
    assert_true(strcmp(s.path_tmp, "/var/run/dnssec-trigger/test.tmp") == 0);
    (void) state; /* unused */
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(lock_file_call_fn),
        cmocka_unit_test(lock_file_check_file_presence),
        cmocka_unit_test(lock_file_check_file_permissions),
        cmocka_unit_test(store_macro_creation)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
