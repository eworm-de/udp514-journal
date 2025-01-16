# udp514-journal - forward syslog from network (udp/514) to journal

# commands
CC	:= gcc
MD	:= markdown
INSTALL	:= install
RM	:= rm
CP	:= cp

# flags
CFLAGS	+= -std=c11 -O2 -fPIC -Wall -Werror
CFLAGS  += $(shell pkg-config --cflags --libs libsystemd 2>/dev/null)
LDFLAGS	+= -Wl,-z,now -Wl,-z,relro -pie

# this is just a fallback in case you do not use git but downloaded
# a release tarball...
VERSION := 0.2.0

all: udp514-journal README.html

version.h: $(wildcard .git/HEAD .git/index .git/refs/tags/*) Makefile
	printf "#ifndef VERSION\n#define VERSION \"%s\"\n#endif\n" $(shell git describe --long 2>/dev/null || echo ${VERSION}) > $@

udp514-journal: udp514-journal.c udp514-journal.h version.h
	$(CC) udp514-journal.c $(CFLAGS) $(LDFLAGS) -o udp514-journal

README.html: README.md
	$(MD) README.md > README.html

install: install-bin install-doc

install-bin: udp514-journal
	$(INSTALL) -D -m0755 udp514-journal $(DESTDIR)/usr/bin/udp514-journal
	$(INSTALL) -D -m0644 udp514-journal.service $(DESTDIR)/usr/lib/systemd/system/udp514-journal.service
	$(INSTALL) -D -m0644 udp514-journal.socket $(DESTDIR)/usr/lib/systemd/system/udp514-journal.socket

install-doc: README.html
	$(INSTALL) -D -m0644 README.md $(DESTDIR)/usr/share/doc/udp514-journal/README.md
	$(INSTALL) -D -m0644 README.html $(DESTDIR)/usr/share/doc/udp514-journal/README.html

clean:
	$(RM) -f *.o *~ udp514-journal README.html version.h

distclean:
	$(RM) -f *.o *~ udp514-journal README.html version.h config.h

release:
	git archive --format=tar.xz --prefix=udp514-journal-$(VERSION)/ $(VERSION) > udp514-journal-$(VERSION).tar.xz
	gpg --armor --detach-sign --comment udp514-journal-$(VERSION).tar.xz udp514-journal-$(VERSION).tar.xz
	git notes --ref=refs/notes/signatures/tar add -C $$(git archive --format=tar --prefix=udp514-journal-$(VERSION)/ $(VERSION) | gpg --armor --detach-sign --comment udp514-journal-$(VERSION).tar | git hash-object -w --stdin) $(VERSION)
