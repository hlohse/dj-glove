CC       = g++
CC_FLAGS = -O3 -Wall -Wextra -march=native -I../$(COMMON_DIR) -I../../
LD_FLAGS = 
SRC      = $(wildcard *.cpp)
OBJ      = $(SRC:%.cpp=$(PC_BUILD_DIR)/%.o)
COMMON   = ../$(COMMON_DIR)/$(PC_BUILD_DIR)/*

