#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../riggerd/lock.h"

static void lock_file(void **state) {
    lock_override("/tmp/dnssec0123456789", 21);
    lock_acquire();
    lock_release();
    (void) state; /* unused */
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(lock_file)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
