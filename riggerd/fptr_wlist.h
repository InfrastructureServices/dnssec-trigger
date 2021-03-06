/*
 * util/fptr_wlist.h - function pointer whitelists.
 *
 * Copyright (c) 2007, NLnet Labs. All rights reserved.
 *
 * This software is open source.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * Neither the name of the NLNET LABS nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *
 * This file contains functions that check function pointers.
 * The functions contain a whitelist of known good callback values.
 * Any other values lead to an error. 
 * 
 * This prevent heap overflow based exploits, where the callback pointer
 * is overwritten by a buffer overflow (apart from this defense, buffer 
 * overflows should be fixed of course).
 *
 * Function pointers are used in
 * 	o network code callbacks.
 * 	o rbtree,  the assertions are before the critical regions.
 *		in other places, assertions are before the callback.
 */

#ifndef UTIL_FPTR_WLIST_H
#define UTIL_FPTR_WLIST_H
#include "netevent.h"

/**
 * Macro to perform an assertion check for fptr wlist checks.
 * Does not get disabled in optimize mode. Check adds security by layers.
 */
#define fptr_ok(x) \
	do { if(!(x)) \
		fatal_exit("%s:%d: %s: pointer whitelist %s failed", \
		__FILE__, __LINE__, __func__, #x); \
	} while(0);

/**
 * Check function pointer whitelist for comm_point callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_point(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for raw comm_point callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_point_raw(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for comm_timer callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_timer(void (*fptr)(void*));

/**
 * Check function pointer whitelist for comm_signal callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_signal(void (*fptr)(int, void*));

/**
 * Check function pointer whitelist for event structure callback values.
 * This is not called by libevent itself, but checked by netevent.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_event(void (*fptr)(int, short, void *));

/**
 * Check function pointer whitelist for rbtree cmp callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_rbtree_cmp(int (*fptr) (const void *, const void *));

#ifdef USE_WINSOCK
/** whitelist for registry enumeration function */
int fptr_whitelist_enum_reg(void (*fptr) (HKEY, void *));
#endif /* USE_WINSOCK */

#endif /* UTIL_FPTR_WLIST_H */
