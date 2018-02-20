CC=gcc
CFLAGS=-c -Wall -Werror
SOURCES=main.c jpeg.c effects.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=jpeg_example
LIBS=-ljpeg

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
