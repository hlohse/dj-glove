include Config.mk
.PHONY: all $(PROJECTS) clean

MAKE          = make
BOARD_TAG     = nano328

BUILD_DIR     = build-$(BOARD_TAG)
PROJECT_DIR   = $(shell find Arduino -maxdepth 1 -type d)
PROJECT_DIRS  = $(filter-out Arduino Arduino/common,$(PROJECT_DIR))
MAKEFILE      = $(ARDMK_DIR)/Arduino.mk
USER_LIB_PATH = $(ARDMK_DIR)/common
CPPFLAGS     += -I$(USER_LIB_PATH)

PROJECTS      = $(PROJECT_DIRS:Arduino/%=%)
BUILD_DIRS    = $(PROJECT_DIRS:%=%/$(BUILD_DIR))
CLEANS        = $(PROJECTS:%=%.clean)

export ARDUINO_DIR
export AVR_TOOLS_DIR
export ARDMK_DIR
export MONITOR_PORT
export BOARD_TAG
export USER_LIB_PATH
export CPPFLAGS
export MAKEFILE

all: $(PROJECTS)
	$(foreach dir,$(BUILD_DIRS), cp $(dir)/*.hex .;)

$(PROJECTS):
	$(MAKE) -C Arduino/$@

clean: $(CLEANS)
	-@rm -rf *.hex 2>/dev/null || true

$(CLEANS):
	$(MAKE) clean -C Arduino/$(CLEANS:%.clean=%)

