#include <stdbool.h>

#include "fwd_zones.h"
#include "../vendor/ccan/json/json.h"

#ifdef FWD_ZONES_SUPPORT

struct nm_connection_list yield_connections_from_json(char *json)
{
    struct nm_connection_list ret;
    nm_connection_list_init(&ret);

    if (json_validate(json) == true) {
        printf("I've got valid json and it looks like this:\n%s\n", json);
    } else {
        printf("Invalid json input");
    }

    return ret;
}

#endif
