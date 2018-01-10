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
`systemctl start udp514-journal`.

Make sure UDP port 514 is not blocked in your firewall.

### Upstream

URL: [GitHub.com](https://github.com/eworm-de/udp514-journal)  
Mirror: [eworm.de](https://git.eworm.de/cgit.cgi/udp514-journal/)
