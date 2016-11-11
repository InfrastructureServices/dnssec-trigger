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

	/* Decode the input string and check it again */
	JsonNode *head = json_decode(json);
	//shouldn't be null because we've checked its validity
	if (head->tag != JSON_OBJECT) {
	    return ret;

	}

	/* We expect to get a list of connections. Anything else is not valid input,
	 * even though it might be valid json. */
	JsonNode *node = head->children.head; // now it should be the first dictionary value i.e. connections
	if (NULL == node || strncmp(node->key, "connections", 11) != 0) { // and also must be an array
	    return ret;
	}
	// node is array called connections

    } else {
	printf("Invalid json input\n");
        // TODO: log error message into syslog
    }

    return ret;
}

#endif
