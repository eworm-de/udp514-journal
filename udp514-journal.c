/*
 * (C) 2018-2019 by Christian Hesse <mail@eworm.de>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#include "udp514-journal.h"

int main(int argc, char **argv) {
	int sock, num;
	socklen_t len;
	struct sockaddr_in cliAddr, servAddr;
	char buffer[BUFFER_SIZE];
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
		char * match;
		CODE * pri;
		uint8_t priority = LOG_INFO;

		memset(buffer, 0, BUFFER_SIZE);
		len = sizeof(cliAddr);
		num = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			(struct sockaddr *) &cliAddr, &len);
		if (num < 0) {
			perror("could not receive data");
			continue;
		}

		/* parse priority */
		if ((match = strndup(buffer, BUFFER_SIZE)) != NULL) {
			*strchr(match, ' ') = 0;
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
