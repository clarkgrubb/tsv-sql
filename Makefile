CC := gcc

CFLAGS := -g

BISON := bison

BISONFLAGS :=

FLEX := flex

FLEXFLAGS :=


HEADERS := backend.h def.h ir.h table.h util.h

GENERATED_HEADERS := grammar.h

ALL_HEADERS := $(HEADERS) $(GENERATED_HEADERS)


SOURCES := backend.c ir.c table.c util.c

GENERATED_SOURCES := tokens.c grammar.c

ALL_LIB_SOURCES := $(SOURCES) $(GENERATED_SOURCES)

ALL_SOURCES := main.c $(ALL_LIB_SOURCES)


TEST_SOURCES := check_backend.c check_table.c check_util.c

TEST_TARGETS := $(patsubst %.c,%,$(TEST_SOURCES))

LIB_OBJECTS := $(patsubst %.c,%.o,$(ALL_LIB_SOURCES))

OBJECTS := $(patsubst %.c,%.o,$(ALL_SOURCES))

TARGET := tsv-sql


# Always recompile if a header changes
#
$(TARGET): $(OBJECTS) $(ALL_HEADERS)
	$(CC) $(OBJECTS) -o $@

grammar.h: grammar.c

clobber: clean
	-rm $(TARGET)

clean:
	-rm $(OBJECTS) $(GENERATED_SOURCES) $(GENERATED_HEADERS) \
	$(TEST_TARGETS)

# Can we define a rule to build a test target?
#
test_targets: $(LIB_OBJECTS) $(TEST_SOURCES) $(ALL_HEADERS)
	gcc -o check_backend $(LIB_OBJECTS) check_backend.c -lcheck
	gcc -o check_table $(LIB_OBJECTS) check_table.c -lcheck
	gcc -o check_util $(LIB_OBJECTS) check_util.c -lcheck

test: test_targets
	./check_backend
	./check_table
	./check_util

valgrind: $(TEST_TARGETS)
	valgrind ./check_backend
	valgrind ./check_table
	valgrind ./check_util

# Always recompile if a header changes
#
%.o: %.c $(ALL_HEADERS)
	$(CC) $(CFLAGS) -c $<

%.c: %.y
	$(BISON) -d $(BISONFLAGS) -o $@ $<

%.c: %.l $(HEADERS)
	$(FLEX) $(FLEXFLAGS) -o $@ $<
