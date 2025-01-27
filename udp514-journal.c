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

int main(int argc, char **argv) {
	int activation, sock;
	unsigned int count = 0;

	/* socket address for listening */
	struct sockaddr_storage ss_listen = {};
	struct sockaddr     *addr_listen     = (struct sockaddr *)     &ss_listen;
	struct sockaddr_in  *addr_listen_in  = (struct sockaddr_in *)  &ss_listen;
	struct sockaddr_in6 *addr_listen_in6 = (struct sockaddr_in6 *) &ss_listen;

	activation = sd_listen_fds(0);

	if (activation > 1) {
		perror("too many file descriptors received");
		return EXIT_FAILURE;
	} else if (activation == 1) {
		/* use the socket passed from systemd */
		sock = SD_LISTEN_FDS_START + 0;
	} else if ((sock = socket(AF_INET6, SOCK_DGRAM, 0)) > 0) {
		/* bind local socket port - IPv6 */
		addr_listen_in6->sin6_family = AF_INET6;
		addr_listen_in6->sin6_addr = in6addr_any;
		addr_listen_in6->sin6_scope_id = 0;
		addr_listen_in6->sin6_port = htons(LOCAL_SERVER_PORT);
		if (bind(sock, addr_listen, sizeof(struct sockaddr_in6)) < 0) {
			perror("could not bind on port " LOCAL_SERVER_PORT_STR);
			close(sock);
			return EXIT_FAILURE;
		}
	} else if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) > 0) {
		/* bind local socket port - fallback IPv4 */
		addr_listen_in->sin_family = AF_INET;
		addr_listen_in->sin_addr.s_addr = htonl(INADDR_ANY);
		addr_listen_in->sin_port = htons(LOCAL_SERVER_PORT);
		if (bind(sock, addr_listen, sizeof(struct sockaddr_in)) < 0) {
			perror("could not bind on port " LOCAL_SERVER_PORT_STR);
			close(sock);
			return EXIT_FAILURE;
		}
	} else {
		perror("could not open socket");
		return EXIT_FAILURE;
	}

	/* tell systemd we are ready to go... */
	sd_notify(0, "READY=1\nSTATUS=Listening for syslog input...");

	/* server loop */
	while (1) {
		char addr_buf[INET6_ADDRSTRLEN], msg_buf[BUFFER_SIZE];
		const char * address;
		socklen_t len;
		char * match;
		CODE * pri;
		uint8_t priority = LOG_INFO;

		/* socket address for client */
		struct sockaddr_storage ss_client = {};
		struct sockaddr     *addr_client     = (struct sockaddr *)     &ss_client;
		struct sockaddr_in  *addr_client_in  = (struct sockaddr_in *)  &ss_client;
		struct sockaddr_in6 *addr_client_in6 = (struct sockaddr_in6 *) &ss_client;

		memset(msg_buf, 0, BUFFER_SIZE);
		len = sizeof(struct sockaddr_storage);
		if (recvfrom(sock, msg_buf, BUFFER_SIZE, 0, addr_client, &len) < 0) {
			perror("could not receive data");
			continue;
		}

		/* parse priority */
		if ((match = strndup(msg_buf, BUFFER_SIZE)) != NULL) {
			char * space = strchr(match, ' ');
			if (space != NULL)
				*space = 0;
			for (pri = prioritynames; pri->c_name && strstr(match, pri->c_name) == NULL; pri++);
			free(match);
			priority = pri->c_val;
		}

		/* get client's ip address */
		switch (addr_client->sa_family) {
			case AF_INET6:
				const struct in6_addr *in6_addr = &addr_client_in6->sin6_addr;
				address = inet_ntop(AF_INET6, in6_addr, addr_buf, INET6_ADDRSTRLEN);
				/* strip prefix (::ffff:) from mapped ipv4 addresses */
				if (address && IN6_IS_ADDR_V4MAPPED(in6_addr)) {
					address += 7;
				}
				break;
			case AF_INET:
				address = inet_ntop(AF_INET, &addr_client_in->sin_addr,
					addr_buf, INET6_ADDRSTRLEN);
				break;
			default:
				fputs("unhadled address family", stderr);
				continue;
		}
		if (address == NULL) {
			perror("could not get clients ip address");
			continue;
		}

		/* send to systemd-journald */
		sd_journal_send("MESSAGE=%s", msg_buf,
			"SYSLOG_IDENTIFIER=%s", address,
			"PRIORITY=%i", priority,
			NULL);

		/* count and update status */
		sd_notifyf(0, "READY=1\nSTATUS=Forwarded %d syslog messages.", ++count);
	}

	/* close socket */
	close(sock);

	return EXIT_SUCCESS;
}
