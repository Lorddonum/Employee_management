CC					?= zig cc
SRC 				= src/employee.c src/main.c src/gui.c src/search.c src/sqlite.c src/table.c src/utils.c libs/sqlite/sqlite3.c
OBJECTS 		= libs/raylib/zig-out/lib/libraylib.a
ESSENTIAL 	= $(SRC:.c=.o) src/shell.o
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

clean:
	rm -f $(ESSENTIAL) *.out *.plist */**.gch
