TRUNK ?= ../..
INCLUDE_DIR?=$(TRUNK)/include

CFLAGS += -Wall -Wextra -I$(INCLUDE_DIR)

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif

TEST_OBJ=test_test_data.o
T_DATA_OBJ=adt_test_data.o

TEST_TEST=test_test_data

TESTS=$(TEST_TEST)

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

all:$(TESTS)


$(TEST_TEST): $(TEST_OBJ) $(T_DATA_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@


clean:
	-rm -f *.o
	-rm -f $(TEST_TEST)


