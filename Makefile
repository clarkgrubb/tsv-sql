MAKEFLAGS += --warn-undefined-variables
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
.SUFFIXES:

CC := gcc
CFLAGS := -g
BISON := bison
BISONFLAGS :=
FLEX := flex
FLEXFLAGS :=

headers := engine.h def.h relation.h table.h util.h
generated_headers := grammar.h
all_headers := $(headers) $(generated_headers)
sources := engine.c relation.c table.c util.c
generated_sources := tokens.c grammar.c
all_lib_sources := $(sources) $(generated_sources)
all_sources := main.c $(all_lib_sources)
test_sources := check_engine.c check_table.c check_relation.c check_util.c
test_targets := $(patsubst %.c,%,$(test_sources))
lib_objects := $(patsubst %.c,%.o,$(all_lib_sources))
objects := $(patsubst %.c,%.o,$(all_sources))
target := tsql
apt_pkgs := bison flex check
test_libs := -lcheck -lm -lrt -pthread

.PHONY: test

# run as sudo
ubuntu.setup:
	apt-get install $(apt_pkgs)

# Always recompile if a header changes
#
$(target): $(objects) $(all_headers)
	$(CC) $(objects) -o $@

grammar.h: grammar.c

clobber: clean
	-rm $(target)

clean:
	-rm $(objects) $(generated_sources) $(generated_headers) \
	$(test_targets)

# Can we define a rule to build a test target?
#
test_targets: $(lib_objects) $(test_sources) $(all_headers)
	gcc -o check_engine $(lib_objects) check_engine.c $(test_libs)
	gcc -o check_relation $(lib_objects) check_relation.c $(test_libs)
	gcc -o check_table $(lib_objects) check_table.c $(test_libs)
	gcc -o check_util $(lib_objects) check_util.c $(test_libs)

test: test_targets
	./check_engine
	./check_relation
	./check_table
	./check_util

valgrind: $(test_targets)
	valgrind ./check_engine
	valgrind ./check_relation
	valgrind ./check_table
	valgrind ./check_util

# Always recompile if a header changes
#
%.o: %.c $(all_headers)
	$(CC) $(CFLAGS) -c $<

%.c: %.y
	$(BISON) -d $(BISONFLAGS) -o $@ $<

%.c: %.l $(headers)
	$(FLEX) $(FLEXFLAGS) -o $@ $<

all: $(target)
