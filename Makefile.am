SUBDIRS = src test

clean-local:
	rm -f *.deb

# Debian packaging
deb:$(bin_PROGRAMS)
	fpm -s dir -t deb -n sawmill -v $(VERSION) --prefix $(prefix)/bin -m "<jbussdieker@gmail.com>" --url "http://github.com/jbussdieker/sawmill" --description "a log shipping tool" -C src --verbose sawmill
