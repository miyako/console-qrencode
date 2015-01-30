AUTOMAKE_OPTIONS = foreign subdir-objects
CFLAGS=-Wall -I/usr/local/include -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\"" 
LDFLAGS=-L/usr/local/lib -lz -lpng -lqrencode
CPPFLAGS=-Wall -I/usr/local/include -I/usr/local/include/libpng16 -DNDEBUG
bin_PROGRAMS = waqrencode
waqrencode_SOURCES = waqrencode.cc
