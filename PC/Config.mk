CC       = g++
CC_FLAGS = -O3 -Wall -Wextra -march=native -I../common
LD_FLAGS = 
SRC      = $(wildcard *.cpp)
OBJ      = $(SRC:%.cpp=build/lib%.o)

