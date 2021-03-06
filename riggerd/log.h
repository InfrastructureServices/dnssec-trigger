/*
 * util/log.h - logging service
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
 * This file contains logging functions.
 */

#ifndef UTIL_LOG_H
#define UTIL_LOG_H

/**
 * verbosity value:
 */
enum verbosity_value {
 /** 0 - no verbose messages */
	NO_VERBOSE = 0,
 /** 1 - operational information */
 	VERB_OPS,
 /** 2 - detailed information */
 	VERB_DETAIL,
 /** 3 - query level information */
 	VERB_QUERY,
 /** 4 - algorithm level information */
 	VERB_ALGO,
 /** 5 - querier client information */
	VERB_CLIENT
};

/** The global verbosity setting */
extern enum verbosity_value verbosity;

/**
 * log a verbose message, pass the level for this message.
 * It has printf formatted arguments. No trailing newline is needed.
 * @param level: verbosity level for this message, compared to global 
 *	verbosity setting.
 * @param format: printf-style format string. Arguments follow.
 */
void verbose(enum verbosity_value level, 
	const char* format, ...) ATTR_FORMAT(printf, 2, 3);

/**
 * call this to initialize logging services.
 * @param filename: if NULL stderr is used.
 * @param use_syslog: set to true to ignore filename and use syslog(3).
 * @param chrootdir: to which directory we have been chrooted, if any.
 */
void log_init(const char* filename, int use_syslog, const char* chrootdir);

/**
 * Set logging to go to the specified file *.
 * This setting does not affect the use_syslog setting.
 * @param f: to that file, or pass NULL to disable logging.
 */
void log_file(FILE *f);

/**
 * Set identity to print, default is 'unbound'. 
 * @param id: string to print. Name of executable.
 */
void log_ident_set(const char* id);

/**
 * Set the time value to print in log entries.
 * @param t: the point is copied and used to find the time.
 * 	if NULL, time(2) is used.
 */
void log_set_time(uint32_t* t);

/**
 * Set if the time value is printed ascii or decimal in log entries.
 * @param use_asc: if true, ascii is printed, otherwise decimal.
 *	If the conversion fails or you have no time functions, 
 *	decimal is printed.
 */
void log_set_time_asc(int use_asc);

/**
 * Log informational message.
 * Pass printf formatted arguments. No trailing newline is needed.
 * @param format: printf-style format string. Arguments follow.
 */
void log_info(const char* format, ...) ATTR_FORMAT(printf, 1, 2);

/**
 * Log error message.
 * Pass printf formatted arguments. No trailing newline is needed.
 * @param format: printf-style format string. Arguments follow.
 */
void log_err(const char* format, ...) ATTR_FORMAT(printf, 1, 2);

/**
 * Log warning message.
 * Pass printf formatted arguments. No trailing newline is needed.
 * @param format: printf-style format string. Arguments follow.
 */
void log_warn(const char* format, ...) ATTR_FORMAT(printf, 1, 2);

/**
 * Log a hex-string to the log. Can be any length.
 * performs mallocs to do so, slow. But debug useful.
 * @param msg: string desc to accompany the hexdump.
 * @param data: data to dump in hex format.
 * @param length: length of data.
 */
void log_hex(const char* msg, void* data, size_t length);

/**
 * Log fatal error message, and exit the current process.
 * Pass printf formatted arguments. No trailing newline is needed.
 * @param format: printf-style format string. Arguments follow.
 */
void fatal_exit(const char* format, ...) ATTR_FORMAT(printf, 1, 2);

/**
 * va_list argument version of log_info.
 * @param pri: priority type, for example 5 (INFO).
 * @param type: string to designate type of message (info, error).
 * @param format: the printf style format to print. no newline.
 * @param args: arguments for format string.
 */
void log_vmsg(int pri, const char* type, const char* format, va_list args);

/**
 * an assertion that is thrown to the logfile.
 */
#ifdef UNBOUND_DEBUG
#  define log_assert(x) \
	do { if(!(x)) \
		fatal_exit("%s:%d: %s: assertion %s failed", \
			__FILE__, __LINE__, __func__, #x); \
	} while(0);
#else
#  define log_assert(x) /*nothing*/
#endif

#ifdef USE_WINSOCK
/**
 * Convert WSA error into string.
 * @param err: from WSAGetLastError()
 * @return: string.
 */
char* wsa_strerror(DWORD err);
#endif /* USE_WINSOCK */

#endif /* UTIL_LOG_H */
