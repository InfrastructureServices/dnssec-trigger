#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../riggerd/lock.h"
#include "../riggerd/store.h"
#include "../riggerd/string_buffer.h"

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

static void store_macro_creation(void **state) {
    struct store s = STORE_INIT("test");
    assert_true(strcmp(s.dir, "/var/run/dnssec-trigger") == 0);
    assert_true(strcmp(s.path, "/var/run/dnssec-trigger/test") == 0);
    assert_true(strcmp(s.path_tmp, "/var/run/dnssec-trigger/test.tmp") == 0);
    (void) state; /* unused */
}

static void store_read_file_content(void **state) {
    const char *file_name = "test/servers-list-ipv4";
    assert_true(access(file_name, R_OK) == 0);

    struct store s = store_init("", "test/servers-list-ipv4", "");

    assert_true(string_list_contains(&s.cache, "1.2.3.4", 8));
    assert_true(string_list_contains(&s.cache, "192.168.168.168", 15));
    assert_true(string_list_length(&s.cache) == 2);

    store_destroy(&s);
    
    (void) state; /* unused */
}

static void store_commit_cache(void **state) {
    const char *dir_name = "test/tmp";
    const char *file_name = "test/tmp/commit-cache";
    const char *tmp_file_name = "test/tmp/commit-cache.tmp";
    struct string_buffer sb = string_builder("5.6.7.8");
    struct string_buffer sb2 = string_builder("9.10.11.12");

    // write to file
    {
        struct store s = store_init(dir_name, file_name, tmp_file_name);
        string_list_clear(&s.cache);
        string_list_push_back(&s.cache, sb.string, sb.length);
        string_list_push_back(&s.cache, sb2.string, sb2.length);
        store_commit(&s);
        store_destroy(&s);
    }

    // read from file
    {
        struct store s = store_init(dir_name, file_name, tmp_file_name);
        assert_true(string_list_contains(&s.cache, sb.string, sb.length));
        assert_true(string_list_contains(&s.cache, sb2.string, sb2.length));
        assert_true(string_list_length(&s.cache) == 2);
        store_destroy(&s);
    }
    
    (void) state; /* unused */
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(lock_file_call_fn),
        cmocka_unit_test(lock_file_check_file_presence),
        cmocka_unit_test(lock_file_check_file_permissions),
        cmocka_unit_test(store_macro_creation),
        cmocka_unit_test(store_read_file_content),
        cmocka_unit_test(store_commit_cache)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
