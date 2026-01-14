CXX = g++
CXXFLAGS = -Wall -g
SRC = ./src
OBJ = ./obj
TARGET = q1

.PHONY: all clean

all: $(TARGET)

./%: $(OBJ)/%.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm ./q*