OBJECTS 	= src/main.o src/utils.o src/employee.o src/disk.o
CFLAGS 		= -std=c11 -Wall -Wextra -pedantic -Og
CFLAGS 		+= $(shell pkg-config --cflags sdl2)
LDFLAGS 	= $(shell pkg-config --libs sdl2)

all: $(OBJECTS)
	cc $(CFLAGS) -o emp.out $(OBJECTS) $(LDFLAGS)

.c.o:
	cc $(CFLAGS) -c -o $@ $<

run: all
	./emp.out
clean:
	rm -f $(OBJECTS) *.out *.plist */**.gch
