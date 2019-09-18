# This file should be included from Makefiles inside
# module subdirs.
#
# Module name equals to the subdir name. Module h-file should
# have the same name (with .h extension)
#
# Before using define following variables:
# MODDIR       -- directory where all modiles are located
# MOD_HEADERS  -- list of module headers (*.h)
# MOD_SOURCES  -- list of module sources (*.cpp)
# PROGRAMS     -- programs to be build
# SHARED_LIB   -- shared library to build (collect all MOD_SOURCES + dependencies)
# SIMPLE_TESTS -- Programs returning 0/1.
#                 For each <name> should be a source <name>.test.cpp.
# SCRIPT_TESTS -- Programs with testing scripts.
#                 For each <name> should be a source <name>.test.cpp and
#                 a script <name>.test.script
# OTHER_TESTS  -- Any programs or scripts which should be run after building
# PKG_CONFIG   -- external libraries used in this module
######################################################

MODDIR ?= ..

MOD_NAME    :=
MOD_STATIC  :=
MOD_OBJECTS :=

ifdef MOD_HEADERS
  MOD_NAME    := $(shell a=$$(pwd); echo $${a\#\#*/})
  MOD_STATIC  := $(MOD_NAME).a
endif
ifdef MOD_SOURCES
  MOD_OBJECTS := $(patsubst %.cpp, %.o, $(MOD_SOURCES))
endif

SHARED_LIB_SO := $(patsubst %, %.so, $(SHARED_LIB))

######################################################
# Dependency search for ALL_SOURCES + MOD_HEADERS

ALL_SRC := $(MOD_SOURCES)
ALL_SRC += $(patsubst %,%.test.cpp, $(SIMPLE_TESTS))
ALL_SRC += $(patsubst %,%.test.cpp, $(SCRIPT_TESTS))
ALL_SRC += $(patsubst %,%.cpp, $(PROGRAMS))
ALL_SRCHDR := $(ALL_SRC) $(MOD_HEADERS)

# Build h-file dependencies (using get_deps script)
$(shell $(MODDIR)/get_deps -H $(MODDIR) $(ALL_SRC) >> Makefile.deps)

# List of modules which should be rebuilt (in correct order).
MDEPS := $(shell $(MODDIR)/get_deps -M $(MODDIR) $(ALL_SRCHDR))

# a-files (should be in opposite order then MDEPS)
ADEPS := $(shell for i in $(MDEPS); do echo $(MODDIR)/$$i/$${i\#*/}.a; done | tac)

######################################################
# Building flags

ALL_PKG_CONFIG :=  $(PKG_CONFIG) $(shell $(MODDIR)/get_deps -L $(MODDIR) $(ALL_SRCHDR))

override CXXFLAGS += $(shell [ "$(ALL_PKG_CONFIG)" == " " ] ||\
                      PKG_CONFIG_PATH=$(MODDIR)/pc pkg-config --cflags '$(ALL_PKG_CONFIG)')
override LDLIBS   += $(shell [ "$(ALL_PKG_CONFIG)" == " " ] ||\
                      PKG_CONFIG_PATH=$(MODDIR)/pc pkg-config --libs '$(ALL_PKG_CONFIG)')
override CXXFLAGS += -std=gnu++11 -Werror=return-type -O2 -fPIC
override CXXFLAGS += -I$(MODDIR)

######################################################
# Building rules
all: make_deps make_nodeps
make_nodeps: $(MOD_STATIC) $(PROGRAMS) $(SHARED_LIB_SO) make_tests
clean:
	rm -f *.o *.a *.so *.test *.passed $(PROGRAMS)

make_deps:
	@echo "## Building dependencies: [$(MDEPS)]"
	@sh -e -c 'for i in $(MDEPS); do $(MAKE) -C $(MODDIR)/$$i make_nodeps; done'
	@echo "## Finish building dependencies"

-include Makefile.deps

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

TEST_OBJECTS := $(patsubst %,%.test.o, $(SIMPLE_TESTS) $(SCRIPT_TESTS))
$(SIMPLE_TEST_PROGS) $(SCRIPT_TEST_PROGS):     CC:=$(CXX)
$(SIMPLE_TEST_PROGS) $(SCRIPT_TEST_PROGS):     %: %.o $(MOD_OBJECTS) $(ADEPS)
$(TEST_OBJECTS): %.o: %.cpp

%.test.passed: %.test $(TEST_DEP)
	@echo "## Running test: $<"
	@$(TEST_CMD)

make_tests: $(SIMPLE_TEST_RES) $(SCRIPT_TEST_RES)
	@for i in $(OTHER_TESTS); do echo "## Running test: $$i"; ./$$i; done

######################################################
# building rules for programs and shared lib
PROG_OBJECTS := $(patsubst %,%.o, $(PROGRAMS))
$(PROGRAMS):     CC:=$(CXX)
$(PROGRAMS):     %: %.o $(MOD_OBJECTS) $(ADEPS)
$(PROG_OBJECTS): %.o: %.cpp

$(SHARED_LIB_SO):   %.so: $(MOD_OBJECTS) $(ADEPS)
	$(CXX) -shared $(LDFLAGS) $+ $(LDLIBS) -o $@
######################################################
# Building rules for module static library

$(MOD_STATIC): $(MOD_OBJECTS)
	ar crs $@ $+

######################################################
print_info:
	@echo "###########################"
	@echo "## Module <<$(MOD_NAME)>>"
	@echo "MOD_STATIC:   $(MOD_STATIC)"
	@echo "MOD_SOURCES:  $(MOD_SOURCES)"
	@echo "MOD_HEADERS:  $(MOD_HEADERS)"
	@echo "MDEPS:        $(MDEPS)"
	@echo "ADEPS:        $(ADEPS)"
	@echo "LDLIBS:       $(LDLIBS)"
	@echo "SIMPLE_TESTS: $(SIMPLE_TESTS)"
	@echo "SCRIPT_TESTS: $(SCRIPT_TESTS)"
