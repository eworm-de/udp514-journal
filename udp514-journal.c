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

#include "udp514-journal.h"
#include <stdlib.h>
#include <stdio.h>
#include <systemd/sd-daemon.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	int sock;
	unsigned int count = 0;

	size_t num_socks = sd_listen_fds(/*unset_environment=*/true);
	if (num_socks != 1) {
		fprintf(stderr, "Invalid number of sockets received from systemd: expected 1, got %zu", num_socks);
		return EXIT_FAILURE;
	}

	sock = SD_LISTEN_FDS_START;

	/* tell systemd we are ready to go... */
	sd_notify(0, "READY=1\nSTATUS=Listening for syslog input...");

	/* server loop */
	while (1) {
		char buffer[BUFFER_SIZE];
		socklen_t len;
		char * match;
		CODE * pri;
		uint8_t priority = LOG_INFO;

		struct sockaddr_in6 cliAddr6;
		struct sockaddr *cliAddr = (struct sockaddr *)&cliAddr6;

		memset(buffer, 0, BUFFER_SIZE);
		len = sizeof(cliAddr6);
		if (recvfrom(sock, buffer, BUFFER_SIZE, 0, cliAddr, &len) < 0) {
			perror("could not receive data");
			continue;
		}

		/* parse priority */
		if ((match = strndup(buffer, BUFFER_SIZE)) != NULL) {
			char * space = strchr(match, ' ');
			if (space != NULL)
				*space = 0;
			for (pri = prioritynames; pri->c_name && strstr(match, pri->c_name) == NULL; pri++);
			free(match);
			priority = pri->c_val;
		}

		char addr_buf[64];
		const char *ret = NULL;
		sa_family_t family = cliAddr->sa_family;
		if (family == AF_INET) {
			ret = inet_ntop(family, &((struct sockaddr_in *)cliAddr)->sin_addr, addr_buf, sizeof addr_buf);
		} else if (family == AF_INET6) {
			ret = inet_ntop(family, &((struct sockaddr_in6 *)cliAddr)->sin6_addr, addr_buf, sizeof addr_buf);
		} else {
			fprintf(stderr, "Unhandled address family %u", family);
		}
		if (ret == NULL) {
			perror("inet_ntop");
			continue;
		}

		/* send to systemd-journald */
		sd_journal_send("MESSAGE=%s", buffer,
			"SYSLOG_IDENTIFIER=%s", addr_buf,
			"PRIORITY=%i", priority,
			NULL);

		/* count and update status */
		sd_notifyf(0, "READY=1\nSTATUS=Forwarded %d syslog messages.", ++count);
	}

	/* close socket */
	close(sock);

	return EXIT_SUCCESS;
}
