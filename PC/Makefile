CC      = g++
CCFLAGS = -std=c++11 -O0 -g -MMD -Wall -Wextra -march=native -funroll-loops
LDFLAGS = -lbluetooth -lasound

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=build/%.o)

all: build $(PROJECTS)

build:
	-@mkdir build

$(PROJECTS): $(OBJECTS)
	$(CC) -o build/$@ $(filter-out $(PROJECTS:%=build/%.o),$(OBJECTS)) build/$@.o $(LDFLAGS)

build/%.o: %.cpp
	$(CC) -c $(CCFLAGS) $^ -o $@

clean:
	-@rm -rf build 2>/dev/null || true

