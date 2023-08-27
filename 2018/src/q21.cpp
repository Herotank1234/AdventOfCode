#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <functional>
#include <unordered_set>

typedef struct Instruction {
  std::string opcode;
  std::vector<int> operands;
  Instruction(std::string opcode1, std::vector<int> operands1) :
    opcode(opcode1), operands(operands1) {}

  void printInstruction() {
    std::cout << opcode << " ";
    for(auto i : operands) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
} Instruction;

void addr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] + regs[instr.operands[1]];
}

void addi(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] + instr.operands[1];
}

void mulr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] * regs[instr.operands[1]];
}

void muli(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] * instr.operands[1];
}

void banr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] & regs[instr.operands[1]];
}

void bani(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] & instr.operands[1];
}

void borr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] | regs[instr.operands[1]];
}

void bori(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] | instr.operands[1];
}

void setr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]];
}

void seti(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = instr.operands[0];
}

void gtir(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = instr.operands[0] > regs[instr.operands[1]];
}

void gtri(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] > instr.operands[1]; 
}

void gtrr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] > regs[instr.operands[1]];
}

void eqir(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = instr.operands[0] == regs[instr.operands[1]];
}

void eqri(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] == instr.operands[1];
}

void eqrr(std::vector<int>& regs, Instruction instr) {
  regs[instr.operands[2]] = regs[instr.operands[0]] == regs[instr.operands[1]];
}

int runProgram(int ip, std::vector<Instruction> instructions) {
  int index = 0;
  std::unordered_map<std::string, std::function<
    void(std::vector<int>&, Instruction)>> funcMap = {
    {"addr", addr}, {"addi", addi}, {"mulr", mulr}, {"muli", muli},
    {"banr", banr}, {"bani", bani}, {"borr", borr}, {"bori", bori},
    {"setr", setr}, {"seti", seti}, {"gtir", gtir}, {"gtri", gtri},
    {"gtrr", gtrr}, {"eqir", eqir}, {"eqri", eqri}, {"eqrr", eqrr}
  };
  std::vector<int> regs = {15823996, 0, 0, 0, 0, 0};
  while(index < (int) instructions.size()) {
    Instruction currInstr = instructions[index];
    auto func = funcMap.find(currInstr.opcode)->second;
    regs[ip] = index;
    func(regs, currInstr);
    index = regs[ip];
    index++;
  }
  return regs[0];
}

int runProgramTest(int ip, std::vector<Instruction> instructions) {
  int index = 0;
  std::unordered_map<std::string, std::function<
    void(std::vector<int>&, Instruction)>> funcMap = {
    {"addr", addr}, {"addi", addi}, {"mulr", mulr}, {"muli", muli},
    {"banr", banr}, {"bani", bani}, {"borr", borr}, {"bori", bori},
    {"setr", setr}, {"seti", seti}, {"gtir", gtir}, {"gtri", gtri},
    {"gtrr", gtrr}, {"eqir", eqir}, {"eqri", eqri}, {"eqrr", eqrr}
  };
  std::vector<int> regs = {0, 0, 0, 0, 0, 0};
  std::unordered_set<int> seen;
  int last = 0, i = 0;
  while(index < (int) instructions.size()) {
    Instruction currInstr = instructions[index];
    auto func = funcMap.find(currInstr.opcode)->second;
    regs[ip] = index;
    if(index == 28) {
      auto search = seen.find(regs[4]);
      if(search != seen.end()) {
        return last;
      } else {
        last = regs[4];
        seen.insert(last);
        i++;
      }
    }
    func(regs, currInstr);
    index = regs[ip];
    index++;
  }
  return -1;
}

int main() {
  std::ifstream ifstrm("./data/q21.txt", std::ios::in);
  int ip = 0;
  std::vector<Instruction> instructions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex matchIP("#ip ([\\d])\\s");
    std::smatch smIP;
    if(std::regex_match(line, smIP, matchIP)) {
      ip = stoi(smIP[1]);
    } else {
      std::cout << "No IP found" << std::endl;
    }
    while(std::getline(ifstrm, line)) {
      std::regex match("([a-z]{4}) ([\\d]+) ([\\d]+) ([\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        std::vector<int> operands = {stoi(sm[2]), stoi(sm[3]), stoi(sm[4])};
        instructions.emplace_back(sm[1], operands);
      }
    }
  }

  std::cout << "Answer to part 1: " << runProgram(ip, instructions) << std::endl;
  std::cout << "Answer to part 2: " << runProgramTest(ip, instructions) << std::endl;
  return 0;
}