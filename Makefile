OBJECTS = main.o utils.o employee.o disk.o
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Og -ggdb

all: $(OBJECTS)
	cc $(CFLAGS) -o a.out $(OBJECTS) 

.c.o:
	cc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) *.out
