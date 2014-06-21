include ../Config.mk

all: dir $(OBJ)

build/lib%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $^ -o $@

dir:
	-@mkdir build 2>/dev/null || true

clean:
	-@rm -rf build 2>/dev/null || true

