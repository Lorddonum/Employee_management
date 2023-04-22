OBJECTS = src/main.o src/utils.o src/employee.o src/disk.o src/graphx.o
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Og -ggdb
LDFLAGS = -lncurses

all: $(OBJECTS)
	cc $(CFLAGS) -o emp.out $(OBJECTS) $(LDFLAGS)

.c.o:
	cc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) *.out *.plist */**.gch
