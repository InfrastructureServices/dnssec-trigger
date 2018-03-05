/**
 * Persistent storage in /var/run/dnssec-trigger/ directory.
 */

#include "config.h"

#if !defined STORE_H && defined FWD_ZONES_SUPPORT
#define STORE_H

/* Directory used for storage of all files available through
 * this module. */
#define STORE_BASE_DIR "/var/run/dnssec-trigger"
/* Concatenate file name with the base directory. */
#define STORE_PATH(NAME) (STORE_BASE_DIR "/" NAME)
/* Concatenate file name with the base directory and append ".tmp"
 * to the path. As the name suggests this file will be stored
 * temporarily and eventually it will replace the normal file. */
#define STORE_PATH_TMP(NAME) (STORE_BASE_DIR "/" NAME ".tmp")

struct store {
    const char *dir;
    const char *path;
    const char *path_tmp;
};

struct store store_init(const char *dir, const char *full_path, const char *full_path_tmp);

#define STORE_INIT(NAME) store_init((STORE_BASE_DIR),(STORE_PATH(NAME)),(STORE_PATH_TMP(NAME)))

#endif /* STORE_H */
