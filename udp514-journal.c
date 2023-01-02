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

#include "udp514-journal.h"

int main(int argc, char **argv) {
	int sock;
	struct sockaddr_in cliAddr, servAddr;
	const int opt_val = 1;
	unsigned int count = 0;

	/* open socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("could not open socket");
		return EXIT_FAILURE;
	}

	/* bind local socket port */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
	if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("could not bind on port " LOCAL_SERVER_PORT_STR);
		return EXIT_FAILURE;
	}

	/* tell systemd we are ready to go... */
	sd_notify(0, "READY=1\nSTATUS=Listening for syslog input...");

	/* server loop */
	while (1) {
		char buffer[BUFFER_SIZE];
		socklen_t len;
		char * match;
		CODE * pri;
		uint8_t priority = LOG_INFO;

		memset(buffer, 0, BUFFER_SIZE);
		len = sizeof(cliAddr);
		if (recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliAddr, &len) < 0) {
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

		/* send to systemd-journald */
		sd_journal_send("MESSAGE=%s", buffer,
			"SYSLOG_IDENTIFIER=%s", inet_ntoa(cliAddr.sin_addr),
			"PRIORITY=%i", priority,
			NULL);

		/* count and update status */
		sd_notifyf(0, "READY=1\nSTATUS=Forwarded %d syslog messages.", ++count);
	}

	/* close socket */
	close(sock);

	return EXIT_SUCCESS;
}
