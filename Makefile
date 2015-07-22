CC := gcc
CFLAGS := -Wall -ggdb -g3 
LDFLAGS := 
SOURCES := ${wildcard *.c}
OBJECTS := ${SOURCES:.c=.o}
EXECUTABLE := barcode

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) $(CFLAGSs) 

$(OBJECTS) :
	$(CC) -c $(patsubst %.o,%.c,$@)

clean:
	rm -f $(OBJECTS) 
	rm -f $(EXECUTABLE)
