#include "string_list.h"

#ifdef FWD_ZONES_SUPPORT

#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void string_list_init(struct string_list* list)
{
	if (NULL == list)
		return;

	list->first = NULL;
}

void string_list_clear(struct string_list* list)
{
	if (NULL == list)
		return;

	struct string_entry* iter = list->first;
	while (NULL != iter) {
		struct string_entry* node = iter;
		iter = node->next;
		free(node->string);
		free(node);
	}
}

static void* calloc_or_die(size_t size) {
	void* mem = calloc(1, size);
	if (NULL == mem){
		fatal_exit("out of memory");
	} else {
		return mem;
	}
}

void string_list_push_back(struct string_list* list, const char* new_value, const size_t buffer_size)
{
	if (NULL == list || NULL == new_value || buffer_size == 0) {
			return;
	}

	size_t len = strnlen(new_value, buffer_size);
	struct string_entry** node = &list->first;

	while (NULL != *node) {
		node = &(*node)->next;
	}

	*node = (struct string_entry*) calloc_or_die(sizeof(struct string_entry));
	(*node)->length = len;
	(*node)->string = (char*) calloc_or_die(len+1);
	strncpy((*node)->string, new_value, len);
}

bool string_list_contains(const struct string_list* list, const char* value, const size_t buffer_size)
{
	if (NULL == list || NULL == value || buffer_size == 0) {
		return false;
	}

	size_t len = strnlen(value, buffer_size);

	/*
	 * Iterate through the whole list
	 */
	for (struct string_entry* iter = list->first; NULL != iter; iter = iter->next) {
		/*
		 * We already know size of both buffers, so we take advantage of that
		 * and also of short-cut evaluation.
		 */
		if (len == iter->length && strncmp(iter->string, value, len) == 0) {
			return true;
		}
	}
	return false;
}

size_t string_list_length(const struct string_list* list)
{
	if (NULL != list)
		return 0;

	size_t len = 0;
	struct string_entry* iter = list->first;
	while (NULL != iter) {
		iter = iter->next;
		++len;
	}
	return len;
}

bool string_list_is_equal(const struct string_list* l1, const struct string_list* l2)
{
	if (NULL == l1 && NULL == l2)
		return true;

	if ((NULL == l1 && NULL != l2) || (NULL == l2 && NULL != l1))
		return false;

	// Every value is unique
	if (string_list_length(l1) != string_list_length(l2)) {
		return false;
	}

	for (struct string_entry* iter = l1->first; NULL != iter; iter = iter->next) {
		if (!string_list_contains(l2, iter->string, iter->length))
			return false;
	}

	return true;
}

#endif
