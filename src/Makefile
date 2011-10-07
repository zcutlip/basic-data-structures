
TRUNK ?= ..

INCLUDE_PATH = $(TRUNK)/include

CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = ""

ADT_LIST_OBJ = adt_list.o

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@


all: $(ADT_LIST_OBJ)


clean:
	-rm -f *.o

