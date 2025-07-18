udp514-journal
==============

[![GitHub stars](https://img.shields.io/github/stars/eworm-de/udp514-journal?logo=GitHub&style=flat&color=red)](https://github.com/eworm-de/udp514-journal/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/eworm-de/udp514-journal?logo=GitHub&style=flat&color=green)](https://github.com/eworm-de/udp514-journal/network)
[![GitHub watchers](https://img.shields.io/github/watchers/eworm-de/udp514-journal?logo=GitHub&style=flat&color=blue)](https://github.com/eworm-de/udp514-journal/watchers)

forward syslog from network (udp/514) to systemd-journald

*Use at your own risk*, pay attention to
[license and warranty](#license-and-warranty), and
[disclaimer on external links](#disclaimer-on-external-links)!

Requirements
------------

To compile and run `udp514-journal` you need:

* [systemd ↗️](https://www.github.com/systemd/systemd)
* [markdown ↗️](https://daringfireball.net/projects/markdown/) (HTML documentation)

Build and install
-----------------

Building and installing is very easy. Just run:

    make

followed by:

    make install

This will place an executable at `/usr/bin/udp514-journal`,
documentation can be found in `/usr/share/doc/udp514-journal/`.
Additionally systemd unit files `udp514-journal.service` and
`udp514-journal.socket` are installed to
`/usr/lib/systemd/system/`.

Usage
-----

Just run `udp514-journal` or start a systemd unit with:

    systemctl start udp514-journal.socket

Make sure UDP port 514 is not blocked in your firewall.

To enable it permanently to make it start on system boot run:

    systemctl enable --now udp514-journal.socket

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

License and warranty
--------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
[GNU General Public License](COPYING.md) for more details.

Disclaimer on external links
----------------------------

Our website contains links to the websites of third parties ("external
links"). As the content of these websites is not under our control, we
cannot assume any liability for such external content. In all cases, the
provider of information of the linked websites is liable for the content
and accuracy of the information provided. At the point in time when the
links were placed, no infringements of the law were recognisable to us.
As soon as an infringement of the law becomes known to us, we will
immediately remove the link in question.

> 💡️ **Hint**: All external links are marked with an arrow pointing
> diagonally in an up-right (or north-east) direction (↗️).

### Upstream

URL:
[GitHub.com](https://github.com/eworm-de/udp514-journal#udp514-journal)

Mirror:
[eworm.de](https://git.eworm.de/cgit.cgi/udp514-journal/)
[GitLab.com](https://gitlab.com/eworm-de/udp514-journal#udp514-journal)

---
[⬆️ Go back to top](#top)
