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

long getRecoveredFreq(const std::vector<Instruction>& instrs) {
  // Get all the possible names of registers
  std::unordered_set<std::string> regNames;
  for(const Instruction& i : instrs) {
    if(i._isAReg) regNames.insert({i._regA});
  }

  // Initialise registers
  std::unordered_map<std::string, long> regs;
  for(std::string name : regNames) {
    regs.insert({name, 0});
  }

  // Run instructions until a valid rcv
  size_t index = 0;
  std::stack<long> sentValues;
  while(index >= 0 && index < instrs.size()) {
    // std::cout << index << std::endl;
    const Instruction& currInstr = instrs[index];
    if(currInstr._opcode == "snd") {
      sentValues.push(regs.find(currInstr._regA)->second);
    } else if(currInstr._opcode == "set") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second = regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second = currInstr._valB;
      }
    } else if(currInstr._opcode == "add") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second += regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second += currInstr._valB;
      }
    } else if(currInstr._opcode == "mul") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second *= regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second *= currInstr._valB;
      }
    } else if(currInstr._opcode == "mod") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second %= regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second %= currInstr._valB;
      }
    } else if(currInstr._opcode == "rcv") {
      if(regs.find(currInstr._regA)->second != 0) return sentValues.top();
    } else if(currInstr._opcode == "jgz") {
      if(currInstr._isAReg) {
        if(regs.find(currInstr._regA)->second > 0) {
          if(currInstr._isBReg) {
            index += regs.find(currInstr._regB) -> second - 1;
          } else {
            index += currInstr._valB - 1;
          }
        }
      } else {
        if(currInstr._valA > 0) {
          if(currInstr._isBReg) {
            index += regs.find(currInstr._regB) -> second - 1;
          } else {
            index += currInstr._valB - 1;
          }
        }
      }
    }
    index++;
  }
  return -1;
}

std::string coroutine(size_t& index, std::unordered_map<std::string, long>& regs,
  std::queue<long>& in, std::queue<long>& out, const std::vector<Instruction>& instrs) {
    while(index >= 0 && index < instrs.size()) {
    const Instruction& currInstr = instrs[index];
    if(currInstr._opcode == "snd") {
      if(currInstr._isAReg) {
        out.push(regs.find(currInstr._regA)->second);
      } else {
        out.push(currInstr._valA);
      }
      index++;
      return "sent";
    } else if(currInstr._opcode == "set") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second = regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second = currInstr._valB;
      }
    } else if(currInstr._opcode == "add") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second += regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second += currInstr._valB;
      }
    } else if(currInstr._opcode == "mul") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second *= regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second *= currInstr._valB;
      }
    } else if(currInstr._opcode == "mod") {
      if(currInstr._isBReg) {
        regs.find(currInstr._regA)->second %= regs.find(currInstr._regB)->second;
      } else {
        regs.find(currInstr._regA)->second %= currInstr._valB;
      }
    } else if(currInstr._opcode == "rcv") {
      if(in.empty()) {
        return "wait";
      } else {
        regs.find(currInstr._regA)->second = in.front();
        in.pop();
      }
    } else if(currInstr._opcode == "jgz") {
      if(currInstr._isAReg) {
        if(regs.find(currInstr._regA)->second > 0) {
          if(currInstr._isBReg) {
            index += regs.find(currInstr._regB) -> second - 1;
          } else {
            index += currInstr._valB - 1;
          }
        }
      } else {
        if(currInstr._valA > 0) {
          if(currInstr._isBReg) {
            index += regs.find(currInstr._regB) -> second - 1;
          } else {
            index += currInstr._valB - 1;
          }
        }
      }
    }
    index++;
  }
  return "success";
}


long runTwo(const std::vector<Instruction>& instrs) {
  // Get all the possible names of registers
  std::unordered_set<std::string> regNames;
  for(const Instruction& i : instrs) {
    if(i._isAReg) regNames.insert({i._regA});
  }

  // Initialise registers
  std::unordered_map<std::string, long> regsProg1;
  std::unordered_map<std::string, long> regsProg2;
  for(std::string name : regNames) {
    regsProg1.insert({name, 0});
    regsProg2.insert({name, 0});
  }
  regsProg1["p"] = 0;
  regsProg2["p"] = 1;

  std::queue<long> prog1Out;
  std::queue<long> prog2Out;

  std::string prog1Status = "";
  std::string prog2Status = "";

  long oneSent = 0;
  size_t prog1Index = 0, prog2Index = 0;
  // for(int i = 0; i < 4; i++) {
  while(!((prog1Status == "wait" || prog1Status == "success") && 
    (prog2Status == "wait" || prog2Status == "success")) ||
    !(prog1Out.empty() && prog2Out.empty())) {
    prog1Status = coroutine(prog1Index, regsProg1, prog2Out, prog1Out, instrs);
    prog2Status = coroutine(prog2Index, regsProg2, prog1Out, prog2Out, instrs);
    // std::cout << prog1Status << " " << prog2Status << std::endl;
    // std::cout << prog1Out.size() << " " << prog2Out.size() << std::endl;

    if(prog2Status == "sent") oneSent++;
  }

  return oneSent;
}

int main() {
  std::ifstream ifstrm("./data/q18.txt", std::ios::in);
  std::vector<Instruction> instrs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(snd|set|add|mul|mod|rcv|jgz) ([a-z]|-?[\\d]+) ?([a-z]|-?[\\d]+)?[\\s]?");
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

  std::cout << "Answer to part 1: " << getRecoveredFreq(instrs) << std::endl;
  std::cout << "Answer to part 2: " << runTwo(instrs) << std::endl;
  return 0;
}