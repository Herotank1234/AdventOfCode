#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

typedef struct Instruction {
  Instruction(std::string instr1, int width1, int length1) :
    instr(instr1), width(width1), length(length1) {}
  Instruction(std::string instr1, std::string axis1, int index1, int shift1) :
    instr(instr1), axis(axis1), index(index1), shift(shift1) {}
  std::string instr;
  std::string axis;
  int width, length, index, shift;
} Instruction;

void doRect(const Instruction& i, std::vector<std::vector<char>>& board) {
  for(int y = 0; y < i.length; y++) {
    for(int x = 0; x < i.width; x++) {
      board[y][x] = '#';
    }
  }
}

void doRotate(const Instruction& i, std::vector<std::vector<char>>& board) {
  if(i.axis == "x") {
    std::vector<char> toBeRotated;
    for(auto row : board) {
      toBeRotated.push_back(row[i.index]);
    }
    std::rotate(toBeRotated.rbegin(), toBeRotated.rbegin() + i.shift, toBeRotated.rend());
    for(size_t k = 0; k < board.size(); k++) {
      board[k][i.index] = toBeRotated[k];
    }
  } else {
    std::rotate(board[i.index].rbegin(), board[i.index].rbegin() + i.shift, 
      board[i.index].rend());
  }
}

int swipeCardAndCountLitPixels(const std::vector<Instruction>& instructions) {
  std::vector<std::vector<char>> board(6, std::vector<char>(50, '.'));
  for(const Instruction& i : instructions) {
    if(i.instr == "rect") {
      doRect(i, board);
    } else if(i.instr == "rotate") {
      doRotate(i, board);
    } else {
      std::cout << "Instruction not recognised: " << i.instr << std::endl;
    }
  }
  int count = 0;
  for(auto row : board) {
    for(auto col : row) {
      if(col == '#') count++;
    }
  }
  return count;
}

int swipeCardAndDisplay(const std::vector<Instruction>& instructions) {
  std::vector<std::vector<char>> board(6, std::vector<char>(50, '.'));
  for(const Instruction& i : instructions) {
    if(i.instr == "rect") {
      doRect(i, board);
    } else if(i.instr == "rotate") {
      doRotate(i, board);
    } else {
      std::cout << "Instruction not recognised: " << i.instr << std::endl;
    }
  }
  std::cout << std::endl;
  for(auto row : board) {
    for(auto col : row) {
      std::cout << col;
    }
    std::cout << std::endl;
  }
  return 0;
}

int main() {
  std::ifstream ifstrm("./data/q8.txt", std::ios::in);
  std::vector<Instruction> instructions;
  std::regex match("(((rect) ([\\d]+)x([\\d]+))|"
    "((rotate) (?:row|column) (y|x)=([\\d]+) by ([\\d]+)))[\\s]?");
  std::smatch sm;
  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        if(sm[3] != "") {
          instructions.emplace_back(sm[3], stoi(sm[4]), stoi(sm[5]));
        } else {
          instructions.emplace_back(sm[7], sm[8], stoi(sm[9]), stoi(sm[10]));
        }
      } else {
        std::cout << "No match: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << swipeCardAndCountLitPixels(instructions) << std::endl;
  std::cout << "Answer to part 2: " << swipeCardAndDisplay(instructions) << std::endl;
  return 0;
}