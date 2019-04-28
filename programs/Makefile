SOURCES    := mapsoft_convert.cpp
TESTS      := mapsoft_convert.test1

PKG_CONFIG := libxml-2.0 jansson

######################################################

MODDIR := ../core
PROGS  := $(patsubst %.cpp,%,$(SOURCES))
MDEPS  := $(shell $(MODDIR)/get_deps $(MODDIR) $(SOURCES))
ADEPS  := $(shell for i in $(MDEPS); do echo $(MODDIR)/$$i/$${i\#*/}.a; done | tac)

HDEPS := $(shell [ "$(SOURCES)" = "" ] ||\
          sed -rne 's|^\#include +"([^/]*/[^/]*)".*|$(MODDIR)/\1|p'\
          $(SOURCES) | sort -u)

all: make_deps $(PROGS) tests

make_deps:
	@echo "## Building dependencies: $(MDEPS)"
	@for i in $(MDEPS); do $(MAKE) -C $(MODDIR)/$$i nodep; done

override CXXFLAGS += $(shell [ "$(PKG_CONFIG)" == "" ] || pkg-config --cflags '$(PKG_CONFIG)')
override LDLIBS   += $(shell [ "$(PKG_CONFIG)" == "" ] || pkg-config --libs '$(PKG_CONFIG)')
override CPPFLAGS += -I$(MODDIR)
override CPPFLAGS += -std=gnu++11 -Werror=return-type -O2

$(SOURCES): $(HDEPS)
%: %.cpp $(OBJECTS) $(ADEPS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $+ $(LDLIBS) -o $@

tests:
	for i in $(TESTS); do ./$$i; done

######################################################

clean:
	rm -f $(PROGS)