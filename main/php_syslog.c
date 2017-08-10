/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2017 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Philip Prindeville <philipp@redfish-solutions.com>           |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "php.h"
#include "php_syslog.h"

#include "zend.h"
#include "zend_smart_string.h"

/*
 * The SCO OpenServer 5 Development System (not the UDK)
 * defines syslog to std_syslog.
 */

#ifdef HAVE_STD_SYSLOG
#define syslog std_syslog
#endif

PHPAPI void php_syslog(int priority, const char *format, ...) /* {{{ */
{
	const char *ptr;
	unsigned char c;
	smart_string fbuf = {0};
	smart_string sbuf = {0};
	va_list args;

	va_start(args, format);
	zend_printf_to_smart_string(&fbuf, format, args);
	smart_string_0(&fbuf);
	va_end(args);

	for (ptr = fbuf.c; ; ++ptr) {
		c = *ptr;
		if (c == '\0') {
			syslog(priority, "%.*s", (int)sbuf.len, sbuf.c);
			break;
		}

		if (c != '\n')
			smart_string_appendc(&sbuf, c);
		else {
			syslog(priority, "%.*s", (int)sbuf.len, sbuf.c);
			smart_string_reset(&sbuf);
		}
	}

	smart_string_free(&fbuf);
	smart_string_free(&sbuf);
}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
