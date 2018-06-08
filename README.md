udp514-journal
==============

forward syslog from network (udp/514) to systemd-journald

Requirements
------------

To compile and run `udp514-journal` you need:

* [systemd](https://www.github.com/systemd/systemd)
* [markdown](http://daringfireball.net/projects/markdown/) (HTML documentation)

Build and install
-----------------

Building and installing is very easy. Just run:

> make

followed by:

> make install

This will place an executable at `/usr/bin/udp514-journal`,
documentation can be found in `/usr/share/doc/udp514-journal/`.
Additionally a systemd unit file is installed to
`/usr/lib/systemd/system/udp514-journal.service`.

Usage
-----

Just run `udp514-journal` or start a systemd unit with
`systemctl start udp514-journal`. Make sure UDP port 514 is not blocked
in your firewall.

Use `journalctl` to view the logs:

    $ journalctl -u udp514-journal
    Jun 07 08:15:22 server 10.0.0.1[548]: dhcp,info mikrotik1: intern assigned 10.0.0.50 to 00:11:22:33:44:55
    Jun 07 09:16:59 server 10.0.0.1[548]: interface,info mikrotik1: en7 link down
    Jun 07 09:17:17 server 10.0.0.1[548]: interface,info mikrotik1: en7 link up (speed 100M, full duplex)
    Jun 07 10:07:16 server 10.1.1.1[548]: wireless,info mikrotik2: 00:11:22:33:44:66@wl2-guest: connected, signal strength -36
    Jun 07 10:07:21 server 10.1.1.1[548]: dhcp,info mikrotik2: guest assigned 192.168.1.50 to 00:11:22:33:44:66

Filtering is available with matching `SYSLOG_IDENTIFIER` the ip address:

    $ journalctl -u udp514-journal SYSLOG_IDENTIFIER=10.1.1.1
    Jun 07 10:07:16 server 10.1.1.1[548]: wireless,info mikrotik2: 00:11:22:33:44:66@wl2-guest: connected, signal strength -36
    Jun 07 10:07:21 server 10.1.1.1[548]: dhcp,info mikrotik2: guest assigned 192.168.1.50 to 00:11:22:33:44:66

### Upstream

URL:
[GitHub.com](https://github.com/eworm-de/udp514-journal#udp514-journal)

Mirror:
[eworm.de](https://git.eworm.de/cgit.cgi/udp514-journal/)
[GitLab.com](https://gitlab.com/eworm-de/udp514-journal#udp514-journal)
