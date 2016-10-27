#include "config.h"

#if !defined CONNECTION_LIST_H && defined FWD_ZONES_SUPPORT
#define CONNECTION_LIST_H

#include <stdbool.h>

#include "string_list.h"

/**
 * All possible types of connections
 * in Network Manager.
 */
enum nm_connection_type {
    NM_CON_VPN,
    NM_CON_WIFI,
    NM_CON_OTHER,
    NM_CON_IGNORE,
    NM_CON_DELIMITER
};

/**
 * "connection" refers to the concept used by
 * NetworkManager. e.g. `$ nmcli con show --active`
 */
struct nm_connection {
    /** Is this connection the default one? */
    bool default_con;
    /** Linked list of zones */
    struct string_list *zones;
    /** Type of this connection as defined in enum connection_type */
    enum nm_connection_type type;
    /** Linked list of servers */
    struct string_list *servers;
};

/**
 * One node of a list of connections
 */
struct nm_connection_node {
    /** Pointer to this connection struct. */
    struct nm_connection *self;
    /** Pointer to the next connection. */
    struct nm_connection *next;
};


/**
 * Linked list of connections.
 * XXX: ?All nodes and its content is owned by this struct.
 */
struct nm_connection_list {
    /** Head of a list */
    struct nm_connection_node *first;
};

void nm_connection_list_init(struct nm_connection_list *list);

#endif /* CONNECTION_LIST_H */
