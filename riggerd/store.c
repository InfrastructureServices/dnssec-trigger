#include "config.h"
#include "store.h"

struct store store_init(const char *dir, const char *full_path, const char *full_path_tmp) {
    struct store s = {
        .dir = dir,
        .path = full_path,
        .path_tmp = full_path_tmp
    };
    return s;
}
