CXX = g++
CXXFLAGS = -Wall -g
SRC = ./src
OBJ = ./obj
TARGET = q1 q2 q3 q4 q5 q6 q7 q8 q9 q10 q11 q12 q13 q14 q15 q16 q17 q18 q19 q20 q21 q22 q23 q24 q25

.PHONY: all clean

all: $(TARGET)

./%: $(OBJ)/%.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm ./q*