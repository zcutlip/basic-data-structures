TRUNK ?= ../..
INCLUDE_DIR?=$(TRUNK)/include

CFLAGS += -Wall -Wextra -I$(INCLUDE_DIR)

DEBUG=1

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif

LDFLAGS=-L$(TRUNK)/src/ -ladt

LIB_ADT=libadt.a

TEST_OBJ=test_test_data.o
T_DATA_OBJ=adt_test_data.o

TEST_TEST=test_test_data
TEST_LIST=test_list
TEST_QUEUE=test_queue
TEST_STACK=test_stack

TESTS=$(TEST_TEST) $(TEST_LIST) $(TEST_STACK) $(TEST_QUEUE)

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

all:tests 

tests: $(TESTS)

$(TEST_LIST): $(TEST_LIST).o $(T_DATA_OBJ) $(TRUNK)/src/$(LIB_ADT)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_STACK): $(TEST_STACK).o $(T_DATA_OBJ) $(TRUNK)/src/$(LIB_ADT)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_QUEUE): $(TEST_QUEUE).o $(T_DATA_OBJ) $(TRUNK)/src/$(LIB_ADT)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_TEST): $(TEST_OBJ) $(T_DATA_OBJ)
	$(CC) $^ -o $@


clean:
	-rm -f *.o
	-rm -f $(TESTS)

bare: clean

