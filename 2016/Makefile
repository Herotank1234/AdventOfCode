CXX = g++
CXXFLAGS = -Wall -g
SRC = ./src
OBJ = ./obj
LIB = -lcrypto
TARGET = q1 q2 q3 q4 q5 q6 q7 q8 q9

.PHONY: all clean

all: $(TARGET)

./%: $(OBJ)/%.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm ./q*