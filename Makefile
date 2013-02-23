CC := gcc

CFLAGS :=

BISON := bison

BISONFLAGS :=

FLEX := flex

FLEXFLAGS :=


HEADERS :=

GENERATED_HEADERS := grammar.h

ALL_HEADERS := $(HEADERS) $(GENERATED_HEADERS)


SOURCES := engine.c main.c backend.c table.c

GENERATED_SOURCES := tokens.c grammar.c

ALL_SOURCES := $(SOURCES) $(GENERATED_SOURCES)


TEST_SOURCES := check_table.c

TEST_TARGETS := $(patsubst %.c,%,$(TEST_SOURCES))


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
	-rm $(OBJECTS) $(GENERATED_SOURCES) $(GENERATED_HEADERS) $(TEST_TARGETS)

$(TEST_TARGETS): table.o
	gcc -o check_table table.o check_table.c -lcheck

test: $(TEST_TARGETS)
	./check_table

# Always recompile if a header changes
#
%.o: %.c $(ALL_HEADERS)
	$(CC) $(CFLAGS) -c $<

%.c: %.y
	$(BISON) -d $(BISONFLAGS) -o $@ $<

%.c: %.l $(HEADERS)
	$(FLEX) $(FLEXFLAGS) -o $@ $<
