#include "config.h"
#include "store.h"
#include "string_list.h"

struct store store_init(const char *dir, const char *full_path, const char *full_path_tmp) {
    struct string_list cache;
    string_list_init(&cache);
    struct store s = {
        .dir = dir,
        .path = full_path,
        .path_tmp = full_path_tmp,
        .cache = cache
    };
    // Read cache into the string list??
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL) {
        // TODO: log debug output
        return s;
    }
    size_t line_len = 512;
    ssize_t read;
    char *line = (char *)calloc_or_die(line_len);
    memset(line, 0, line_len);
    while ((read = getline(&line, &line_len, fp) != -1)){
        string_list_push_back(&s.cache, line, (size_t) read);
        fprintf(stderr, "\naaaaaaaaaaaaaaa: %s, %d\n", line, (int) read);
        memset(line, 0, line_len);
    }
    free(line);
    return s;
}
