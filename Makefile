include Config.mk
.PHONY: all pc_common $(PROJECTS) clean

MAKE           = make
BOARD_TAG      = nano328
MAKEFILE       = $(ARDMK_DIR)/Arduino.mk
USER_LIB_PATH  = $(ARDMK_DIR)/common
CPPFLAGS      += -I$(USER_LIB_PATH)

ARDUINO_BUILD_DIR    = build-$(BOARD_TAG)
ARDUINO_PROJECT_DIR  = $(shell find Arduino -maxdepth 1 -type d)
ARDUINO_PROJECTS     = $(filter-out Arduino Arduino/common,$(ARDUINO_PROJECT_DIR))
ARDUINO_BUILD_DIRS   = $(ARDUINO_PROJECTS:%=%/$(ARDUINO_BUILD_DIR))
ARDUINO_CLEANS       = $(ARDUINO_PROJECTS:%=%.clean.arduino)

PC_PROJECT_DIR  = $(shell find PC -maxdepth 1 -type d)
PC_PROJECTS     = $(filter-out PC PC/common,$(PC_PROJECT_DIR))
PC_BUILD_DIRS   = $(PC_PROJECTS:%=%/build)
PC_CLEANS       = $(PC_PROJECTS:%=%.clean.pc)

export ARDUINO_DIR
export AVR_TOOLS_DIR
export ARDMK_DIR
export MONITOR_PORT
export BOARD_TAG
export USER_LIB_PATH
export CPPFLAGS
export MAKEFILE

all: $(ARDUINO_PROJECTS:Arduino/%=%.arduino) pc_common $(PC_PROJECTS:PC/%=%.pc)
	$(foreach dir,$(ARDUINO_BUILD_DIRS), cp $(dir)/*.hex .;)
	$(foreach dir,$(PC_BUILD_DIRS), cp $(dir)/*.run .;)

$(ARDUINO_PROJECTS:Arduino/%=%.arduino):
	$(MAKE) -C Arduino/$(@:%.arduino=%)

pc_common:
	$(MAKE) -C PC/common

$(PC_PROJECTS:PC/%=%.pc):
	$(MAKE) -C PC/$(@:%.pc=%)

clean: $(ARDUINO_CLEANS) pc_common_clean $(PC_CLEANS)
	-@rm -rf *.hex 2>/dev/null || true
	-@rm -rf *.run 2>/dev/null || true

$(ARDUINO_CLEANS):
	$(MAKE) clean -C $(ARDUINO_CLEANS:%.clean.arduino=%)

pc_common_clean:
	$(MAKE) clean -C PC/common

$(PC_CLEANS):
	$(MAKE) clean -C $(PC_CLEANS:%.clean.pc=%)

