#include "connection_list.h"
#include "string_list.h"

#ifdef FWD_ZONES_SUPPORT

void nm_connection_init(struct nm_connection *conn)
{
    conn->default_con = false;
    string_list_init(&conn->zones);
    conn->type = NM_CON_IGNORE;
    string_list_init(&conn->servers);
}

void nm_connection_list_init(struct nm_connection_list *list)
{
    list->first = NULL;
    list->ownership = LIST_OWNING;
}

void nm_connection_list_init_non_owning(struct nm_connection_list *list)
{
    list->first = NULL;
    list->ownership = LIST_NON_OWNING;
}

void nm_connection_list_clear(struct nm_connection_list *list)
{
    //TODO: implement this!
}

void nm_connection_list_push_back(struct nm_connection_list *list, struct nm_connection *new_value)
{
    if (NULL == list || NULL == new_value) {
        return;
    }

    struct nm_connection_node **node = &list->first;
    while (NULL != *node) {
        node = &(*node)->next;
    }
    *node = (struct nm_connection_node *)calloc_or_die(sizeof(struct nm_connection_node));
    (*node)->next = NULL;
    (*node)->self = new_value;
    // new_value is now owned by connection list => it will be freed with the list
}

struct nm_connection_list nm_connection_list_filter(struct nm_connection_list *list,
        unsigned int count, ...)
{
    struct nm_connection_list ret;
    nm_connection_list_init_non_owning(&ret);

    if (NULL == list)
        return ret;

    va_list args;
    va_start(args, count);
    // Load functions into a temporary array
    filter_conn_fcn *fcn = (filter_conn_fcn *)calloc_or_die(sizeof(filter_conn_fcn *)*count);
    for(unsigned int i = 0; i < count; ++i)
    {
        fcn[i] = va_arg(args, filter_conn_fcn);
    }

    for (struct nm_connection_node *iter = list->first; NULL != iter; iter = iter->next) {
        if (count == 0) {
            // This is weird use case, but ok ...
            nm_connection_list_push_back(&ret, iter->self);
        } else {
            // TODO: implement the same for OR instead of AND ?
            // Accumulate the result of each filter
            bool acc = true;
            for(unsigned int i = 0; i < count; ++i)
            {
                acc &= fcn[i](iter->self);
            }
            if (acc) {
                nm_connection_list_push_back(&ret, iter->self);
            }
        }
    }

    va_end(args);
    free(fcn);
    return ret;
}

size_t nm_connection_list_length(struct nm_connection_list *list)
{
    if (NULL == list)
        return 0;

    size_t counter = 0;
    for (struct nm_connection_node *iter = list->first; NULL != iter; iter = iter->next) {
        counter++;
    }

    return counter;
}

void nm_connection_list_dbg_print(struct nm_connection_list *list)
{
    if (NULL == list)
        return;

    struct nm_connection_node *iter = list->first;
    int n = 0;
    while (NULL != iter) {
        printf("Connection %d\n", n);
        printf("default: ");
        if (true == iter->self->default_con) {
            printf("yes\n");
        } else {
            printf("no\n");
        }
        printf("type: ");
        switch (iter->self->type) {
            case NM_CON_VPN:
                printf("VPN\n");
                break;
            case NM_CON_WIFI:
                printf("WIFI\n");
                break;
            case NM_CON_OTHER:
                printf("OTHER\n");
                break;
            case NM_CON_DELIMITER:
                printf("DEL\n");
                break;
            case NM_CON_IGNORE:
                printf("IGNORE\n");
                break;
            default:
                printf("Unknown type\n");
        }
        printf("zones: ");
        string_list_dbg_print(&iter->self->zones);
        printf("\n");
        printf("servers: ");
        string_list_dbg_print(&iter->self->servers);
        printf("\n");

        printf("--------------------------------------\n");
        ++n;
        iter = iter->next;
    }
}

bool nm_connection_filter_type_vpn(struct nm_connection const *conn)
{
    if (conn->type == NM_CON_VPN)
        return true;
    else
        return false;
}

bool nm_connection_filter_default(struct nm_connection const *conn)
{
    if (conn->default_con == true)
        return true;
    else
        return false;
}

bool nm_connection_filter_type_other(struct nm_connection const *conn)
{
    if (conn->type == NM_CON_OTHER)
        return true;
    else
        return false;
}

#endif 
