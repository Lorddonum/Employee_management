CC					?= zig cc
SRC 				= src/disk.c src/employee.c src/main.c src/search.c src/sqlite.c src/table.c src/utils.c libs/sqlite/sqlite3.c
OBJECTS 		= $(SRC:.c=.o) libs/raylib/zig-out/lib/libraylib.a
CFLAGS 			+= -std=c2x -pedantic -Wall -Wextra -g3 -ggdb

all: $(DEP) $(OBJECTS)
	$(CC) $(CFLAGS) -o emp.out $(OBJECTS) $(LDFLAGS)

DEP: raylib raygui

raylib:
	# TODO: add necessar setup steps for raylib
	zig build ./libs/raylib

raygui:
	# TODO: add necessary setup steps for raygui

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) *.out *.plist */**.gch
