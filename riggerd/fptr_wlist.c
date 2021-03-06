/*
 * util/fptr_wlist.c - function pointer whitelists.
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
 * Due to the listing nature, this file violates all the modularization
 * boundaries in the program.
 */
#include "config.h"
#include "fptr_wlist.h"
#include "svr.h"
#include "probe.h"
#include "mini_event.h"
#include "http.h"
#include "update.h"
#ifdef USE_WINSOCK
#include "winrc/netlist.h"
#include "winrc/win_svc.h"
#endif

int 
fptr_whitelist_comm_point(comm_point_callback_t *fptr)
{
	if(fptr == &outq_handle_udp) return 1;
	else if(fptr == &outq_handle_tcp) return 1;
	return 0;
}

int 
fptr_whitelist_comm_point_raw(comm_point_callback_t *fptr)
{
	if(fptr == &handle_ssl_accept) return 1;
	else if(fptr == &http_get_callback) return 1;
	else if(fptr == &control_callback) return 1;
	return 0;
}

int 
fptr_whitelist_comm_timer(void (*fptr)(void*))
{
	if(fptr == &outq_timeout) return 1;
	else if(fptr == &svr_retry_callback) return 1;
	else if(fptr == &http_get_timeout_handler) return 1;
	else if(fptr == &selfupdate_timeout) return 1;
	else if(fptr == &svr_tcp_callback) return 1;
#ifdef USE_WINSOCK
	else if(fptr == &wsvc_cron_cb) return 1;
#endif
	return 0;
}

int 
fptr_whitelist_comm_signal(void (*fptr)(int, void*))
{
	(void)fptr;
	return 0;
}

int 
fptr_whitelist_event(void (*fptr)(int, short, void *))
{
	if(fptr == &comm_point_udp_callback) return 1;
	else if(fptr == &comm_point_udp_ancil_callback) return 1;
	else if(fptr == &comm_point_tcp_accept_callback) return 1;
	else if(fptr == &comm_point_tcp_handle_callback) return 1;
	else if(fptr == &comm_timer_callback) return 1;
	else if(fptr == &comm_signal_callback) return 1;
	else if(fptr == &comm_point_local_handle_callback) return 1;
	else if(fptr == &comm_point_raw_handle_callback) return 1;
#ifdef USE_WINSOCK
	else if(fptr == &netlist_change_cb) return 1;
	else if(fptr == &worker_win_stop_cb) return 1;
#endif
	return 0;
}

int 
fptr_whitelist_rbtree_cmp(int (*fptr) (const void *, const void *))
{
	if(fptr == &mini_ev_cmp) return 1;
	return 0;
}

#ifdef USE_WINSOCK
int fptr_whitelist_enum_reg(void (*fptr) (HKEY, void *))
{
	if(fptr == &enum_reg_set_nameserver) return 1;
	return 0;
}
#endif /* USE_WINSOCK */

