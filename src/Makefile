TRUNK ?= ..

INCLUDE_PATH = $(TRUNK)/include

CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = ""

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif


ADT_LIST_OBJ = adt_list.o

ADT_LIB=libadt.a

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@


all: lib

lib: $(ADT_LIST_OBJ)
	$(AR) rcs $(ADT_LIB) $^


clean:
	-rm -f *.o
	-rm $(ADT_LIB)

