export TRUNK ?= $(PWD)

EXPORT_DIR=$(TRUNK)/export
INCLUDE_DIR=$(TRUNK)/include
SRC_DIR=$(TRUNK)/src

ADT_LIB=libadt.a

all:
	$(MAKE) -C $(SRC_DIR) $@


export: all
	mkdir $(EXPORT_DIR) || exit 1
	cp $(SRC_DIR)/$(ADT_LIB) $(EXPORT_DIR) || exit 1
	cp $(INCLUDE_DIR)/*.h $(EXPORT_DIR) || exit 1

clean:
	$(MAKE) -C $(SRC_DIR) $@




bare:
	$(MAKE) -C $(SRC_DIR) $@
	-rm -rf $(EXPORT_DIR)

