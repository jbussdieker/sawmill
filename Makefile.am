TESTS = test/memory_leak.test
SUBDIRS = src

clean-local:
	rm -f *.deb

# Debian packaging
deb:$(bin_PROGRAMS)
	fpm -s dir -t deb -n sawmill -v $(VERSION) --prefix $(prefix) --description "a log shipping tool" $(bin_PROGRAMS)
