include ../Makefile.include
.PHONY: clean

BUILD      = $(call FIXPATH,$(BUILD_DIR))
INC        = $(call FIXPATH,$(INC_DIR))
SRC        = $(call FIXPATH,$(SRC_DIR))
BIN        = $(call FIXPATH,$(BIN_DIR))
THIRDPARTY = $(call FIXPATH,$(THIRPARTY_DIR))

CC_FLAGS = $(CC_FLAGS_COMMON) -I$(INC)
LD_FLAGS = $(LD_FLAGS_COMMON) -L$(THIDPARTY)

clean:
	-@rm $(call FIXPATH,$(BUILD)/*) 2>/dev/null || true 
	-@$(foreach target,$(TARGETS),\
	    rm $(call FIXPATH,$(BIN)/$(target)$(EXECSUFFIX)) 2>/dev/null || true;)

