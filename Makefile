CC					= gcc
SRC 				= src/employee.c src/shell.c src/main.c src/search.c src/sqlite.c src/table.c src/utils.c libs/sqlite/sqlite3.c
OBJECTS 		= libs/static/libraylib.a
ESSENTIAL 	= $(SRC:.c=.o)
CFLAGS 			+= -std=c2x -pedantic -Wall -Wextra -g3 -ggdb
LDFLAGS 		+= -leditline

all: $(DEP) $(ESSENTIAL)
	$(CC) $(CFLAGS) -o emp.out $(ESSENTIAL) $(OBJECTS) $(LDFLAGS)

DEP: raylib raygui

raylib:
	# TODO: add necessar setup steps for raylib
	zig build ./libs/raylib

raygui:
	# TODO: add necessary setup steps for raygui

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

docs:
	doxygen src/

clean:
	rm -f $(ESSENTIAL) *.out *.plist */**.gch
