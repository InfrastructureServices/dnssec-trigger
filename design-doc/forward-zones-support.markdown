
# Data types

## string_list
Implementation of single linked list where every node can store `char` buffer and its length. This buffer is meant to be owned solely by this list.

## nm_connection_type
Enum type representing type of given connection (e.g. Wi-Fi or VPN)

## nm_connection
Structure describing one single connection on local host. It contains info about forward zones, dns servers, type and whether it's default or not.

## nm_connection_list
Single linker list of connections encapsulated with node.

### Owning vs non-owning lists
Owning list is supposed to be the original one with all connections. It's responsible for the memory it has. On the other hand, non-owning list is only a subset of the original one. It contains pointers to the same memory locations as the original one, thus it might not outlive its original.

**Use cases:**
 * owning - the original one - e.g. list parsed from JSON input
 * non-owning - view into the original, e.g. only default connections, or VPN

**Filtering:**
There is the `nm_connection_list_filter` function, which takes list of predicates as arguments (variable amount of them) and filter the list accordingly.


# Functions

## yield form json
Used to parse json given by the dnssec trigger script into connection list.


# Used libraries
So far only JSON parser is used.
