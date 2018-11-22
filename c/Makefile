# MT, 2018nov22
#
# - Original source: http://mrbook.org/blog/tutorials/make/

CC=gcc

#CFLAGS=-c -O3 -std=gnu11 -Wall
CFLAGS=-c -g -std=gnu11 -Wall
LDFLAGS=
SOURCES=main.c mtalloc/alloc.c mtalloc/mem.c mtalloc/node.c mtalloc/nodemem.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mtalloc_sample

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
