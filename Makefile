#!/usr/bin/make -f

#-----------------------------------------------------------------------------

.PHONY: all install clean

all clean:
	$(MAKE) -C src $@

install: all
	install -D -m 755 crashceptor.script $(DESTDIR)/usr/sbin/crashceptor
	install -D -m 755 src/crashceptor $(DESTDIR)/usr/lib/crashceptor/crashceptor
	install -d -m 755 $(DESTDIR)/var/lib/crashceptor
	install -d -m 1777 $(DESTDIR)/var/log/crashceptor

#-----------------------------------------------------------------------------
# vim:ft=make
