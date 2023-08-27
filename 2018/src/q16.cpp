#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <functional>

typedef struct Sample {
  std::vector<int> before, instr, after;
  Sample(std::vector<int> before1, std::vector<int> instr1,
    std::vector<int> after1) : before(before1), instr(instr1),
    after(after1) {}

  void printSample() {
    for(int b : before) {
      std::cout << b << " ";
    }
    std::cout << std::endl;
    for(int b : instr) {
      std::cout << b << " ";
    }
    std::cout << std::endl;
    for(int b : after) {
      std::cout << b << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
} Sample;

std::vector<int> addr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] + before[instr[2]];
  return before;
}

std::vector<int> addi(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] + instr[2];
  return before;
}

std::vector<int> mulr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] * before[instr[2]];
  return before;
}

std::vector<int> muli(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] * instr[2];
  return before;
}

std::vector<int> banr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] & before[instr[2]];
  return before;
}

std::vector<int> bani(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] & instr[2];
  return before;
}

std::vector<int> borr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] | before[instr[2]];
  return before;
}

std::vector<int> bori(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] | instr[2];
  return before;
}

std::vector<int> setr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]];
  return before;
}

std::vector<int> seti(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = instr[1];
  return before;
}

std::vector<int> gtir(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = instr[1] > before[instr[2]];
  return before;
}

std::vector<int> gtri(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] > instr[2]; 
  return before;
}

std::vector<int> gtrr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] > before[instr[2]];
  return before;
}

std::vector<int> eqir(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = instr[1] == before[instr[2]];
  return before;
}

std::vector<int> eqri(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] == instr[2];
  return before;
}

std::vector<int> eqrr(std::vector<int> before, std::vector<int> instr) {
  before[instr[3]] = before[instr[1]] == before[instr[2]];
  return before;
}

int getPossibleOpcodes(const std::vector<Sample> samples) {
  std::unordered_map<std::string, std::function<
    std::vector<int>(std::vector<int>, std::vector<int>)
  >> funcMap = {
    {"addr", addr},
    {"addi", addi},
    {"mulr", mulr},
    {"muli", muli},
    {"banr", banr},
    {"bani", bani},
    {"borr", borr},
    {"bori", bori},
    {"setr", setr},
    {"seti", seti},
    {"gtir", gtir},
    {"gtri", gtri},
    {"gtrr", gtrr},
    {"eqir", eqir},
    {"eqri", eqri},
    {"eqrr", eqrr}
  };

  int res =  0;
  for(Sample s : samples) {
    std::vector<std::string> possibilities = {"addr", "addi", "mulr", "muli", "banr",
      "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", 
      "eqrr"};
    int validPossibilities = 0;
    for(std::string p : possibilities) {
      auto func = funcMap.find(p)->second;
      auto result = func(s.before, s.instr);
      if(result == s.after) {
        validPossibilities++;
      }
    }
    if(validPossibilities >= 3) res++;
  }

  return res;
}

int runProgram(std::vector<Sample> samples, std::vector<std::vector<int>> instrs) {
  std::unordered_map<int, std::vector<std::string>> opcodePossibilities;
  std::vector<std::string> possibilities = {"addr", "addi", "mulr", "muli", "banr",
    "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", 
    "eqrr"};
  std::unordered_map<std::string, std::function<
    std::vector<int>(std::vector<int>, std::vector<int>)
  >> funcMap = {
    {"addr", addr},
    {"addi", addi},
    {"mulr", mulr},
    {"muli", muli},
    {"banr", banr},
    {"bani", bani},
    {"borr", borr},
    {"bori", bori},
    {"setr", setr},
    {"seti", seti},
    {"gtir", gtir},
    {"gtri", gtri},
    {"gtrr", gtrr},
    {"eqir", eqir},
    {"eqri", eqri},
    {"eqrr", eqrr}
  };

  for(int i = 0; i < 16; i++) {
    opcodePossibilities.insert({i, possibilities});
  }

  for(Sample s : samples) {
    std::vector<std::string>& currPossibilities = 
      opcodePossibilities.find(s.instr[0])->second;
    std::vector<std::string> nextPossibilities;
    for(std::string p : currPossibilities) {
      auto func = funcMap.find(p)->second;
      auto result = func(s.before, s.instr);
      if(result == s.after) {
        nextPossibilities.push_back(p);
      }
    }
    currPossibilities = nextPossibilities;
  }

  int changes = -1;
  while(changes != 0) {
    changes = 0;
    for(auto it = opcodePossibilities.begin(); it != opcodePossibilities.end(); it++) {
      if(it->second.size() == 1) {
        for(auto it1 = opcodePossibilities.begin(); it1 != opcodePossibilities.end(); it1++) {
          if(it != it1) {
            for(auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
              if((*it2) == it->second[0]) {
                changes++;
                it1->second.erase(it2);
                break;
              }
            }
          }
        }
      }
    }
  }

  std::vector<int> regs = {0, 0, 0, 0};
  for(auto instr : instrs) {
    std::string instrStr = (opcodePossibilities.find(instr[0])->second)[0];
    auto func = funcMap.find(instrStr)->second;
    auto res = func(regs, instr);
    regs = res;
  }

  return regs[0];
}

int main() {
  std::ifstream ifstrm("./data/q16.txt", std::ios::in);
  std::vector<Sample> samples;
  std::vector<std::vector<int>> instrs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      if(line.substr(0, 6) == "Before") {
        std::vector<int> before, instr, after;
        std::regex matchBefore("Before: \\[([\\d]), ([\\d]), ([\\d]), ([\\d])\\][\\s]?");
        std::smatch smBefore;
        if(std::regex_match(line, smBefore, matchBefore)) {
          for(int i = 1; i <= 4; i++) {
            before.push_back(stoi(smBefore[i]));
          }
        }
        
        std::getline(ifstrm, line);
        std::regex matchInstr("([\\d]+) ([\\d]+) ([\\d]+) ([\\d]+)[\\s]?");
        std::smatch smInstr;
        if(std::regex_match(line, smInstr, matchInstr)) {
          for(int i = 1; i <= 4; i++) {
            instr.push_back(stoi(smInstr[i]));
          }
        }

        std::getline(ifstrm, line);
        std::regex matchAfter("After:  \\[([\\d]), ([\\d]), ([\\d]), ([\\d])\\][\\s]?");
        std::smatch smAfter;
        if(std::regex_match(line, smAfter, matchAfter)) {
          for(int i = 1; i <= 4; i++) {
            after.push_back(stoi(smAfter[i]));
          }
        }

        samples.emplace_back(before, instr, after);
        std::getline(ifstrm, line);
        while(line == "") std::getline(ifstrm, line);
      } else {
        std::regex match("([\\d]+) ([\\d]+) ([\\d]+) ([\\d]+)[\\s]?");
        std::smatch sm;
        if(std::regex_match(line, sm, match)) {
          std::vector<int> instr;
          for(int i = 1; i <= 4; i++) {
            instr.push_back(stoi(sm[i]));
          }
          instrs.push_back(instr);
        }
      }
    }
  }

  std::cout << "Answer to part 1: " << getPossibleOpcodes(samples) << std::endl;
  std::cout << "Answer to part 2: " << runProgram(samples, instrs) << std::endl;
  return 0;
}
