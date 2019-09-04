/*
 * (C) 2018-2019 by Christian Hesse <mail@eworm.de>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
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
