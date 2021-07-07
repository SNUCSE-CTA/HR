CXX := g++

CXXFLAGS := -std=c++17 -O3 -fsanitize=address
CPPFLAGS := -Iinclude

SRC := src
OBJ := obj

SRCS := $(wildcard $(SRC)/*.cc)
OBJS := $(SRCS:$(SRC)/%.cc=$(OBJ)/%.o)

TARGET := program
INPUT := data/sample.igraph


.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET) $(INPUT)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cc | $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $@

clean:
	rm -rv $(TARGET) $(OBJ)

