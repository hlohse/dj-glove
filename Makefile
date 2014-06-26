include Config.mk

MAKE           = make
BOARD_TAG      = nano328
MAKEFILE       = $(ARDMK_DIR)/Arduino.mk
USER_LIB_PATH  = $(ARDMK_DIR)/common
CPPFLAGS      += -I$(USER_LIB_PATH) -I../../common
CLEAN_SUFFIX   = .clean

ARDUINO_DIR_   = Arduino
PC_DIR         = PC
COMMON_DIR     = common
ARDUINO_EXT    = .hex
PC_EXT         = .run

ARDUINO_BUILD_DIR     = build-$(BOARD_TAG)
ARDUINO_TARGET_SUFFIX = .arduino
ARDUINO_PROJECT_DIR   = $(shell find $(ARDUINO_DIR_) -maxdepth 1 -type d)
ARDUINO_PROJECTS      = $(filter-out $(ARDUINO_DIR_) $(ARDUINO_DIR_)/$(COMMON_DIR),$(ARDUINO_PROJECT_DIR))
ARDUINO_TARGETS       = $(ARDUINO_PROJECTS:$(ARDUINO_DIR_)/%=%$(ARDUINO_TARGET_SUFFIX))
ARDUINO_BUILD_DIRS    = $(ARDUINO_PROJECTS:%=%/$(ARDUINO_BUILD_DIR))
ARDUINO_CLEANS        = $(ARDUINO_PROJECTS:%=%$(CLEAN_SUFFIX))

PC_BUILD_DIR     = build
PC_TARGET_SUFFIX = .pc
PC_PROJECT_DIR   = $(shell find $(PC_DIR) -maxdepth 1 -type d)
PC_PROJECTS      = $(filter-out $(PC_DIR) $(PC_DIR)/$(COMMON_DIR),$(PC_PROJECT_DIR))
PC_TARGETS       = $(PC_PROJECTS:$(PC_DIR)/%=%$(PC_TARGET_SUFFIX))
PC_BUILD_DIRS    = $(PC_PROJECTS:%=%/$(PC_BUILD_DIR))
PC_CLEANS        = $(PC_PROJECTS:%=%$(CLEAN_SUFFIX))

.PHONY: all pc_common $(ARDUINO_TARGETS) $(PC_TARGETS)

export ARDUINO_DIR
export AVR_TOOLS_DIR
export ARDMK_DIR
export MONITOR_PORT
export BOARD_TAG
export USER_LIB_PATH
export CPPFLAGS
export MAKEFILE
export COMMON_DIR
export PC_BUILD_DIR
export ARDUINO_EXT
export PC_EXT

all: $(ARDUINO_TARGETS) pc_common $(PC_TARGETS)
	$(foreach dir,$(ARDUINO_BUILD_DIRS), cp $(dir)/*$(ARDUINO_EXT) .;)
	$(foreach dir,$(PC_BUILD_DIRS), cp $(dir)/*$(PC_EXT) .;)

$(ARDUINO_TARGETS):
	$(MAKE) -C $(ARDUINO_DIR_)/$(@:%$(ARDUINO_TARGET_SUFFIX)=%)

pc_common:
	$(MAKE) -C $(PC_DIR)/$(COMMON_DIR)

$(PC_TARGETS):
	$(MAKE) -C $(PC_DIR)/$(@:%$(PC_TARGET_SUFFIX)=%)

clean: $(ARDUINO_CLEANS) pc_common_clean $(PC_CLEANS)
	-@rm -rf *$(ARDUINO_EXT) 2>/dev/null || true
	-@rm -rf *$(PC_EXT) 2>/dev/null || true

$(ARDUINO_CLEANS):
	-@$(MAKE) clean -C $(@:%$(CLEAN_SUFFIX)=%)

pc_common_clean:
	-@$(MAKE) clean -C $(PC_DIR)/$(COMMON_DIR)

$(PC_CLEANS):
	-@$(MAKE) clean -C $(@:%$(CLEAN_SUFFIX)=%)

