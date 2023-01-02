/*
 * (C) 2018-2023 by Christian Hesse <mail@eworm.de>
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

#define SYSLOG_NAMES

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <systemd/sd-journal.h>
#include <systemd/sd-daemon.h>

#define STR_(x) #x
#define STR(x) STR_(x)

#define LOCAL_SERVER_PORT 514
#define LOCAL_SERVER_PORT_STR STR(LOCAL_SERVER_PORT)
#define BUFFER_SIZE 1024

#endif /* _UDP514_JOURNAL_H */
