OBJECTS = Prepa.o utils.o

all: $(OBJECTS)
	cc -o a.out $(OBJECTS)

.c.o:
	cc -c -o $@ $<

clean:
	rm -f $(OBJECTS)
