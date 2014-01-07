TRUNK ?= $(PWD)/..

TESTS_DIR = $(TRUNK)/src/tests

INCLUDE_PATH = $(TRUNK)/include

CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = ""

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif


ADT_OBJS = adt_list.o adt_stack.o adt_queue.o adt_hash.o

ADT_LIB=libadt.a

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

lib: $(ADT_OBJS)
	$(AR) rcs $(ADT_LIB) $^

all: lib tests

.PHONY: clean tests

tests: lib
	$(MAKE) -C $(TESTS_DIR) $@


clean: 
	make -C $(TESTS_DIR) $@
	-rm -f *.o
	-rm $(ADT_LIB)

bare: clean
	$(MAKE) -C $(TESTS_DIR) $@
