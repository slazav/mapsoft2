HEADERS  := io_gpx.h io_kml.h
SOURCES  := io_gpx.cpp io_kml.cpp
SCRIPT_TESTS := io

CXXFLAGS += `pkg-config --cflags 'libxml-2.0 jansson'`
LDLIBS   += `pkg-config --libs 'libxml-2.0 jansson'`
include ../Makefile.module
