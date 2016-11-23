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
	//printf("I've got valid json and it looks like this:\n%s\n", json);

	/* Decode the input string and check it again */
	JsonNode *head = json_decode(json);
	if (NULL == head || head->tag != JSON_OBJECT) {
            json_delete(head);
	    return ret;

	}

	/* We expect to get a list of connections. Anything else is not valid input,
	 * even though it might be valid json. */
	JsonNode *node = head->children.head; // now it should be the first dictionary value i.e. connections
	if (NULL == node || strncmp(node->key, "connections", 11) != 0) { // and also must be an array
            json_delete(head);
	    return ret;
	}

        /* Now we finally have the array of connections and this is
         * its head */
        JsonNode *connection = node->children.head;
        /* Go through all connections and put them into the connection list ret */
        while (NULL != connection) {

            struct nm_connection *new_conn = (struct nm_connection *)calloc_or_die(sizeof(struct nm_connection));
            nm_connection_init(new_conn);

            /* Read all key:value pairs in each node. Expected values
             * are: default, servers, type, zones */
            JsonNode *parameter = connection->children.head;
            while (NULL != parameter) {

                // Check JSON key
                if (JSON_BOOL == parameter->tag && strncmp(parameter->key, "default", 7) == 0) {
                    new_conn->default_con = parameter->bool_;
                } else if (JSON_STRING == parameter->tag && strncmp(parameter->key, "type", 4) == 0) {
                    if (strncmp(parameter->string_, "wifi", 4) == 0) {
                        new_conn->type = NM_CON_WIFI;
                    } else if (strncmp(parameter->string_, "vpn", 3) == 0) {
                        new_conn->type = NM_CON_VPN;
                    } else if (strncmp(parameter->string_, "other", 5) == 0) {
                        new_conn->type = NM_CON_OTHER;
                    } else {
                        new_conn->type = NM_CON_IGNORE;
                    }
                } else if (JSON_ARRAY == parameter->tag && strncmp(parameter->key, "zones", 5) == 0) {
                    JsonNode *zone = parameter->children.head;
                    while (NULL != zone) {
                        string_list_push_back(&new_conn->zones, zone->string_, strlen(zone->string_));
                        zone = zone->next;
                    }
                } else if (JSON_ARRAY == parameter->tag && strncmp(parameter->key, "servers", 7) == 0) {
                    JsonNode *server = parameter->children.head;
                    while (NULL != server) {
                        string_list_push_back(&new_conn->servers, server->string_, strlen(server->string_));
                        server = server->next;
                    }
                } else {
                    // TODO: debug output: invalid json key
                }

                parameter = parameter->next;
            }

            nm_connection_list_push_back(&ret, new_conn);
            connection = connection->next;
        }
        json_delete(head);
    } else {
	printf("Invalid json input\n");
        // TODO: log error message into syslog
    }

    return ret;
}

int main() {
    printf("Test json parser:\n");
    struct nm_connection_list l = yield_connections_from_json_test(json);
    nm_connection_list_dbg_print(&l);
}
