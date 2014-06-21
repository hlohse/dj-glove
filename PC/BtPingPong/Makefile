include ../Config.mk

TARGET    = $(notdir $(shell pwd))

all: dir $(OBJ) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LD_FLAGS) -o build/$@.run $^  ../common/build/* -lbluetooth

build/lib%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $^ -o $@

dir:
	-@mkdir build 2>/dev/null || true

clean:
	-@rm -rf build 2>/dev/null || true

