include ../Makefile.include
include ../Makefile.config

BUILD = $(call FIXPATH,$(BUILD_DIR)/)
INC   = $(call FIXPATH,$(INC_DIR)/)
SRC   = $(call FIXPATH,$(SRC_DIR)/)
BIN   = $(call FIXPATH,$(BIN_DIR)/)

CPPS = $(wildcard $(SRC)*.cpp)
OBJS = $(addprefix $(BUILD),lib$(notdir $(CPPS:.cpp=.o)))
BINS = $(addprefix $(BIN),$(addsuffix $(ARDUINOSUFFIX),$(TARGETS)))

CC_FLAGS = $(CC_FLAGS_COMMON) -mmcu=atmega328p -I$(INC) \
           -I$(ARDUINO_CORE_PATH) -I$(ARDUINO_VAR_PATH) \
           -I/usr/lib/gcc/avr/4.7.2/plugin/include/config/avr/
LD_FLAGS = $(LD_FLAGS_COMMON)

all: $(BINS)

$(BINS): ar $(OBJS)
	$(AVRCC) -o $@ $(BUILD)static.a $(LD_FLAGS)

ar: $(OBJS)
	avr-ar rcs $(BUILD)static.a $(OBJS)

$(OBJS): $(CPPS)
	$(AVRCC) $(CC_FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-@rm $(BUILD)* 2>/dev/null || true 
	-@$(foreach target,$(TARGETS),\
	    rm $(BIN)$(target)$(ARDUINOSUFFIX) 2>/dev/null || true;)

