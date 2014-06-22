include ../Common.mk

TARGET    = $(notdir $(shell pwd))
LD_FLAGS += -lbluetooth

all: dir $(OBJ) $(TARGET)

$(TARGET): $(OBJ) $(COMMON)
	$(CC) -o $(PC_BUILD_DIR)/$@$(PC_EXT) $^ $(LD_FLAGS)

build/%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $^ -o $@

dir:
	-@mkdir $(PC_BUILD_DIR) 2>/dev/null || true

clean:
	-@rm -rf $(PC_BUILD_DIR) 2>/dev/null || true

