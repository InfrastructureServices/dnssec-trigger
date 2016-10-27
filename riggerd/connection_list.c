#include "connection_list.h"

#ifdef FWD_ZONES_SUPPORT

void nm_connection_list_init(struct nm_connection_list *list)
{
    list->first = NULL;
}

#endif 
