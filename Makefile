# DEMO MAKEFILE
# Intended for future project usage with the C++ Application Skeleton
# All commands have "@" preceding them since the Unix way is "no news is good news"
# Remove them if you want all the news
# TODO: Add PCH target for compiling all headers
# TODO: Add dist target to zip/tar code for distribution

#####################################################################
# Flags
#####################################################################
CXXFLAGS += -std=c++14 -g -O3 -Wall -Weffc++ -pedantic  \
	-pedantic-errors -Wextra -Waggregate-return -Wcast-align \
	-Wcast-qual -Wconversion \
	-Wdisabled-optimization \
	-Werror -Wfloat-equal -Wformat=2 \
	-Wformat-nonliteral -Wformat-security  \
	-Wformat-y2k \
	-Wimplicit  -Wimport  -Winit-self  -Winline \
	-Winvalid-pch   \
	-Wlong-long \
	-Wmissing-field-initializers -Wmissing-format-attribute   \
	-Wmissing-include-dirs -Wmissing-noreturn \
	-Wpacked -Wpointer-arith \
	-Wredundant-decls \
	-Wshadow -Wstack-protector \
	-Wstrict-aliasing=2 -Wswitch-default \
	-Wswitch-enum \
	-Wunreachable-code \
	-Wunused \
	-Wunused-parameter \
	-Wvariadic-macros \
	-Wwrite-strings
LDFLAGS +=
program_LIBRARIES := -lboost_filesystem -lboost_system -L $(BOOST_LIBS)/lib
program_TEST_LIBRARIES :=
program_EXPERIMENTALS_LIBRARIES :=
program_INCLUDES := -I headers
program_TEST_INCLUDES :=
program_EXPERIMENTALS_INCLUDES :=

#####################################################################
# Source Directories
#####################################################################
program_SRCDIR := src
program_HEADER_SRCDIR := headers
program_TEST_SRCDIR := tests
program_EXPERIMENTALS_SRCDIR := experimentals

#####################################################################
# Build Directory
#####################################################################
program_BUILDDIR := build
program_PRODUCTION_BUILDDIR := $(program_BUILDDIR)/production
program_TEST_BUILDDIR := $(program_BUILDDIR)/test
program_EXPERIMENTALS_BUILDDIR := $(program_BUILDDIR)/experimentals

#####################################################################
# Targets
#####################################################################
program_TARGET_DIR := target
program_PRODUCTION_TARGET := jackcompiler
program_TEST_TARGET := jackcompiler_test
program_EXPERIMENTALS_TARGET := experimentals

#####################################################################
# Sources and Source Extension
#####################################################################
program_SRCEXT := cpp
program_HEADEREXT := hpp
program_HEADER_SOURCES := $(shell find $(program_HEADER_SRCDIR) -type f -name *.$(program_HEADEREXT))
program_SOURCES := $(shell find $(program_SRCDIR) -type f -name *.$(program_SRCEXT))
program_TEST_SOURCES := $(shell find $(program_TEST_SRCDIR) -type f -name *.$(program_SRCEXT))
program_EXPERIMENTALS_SOURCES := $(shell find $(program_EXPERIMENTALS_SRCDIR) -type f -name *.$(program_SRCEXT))
program_ALL_SOURCES := $(shell find . -type f -name *.$(program_SRCEXT) -o -type f -name *.$(program_HEADEREXT))

#####################################################################
# Objects
#####################################################################
program_PRODUCTION_OBJECTS := $(patsubst $(program_SRCDIR)/%, $(program_PRODUCTION_BUILDDIR)/%, $(program_SOURCES:.$(program_SRCEXT)=.o))
program_TEST_OBJECTS := $(patsubst $(program_TEST_SRCDIR)/%, $(program_TEST_BUILDDIR)/%, $(program_TEST_SOURCES:.$(program_SRCEXT)=.o)) $(filter-out $(program_PRODUCTION_BUILDDIR)/main.o,$(program_PRODUCTION_OBJECTS))
program_EXPERIMENTALS_OBJECTS := $(patsubst $(program_EXPERIMENTALS_SRCDIR)/%, $(program_EXPERIMENTALS_BUILDDIR)/%, $(program_EXPERIMENTALS_SOURCES:.$(program_SRCEXT)=.o))

#####################################################################
# Production Target
#####################################################################
$(program_PRODUCTION_TARGET): $(program_PRODUCTION_OBJECTS)
	$(CXX) $^ -o $(program_TARGET_DIR)/$(program_PRODUCTION_TARGET) $(program_LIBRARIES)

$(program_PRODUCTION_BUILDDIR)/%.o: $(program_SRCDIR)/%.$(program_SRCEXT)
	$(CXX) $(CXXFLAGS) $(program_INCLUDES) -c -o $@ $<

#####################################################################
# Test Target
#####################################################################
$(program_TEST_TARGET): $(program_TEST_OBJECTS)
	$(CXX) $^ -o $(program_TARGET_DIR)/$(program_TEST_TARGET) $(program_TEST_LIBRARIES)

$(program_TEST_BUILDDIR)/%.o: $(program_TEST_SRCDIR)/%.$(program_SRCEXT)
	$(CXX) $(CXXFLAGS) $(program_TEST_INCLUDES) -c -o $@ $<

#####################################################################
# Experimentals Target
#####################################################################
$(program_EXPERIMENTALS_TARGET): $(program_EXPERIMENTALS_OBJECTS)
	$(CXX) $^ -o $(program_TARGET_DIR)/$(program_EXPERIMENTALS_TARGET) $(program_EXPERIMENTALS_LIBRARIES)

$(program_EXPERIMENTALS_BUILDDIR)/%.o: $(program_EXPERIMENTALS_SRCDIR)/%.$(program_SRCEXT)
	$(CXX) $(CXXFLAGS) $(program_EXPERIMENTALS_INCLUDES) -c -o $@ $<

#####################################################################
# Cleaning Things
#####################################################################
clean:
	$(RM) $(wildcard $(program_BUILDDIR)/*/*.o) $(wildcard $(program_TARGET_DIR)/*)

#####################################################################
# What do I have to do?
#####################################################################
todolist:
	for file in $(program_ALL_SOURCES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

#####################################################################
# Run the built executable, easily!
#####################################################################
run:
	./target/$(program_PRODUCTION_TARGET)

test:
	./target/$(program_TEST_TARGET)

#####################################################################
# So fake!
#####################################################################
.PHONY: clean todolist $(program_TEST_TARGET) $(program_EXPERIMENTALS_TARGET) run test
