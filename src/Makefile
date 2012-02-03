TRUNK ?= $(PWD)/..

TESTS_DIR = $(TRUNK)/src/tests

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

lib: $(ADT_LIST_OBJ)
	$(AR) rcs $(ADT_LIB) $^

all: lib tests

.PHONY: clean tests

tests: lib
	$(MAKE) -C $(TESTS_DIR) $@



clean:
	-rm -f *.o
	-rm $(ADT_LIB)

bare: clean
	$(MAKE) -C $(TESTS_DIR) $@
