/*
 * (C) 2018-2025 by Christian Hesse <mail@eworm.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef _UDP514_JOURNAL_H
#define _UDP514_JOURNAL_H

#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SYSLOG_NAMES
#define __USE_MISC
#include <sys/syslog.h>

#include <systemd/sd-journal.h>
#include <systemd/sd-daemon.h>

#define STR_(x) #x
#define STR(x) STR_(x)

#define LOCAL_SERVER_PORT 514
#define LOCAL_SERVER_PORT_STR STR(LOCAL_SERVER_PORT)
#define BUFFER_SIZE 1024

/* RFC 3164 date: "Oct 11 22:14:15"
   https://www.rfc-editor.org/rfc/rfc3164 */
#define DATE_RFC3164 "[A-Za-z]\\{3\\} [ 123][0-9] [ 012][0-9]:[0-5][0-9]:[0-6][0-9]"
/* RFC 5424 date: "2003-10-11T22:14:15.003Z" (or similar)
   https://www.rfc-editor.org/rfc/rfc5424 */
#define DATE_RFC5424 "[2-9][0-9]\\{3\\}-[012][0-9]-[0123][0-9]T[012][0-9]:[0-5][0-9]:[0-6][0-9]\\(\\.[0-9]\\{1,6\\}\\)\\?\\(Z\\|[+-][012][0-9]:[0-5][0-9]\\)"

#endif /* _UDP514_JOURNAL_H */
