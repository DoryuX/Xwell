CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c \
		ident.h \
		utils.c \
		parse.c
OBJECTS=$(SOURCES:.c=.o)
EXEC=xwell

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXEC)
