#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <functional>

typedef struct Instruction {
  std::string reg;
  bool inc;
  int diff;
  std::string condReg;
  std::string condOp;
  int condVal;

  Instruction(std::string reg1, bool inc1, int diff1, std::string condReg1, 
    std::string condOp1, int condVal1) : reg(reg1), inc(inc1), diff(diff1), 
    condReg(condReg1), condOp(condOp1), condVal(condVal1) {}

  void print() const {
    std::cout << reg << (inc ? " inc " : " dec ") << diff << " " << condReg << 
      " " << condOp << " " << condVal << std::endl;
  }
} Instruction;

bool lt(const int& a, const int& b) {
  return a < b;
}

bool gt(const int& a, const int& b) {
  return a > b;
}

bool lte(const int& a, const int& b) {
  return a <= b;
}

bool gte(const int& a, const int& b) {
  return a >= b;
}

bool eq(const int& a, const int& b) {
  return a == b;
}

bool neq(const int& a, const int& b) {
  return a != b;
}

int run(const std::vector<Instruction>& instructions) {
  std::unordered_map<std::string, int> regs;
  std::unordered_set<std::string> regNames;
  std::unordered_map<std::string, std::function<bool(const int&, const int&)>> funcPtr = {
    {"<", lt},
    {">", gt},
    {"<=", lte},
    {">=", gte},
    {"==", eq},
    {"!=", neq}
  };

  for(auto inst : instructions) {
    regNames.insert(inst.reg);
    regNames.insert(inst.condReg);
  }

  for(std::string regName : regNames) {
    regs.insert({regName, 0});
  }

  for(auto inst : instructions) {
    auto op = funcPtr.find(inst.condOp)->second;
    int regVal = regs.find(inst.condReg)->second;
    if(op(regVal, inst.condVal)) {
      if(inst.inc) {
        regs[inst.reg] += inst.diff;
      } else {
        regs[inst.reg] -= inst.diff;
      }
    }
  }

  int largest = 0;
  for(auto reg : regs) {
    if(reg.second > largest) largest = reg.second;
  }

  return largest;
}

int runStoreLargest(const std::vector<Instruction>& instructions) {
  std::unordered_map<std::string, int> regs;
  std::unordered_set<std::string> regNames;
  std::unordered_map<std::string, std::function<bool(const int&, const int&)>> funcPtr = {
    {"<", lt},
    {">", gt},
    {"<=", lte},
    {">=", gte},
    {"==", eq},
    {"!=", neq}
  };

  for(auto inst : instructions) {
    regNames.insert(inst.reg);
    regNames.insert(inst.condReg);
  }

  for(std::string regName : regNames) {
    regs.insert({regName, 0});
  }

  int largest = 0;

  for(auto inst : instructions) {
    auto op = funcPtr.find(inst.condOp)->second;
    int regVal = regs.find(inst.condReg)->second;
    if(op(regVal, inst.condVal)) {
      if(inst.inc) {
        regs[inst.reg] += inst.diff;
      } else {
        regs[inst.reg] -= inst.diff;
      }
    }
    if(regs[inst.reg] > largest) largest = regs[inst.reg];
  }

  return largest;
}

int main() {
  std::ifstream ifstrm("./data/q8.txt", std::ios::in);
  std::vector<Instruction> instructions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("([a-z]+) (inc|dec) ([-]?[\\d]+) "
        "if ([a-z]+) (>|<|<=|>=|==|!=) ([-]?[\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        instructions.emplace_back(sm[1], sm[2] == "inc", stoi(sm[3]), sm[4], 
          sm[5], stoi(sm[6]));
      } else {
        std::cout << "No match" << std::endl;
      }
    }
  }


  std::cout << "Answer to part 1: " << run(instructions) << std::endl;
  std::cout << "Answer to part 2: " << runStoreLargest(instructions) << std::endl;
  return 0;
}