TRUNK ?= $(PWD)/..

TESTS_DIR = $(TRUNK)/src/tests

INCLUDE_PATH = $(TRUNK)/include

CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = ""

ADT_OBJS = adt_list.o adt_stack.o adt_queue.o adt_hash.o adt_queue.o adt_ring_buffer.o
ifneq ($(ALL_CLANG_WARNINGS),0)
	CFLAGS += -Wno-missing-field-initializers -Wno-missing-prototypes -Werror=return-type -Wunreachable-code -Werror=deprecated-objc-isa-usage -Werror=objc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-non-literal-null-conversion -Wno-objc-literal-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-sign-conversion -Winfinite-recursion -Wno-comma -Wno-block-capture-autoreleasing -Wno-strict-prototypes -Wdeprecated-declarations -Wno-trigraphs
endif

ADT_LIB=libadt.a

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif

ifeq ($(ANALYZE),1)
	CC = clang
	CFLAGS += --analyze -g -O0
endif


ifeq ($(ADT_XNU),1)
	ADT_OBJS +=adt_xnu_malloc.o adt_xnu_synchronization.o
	CFLAGS +=-DADT_XNU -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks/Kernel.framework/Headers
else
ifeq ($(ADT_POSIX),1)
	CFLAGS+=-DADT_POSIX
	ADT_OBJS+=adt_posix_synchronization.o
endif #adt_xnu
endif #adt_posix



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
