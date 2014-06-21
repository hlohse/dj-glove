include ../Config.mk

all: dir $(OBJ)

build/%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $^ -o $@

dir:
	-@mkdir $(PC_BUILD_DIR) 2>/dev/null || true

clean:
	-@rm -rf $(PC_BUILD_DIR) 2>/dev/null || true

