SOURCES := mapsoft_convert.cpp

PKG_CONFIG := libxml-2.0 jansson

######################################################

MODDIR := ../core
PROGS  := $(patsubst %.cpp,%,$(SOURCES))
MDEPS  := $(shell $(MODDIR)/get_deps $(MODDIR) $(SOURCES))
ADEPS  := $(shell for i in $(MDEPS); do echo $(MODDIR)/$$i/$${i\#*/}.a; done | tac)

HDEPS := $(shell [ "$(SOURCES)" = "" ] ||\
          sed -rne 's|^\#include +"([^/]*/[^/]*)".*|$(MODDIR)/\1|p'\
          $(SOURCES) | sort -u)

all: make_deps $(PROGS)

make_deps:
	@echo "## Building dependencies: $(MDEPS)"
	@for i in $(MDEPS); do $(MAKE) -C $(MODDIR)/$$i nodep; done

override CXXFLAGS += $(shell pkg-config --cflags '$(PKG_CONFIG)')
override LDLIBS   += $(shell pkg-config --libs '$(PKG_CONFIG)')
override CPPFLAGS += -I$(MODDIR)

$(SOURCES): $(HDEPS)
%: %.cpp $(OBJECTS) $(ADEPS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $+ $(LDLIBS) -o $@

######################################################

clean:
	rm -f $(PROGS)