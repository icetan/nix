libraries += libfetchers

libfetchers_NAME = libnixfetchers

libfetchers_DIR := $(d)

libfetchers_SOURCES := $(wildcard $(d)/*.cc)

libfetchers_CXXFLAGS += -I src/libutil -I src/libstore

libfetchers_LDFLAGS += -pthread -lzip -lgit2 -larchive

libfetchers_LIBS = libutil libstore
