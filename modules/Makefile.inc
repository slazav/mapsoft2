# This file should be included from Makefiles inside
# module subdirs.
#
# Module name equals to the subdir name. Module h-file should
# have the same name (with .h extension)
#
# Before using define following variables:
# MODDIR       -- directory where all modiles are located
# MOD_HEADERS  -- list of headers (*.h, only for modules)
# MOD_SOURCES  -- list of sources (*.cpp, only for modules)
# PROGRAMS     -- programs to be build
# SIMPLE_TESTS -- Programs returning 0/1.
#                 For each <name> should be a source <name>.test.cpp.
# SCRIPT_TESTS -- Programs with testing scripts.
#                 For each <name> should be a source <name>.test.cpp and
#                 a script <name>.test.script
# OTHER_TESTS  -- Any programs or scripts which shoukld be run after building
# ALL_SOURCES  -- extra sources and headers to be include in dependency search
######################################################

MODDIR ?= ..

ifdef MOD_HEADERS
  MOD_NAME    := $(shell a=$$(pwd); echo $${a\#\#*/})
  MOD_STATIC  := $(MOD_NAME).a
  MOD_OBJECTS := $(patsubst %.cpp, %.o, $(MOD_SOURCES))
else
  MOD_NAME    :=
  MOD_STATIC  :=
  MOD_OBJECTS :=
endif

######################################################
# Dependency search for ALL_SOURCES

ALL_SOURCES += $(MOD_SOURCES) $(MOD_HEADERS)
ALL_SOURCES += $(patsubst %,%.test.cpp, $(SIMPLE_TESTS))
ALL_SOURCES += $(patsubst %,%.test.cpp, $(SCRIPT_TESTS))
ALL_SOURCES += $(patsubst %,%.cpp, $(PROGRAMS))

# Extract #include statements from MOD_SOURCES and MOD_HEADERS:
HDEPS := $(cat $ALL_SOURCES |\
          sed -rne 's|^\#include +"([^/]*/[^/]*)".*|$(MODDIR)/\1|p'\
          $(ALL_SOURCES) | sort -u)

# List of modules which should be rebuilt (in correct order).
# Here an extenal script is used (get_deps). It is also possble
# to build the list similarly to HDEPS and then visit all modules
# recursively, but then one module can be visited many times.
MDEPS := $(shell $(MODDIR)/get_deps $(MODDIR) $(ALL_SOURCES))

# a-files (should be in opposite order then MDEPS)
ADEPS := $(shell for i in $(MDEPS); do echo $(MODDIR)/$$i/$${i\#*/}.a; done | tac)

######################################################
# Building flags

override CXXFLAGS += $(shell [ "$(PKG_CONFIG)" == "" ] ||\
                       pkg-config --cflags '$(PKG_CONFIG)')
override LDLIBS   += $(shell [ "$(PKG_CONFIG)" == "" ] ||\
                       pkg-config --libs '$(PKG_CONFIG)')
override CXXFLAGS += -std=gnu++11 -Werror=return-type -O2
override CXXFLAGS += -I$(MODDIR)

######################################################
# Building rules
all: make_deps make_nodeps
make_nodeps: $(MOD_STATIC) $(PROGRAMS) make_tests
clean:
	rm -f *.o *.a *.test *.passed $(PROGRAMS)

make_deps:
	@echo "## Building dependencies: [$(MDEPS)]"
	@for i in $(MDEPS); do $(MAKE) -C $(MODDIR)/$$i make_nodeps; done
	@echo "## Finish building dependencies"

######################################################
# Building rules for tests
SIMPLE_TEST_PROGS := $(patsubst %, %.test, $(SIMPLE_TESTS))
SCRIPT_TEST_PROGS := $(patsubst %, %.test, $(SCRIPT_TESTS))
SIMPLE_TEST_RES   := $(patsubst %, %.test.passed, $(SIMPLE_TESTS))
SCRIPT_TEST_RES   := $(patsubst %, %.test.passed, $(SCRIPT_TESTS))

$(SIMPLE_TEST_RES): TEST_DEP := ''
$(SCRIPT_TEST_RES): TEST_DEP := %.test.script
$(SIMPLE_TEST_RES): TEST_CMD = ./$< && > $<.passed
$(SCRIPT_TEST_RES): TEST_CMD = ./$<.script && > $<.passed

TEST_OBJECTS += $(patsubst %,%.o, $(SIMPLE_TESTS) $(SCRIPT_TESTS))
$(SIMPLE_TEST_PROGS) $(SCRIPT_TEST_PROGS):     CC:=$(CXX)
$(SIMPLE_TEST_PROGS) $(SCRIPT_TEST_PROGS):     %: %.o $(MOD_OBJECTS) $(ADEPS)
$(TEST_OBJECTS): %.o: %.cpp $(MOD_HEADERS) $(HDEPS)

%.test.passed: %.test $(TEST_DEP)
	@echo "## Running test: $<"
	@$(TEST_CMD)

make_tests: $(SIMPLE_TEST_RES) $(SCRIPT_TEST_RES)
	@for i in $(OTHER_TESTS); do echo "## Running test: $$i"; ./$$i; done

######################################################
# building rules for programs
PROG_OBJECTS += $(patsubst %,%.o, $(PROGRAMS))
$(PROGRAMS):     CC:=$(CXX)
$(PROGRAMS):     %: %.o $(MOD_OBJECTS) $(ADEPS)
$(PROG_OBJECTS): %.o: %.cpp $(MOD_HEADERS) $(HDEPS)

######################################################
# Building rules for module static library
$(MOD_OBJECTS): $(MOD_HEADERS) $(HDEPS)

$(MOD_STATIC): $(MOD_OBJECTS)
#	rm -f $@
	ar crs $@ $+

######################################################
print_info:
	@echo "###########################"
	@echo "## Module <<$(MOD_NAME)>>"
	@echo "MOD_STATIC:   $(MOD_STATIC)"
	@echo "MOD_SOURCES:  $(MOD_SOURCES)"
	@echo "MOD_HEADERS:  $(MOD_HEADERS)"
	@echo "HDEPS:        $(HDEPS)"
	@echo "MDEPS:        $(MDEPS)"
	@echo "ADEPS:        $(ADEPS)"
	@echo "LDLIBS:       $(LDLIBS)"
	@echo "SIMPLE_TESTS: $(SIMPLE_TESTS)"
	@echo "SCRIPT_TESTS: $(SCRIPT_TESTS)"
