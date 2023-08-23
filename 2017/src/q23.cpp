#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

typedef struct Instruction {
  Instruction(std::string opcode, std::string regA, std::string regB) :
    _opcode(opcode), _regA(regA), _regB(regB), _isAReg(true), _isBReg(true) {}

  Instruction(std::string opcode, std::string regA, long valB) :
    _opcode(opcode), _regA(regA), _valB(valB), _isAReg(true), _isBReg(false) {}

  Instruction(std::string opcode, long valA, std::string regB) :
    _opcode(opcode), _regB(regB), _valA(valA), _isAReg(false), _isBReg(true) {}

  Instruction(std::string opcode, long valA, long valB) :
    _opcode(opcode), _valA(valA), _valB(valB), _isAReg(false), _isBReg(false) {}

  void print() const {
    std::cout << _opcode << " ";
    if(_isAReg) {
      std::cout << _regA << " ";
    } else {
      std::cout << _valA << " ";
    }
    if(_isBReg) {
      std::cout << _regB << " ";
    } else {
      std::cout << _valB << " ";
    }
    std::cout << std::endl;
  }

  std::string _opcode;
  std::string _regA, _regB;
  long _valA, _valB;
  bool _isAReg, _isBReg;
} Instruction;

int countMuls(const std::vector<Instruction>& instrs) {
  std::unordered_map<std::string, int> regs = {
    {"a", 0}, {"b", 0}, {"c", 0}, {"d", 0},
    {"e", 0}, {"f", 0}, {"g", 0}, {"h", 0}
  };
  int muls = 0;
  size_t index = 0;
  while(index >= 0 && index < instrs.size()) {
    const Instruction& i = instrs[index];
    if(i._opcode == "set") {
      regs.find(i._regA)->second = i._isBReg ? regs.find(i._regB)->second : i._valB;
    } else if(i._opcode == "sub") {
      regs.find(i._regA)->second -= i._isBReg ? regs.find(i._regB)->second : i._valB;
    } else if(i._opcode == "mul") {
      regs.find(i._regA)->second *= i._isBReg ? regs.find(i._regB)->second : i._valB;
      muls++;
    } else if(i._opcode == "jnz") {
      if(i._isAReg) {
        if(regs.find(i._regA)->second != 0) {
          index += (i._isBReg ? regs.find(i._regB)->second : i._valB) - 1;
        }
      } else {
        if(i._valA != 0) {
          index += (i._isBReg ? regs.find(i._regB)->second : i._valB) - 1;
        }
      }
    } else {
      std::cout << "Opcode not recognised" << std::endl;
    }
    index++;
  }

  return muls;
}

int runDebug(const std::vector<Instruction>& instrs) {
  // Rewriting the program
  int b = 108100;
  int c = 125117;
  int h = 0;
  while(b - c != 0) {
    int f = 1;
    // Checks if b is composite
    // for(int d = 2; d < b; d++) {
    //   for(int e = 2; e < b; e++) {
    //     if(b == (d * e)) {
    //       f = 0;
    //     }
    //   }
    // }

    // Optimised code here
    for(int i = 2; i < b; i++) {
      if(b % i == 0) {
        f = 0;
        break;
      }
    }
    if(f == 0) h++;
    b += 17;
  }

  return h;
}

int main() {
  std::ifstream ifstrm("./data/q23.txt", std::ios::in);
  std::vector<Instruction> instrs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(set|sub|mul|jnz) ([a-z]|-?[\\d]+) ?([a-z]|-?[\\d]+)?[\\s]?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        if(isdigit(sm[2].str()[0]) || sm[2].str()[0] == '-') {
          if(isdigit(sm[3].str()[0]) || sm[3].str()[0] == '-') {
            instrs.emplace_back(sm[1], stoi(sm[2]), stoi(sm[3]));
          } else {
            instrs.emplace_back(sm[1], stoi(sm[2]), sm[3]);
          }
        } else {
          if(isdigit(sm[3].str()[0]) || sm[3].str()[0] == '-') {
            instrs.emplace_back(sm[1], sm[2], stoi(sm[3]));
          } else {
            instrs.emplace_back(sm[1], sm[2], sm[3]);
          }
        }
      } else {
        std::cout << "No match found" << std::endl;
      }
    } 
  }

  std::cout << "Answer to part 1: " << countMuls(instrs) << std::endl;
  std::cout << "Answer to part 2: " << runDebug(instrs) << std::endl;
  return 0;
}