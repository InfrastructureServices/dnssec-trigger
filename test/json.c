#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../riggerd/fwd_zones.h"
#include "../riggerd/connection_list.h"
#include "../riggerd/string_list.h"
#include "../vendor/ccan/json/json.h"

char *json = 
"{"
"\n    \"connections\": ["
"\n        {"
"\n            \"default\": false,"
"\n            \"servers\": ["
"\n                \"10.2.0.4\","
"\n                \"10.9.2.4\","
"\n                \"10.2.0.6\""
"\n            ],"
"\n            \"type\": \"wifi\","
"\n            \"zones\": ["
"\n                \"example.com\""
"\n            ]"
"\n        },"
"\n        {"
"\n            \"default\": true,"
"\n            \"servers\": ["
"\n                \"10.60.0.16\","
"\n                \"10.67.5.56\""
"\n            ],"
"\n            \"type\": \"other\","
"\n            \"zones\": ["
"\n                \"brno.example.com\","
"\n                \"prague.example.com\","
"\n                \"finance.prague.example.com\","
"\n                \"laboratory.prague.example.com\","
"\n                \"lab2.prague.example.com\""
"\n            ]"
"\n        },"
"\n        {"
"\n            \"default\": false,"
"\n            \"servers\": ["
"\n                \"10.148.8.37\","
"\n                \"10.145.245.24\""
"\n            ],"
"\n            \"type\": \"vpn\","
"\n            \"zones\": ["
"\n                \"example.com\""
"\n            ]"
"\n        }"
"\n    ]"
"\n}\n";

struct nm_connection_list yield_connections_from_json_test(char *json)
{
    struct nm_connection_list ret;
    nm_connection_list_init(&ret);

    if (json_validate(json) == true) {
	printf("I've got valid json and it looks like this:\n%s\n", json);

	/* Decode the input string and check it again */
	JsonNode *head = json_decode(json);
	if (NULL == head || head->tag != JSON_OBJECT) {
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

int main() {
    printf("Test json parser:", json);
    yield_connections_from_json_test(json);
}
