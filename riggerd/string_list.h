#include "config.h"

#if !defined STRING_LIST_H && defined FWD_ZONES_SUPPORT
#define STRING_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Linked list of strings
 */
struct string_list {
	/** A linked list of strings */
	struct string_entry *first;
};

/**
 * One node in the string list
 */
struct string_entry {
	/** Next in list */
	struct string_entry* next;
	/** String owned by this list
	 * Do not use this pointer elsewhere
	 */
	char* string;
	/** Length of the string buffer */
	size_t length;
};

// TODO: move somewhere else
/*
 * Thin wrapper around malloc. It either gets a valid memory
 * or exit the whole process.
 */
void* calloc_or_die(size_t size);

/**
 * Initialize a new list of strings
 * @param list: New list
 */
void string_list_init(struct string_list* list);

/**
 * Clear the list and free all contained buffers
 * @param list: List to be cleared. To free the structure itself is caller responsibility.
 */
void string_list_clear(struct string_list* list);

/**
 * Push new string at the end of the list. The string
 * is copied into the node.
 * @param list: List to append to
 * @param new_value: String to be appended
 * @param buffer_size: Size of the buffer from which the string is copied
 */
void string_list_push_back(struct string_list* list, const char* new_value, const size_t buffer_size);

/**
 * Find out whether the list contains the given value
 * @param list: List to check
 * @param new_value: String to be found
 * @param buffer_size: Size of the string buffer
 */
bool string_list_contains(const struct string_list* list, const char* value, const size_t buffer_size);

/**
 * Find out the size of given list
 * @param list: List to check
 */
size_t string_list_length(const struct string_list* list);

/**
 * Compare content of two lists. Every value must be unique.
 * @param l1: First list
 * @param l2: Second list
 */
bool string_list_is_equal(const struct string_list* l1, const struct string_list* l2);

/**
 * Print list of strings onto one line without newline at the end.
 * @param list: List to be printed
 */
void string_list_dbg_print(const struct string_list* list);

#endif /* STRING_LIST_H */

