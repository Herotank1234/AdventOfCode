#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

typedef struct Instruction {
  Instruction(std::string opcode, std::string reg) :
    opcode(opcode), reg_left(reg) {}

  Instruction(std::string opcode, int value_left, int value_right) : 
    opcode(opcode), value_left(value_left), is_value_left(true), 
    value_right(value_right), is_value_right(true) {}

  Instruction(std::string opcode, std::string reg_left, int value_right) :
    opcode(opcode), reg_left(reg_left), value_right(value_right), is_value_right(true) {}

  Instruction(std::string opcode, int value_left, std::string reg_right) :
    opcode(opcode), value_left(value_left), is_value_left(true), reg_right(reg_right) {}

  Instruction(std::string opcode, std::string reg_left, std::string reg_right) :
    opcode(opcode), reg_left(reg_left), reg_right(reg_right) {}

  void print() {
    std::cout << opcode << " " << reg_left << " " << value_left 
      << " " << reg_right << " " << value_right << std::endl;
  }

  int get_left_value(const std::unordered_map<std::string, int> &registers) const {
    if(is_value_left) { return value_left; }
    return registers.find(reg_left)->second;
  }

  int get_right_value(const std::unordered_map<std::string, int> &registers) const {
    if(is_value_right) { return value_right; }
    return registers.find(reg_right)->second;
  }

  std::string opcode = "";
  std::string reg_left = "";
  int value_left = 0;
  bool is_value_left = false;
  std::string reg_right = "";
  int value_right = 0;
  bool is_value_right = false;
} Instruction;

bool is_numeric(const std::string &str) {
  for(const char &c : str) {
    if(!(c == '-' || (c >= '0' && c <= '9'))) {
      return false;
    }
  }
  return true;
}

void print(const std::unordered_map<std::string, int> &registers) {
  for(const auto &kv : registers) {
    std::cout << kv.first << ": " << kv.second << ", ";
  }
  std::cout << std::endl;
}

bool is_alternating(const std::vector<int> out) {
  int check = 0;
  for(int o : out) {
    if(o != check) return false;
    check = 1 - check;
  }
  return true;
}

bool run(
  const std::vector<Instruction> &instructions,
  int initial_value)
{
  std::unordered_map<std::string, int> registers = {
    {"a", initial_value},
    {"b", 0},
    {"c", 0},
    {"d", 0}
  };
  std::vector<int> out;

  size_t index = 0;
  while(index < instructions.size() && out.size() <= 30) {
    std::string opcode = instructions[index].opcode;
    if(opcode == "cpy") {
      int cpy_value = instructions[index].get_left_value(registers);
      registers[instructions[index].reg_right] = cpy_value;
      index++;

    } else if(opcode == "inc") {
      registers[instructions[index].reg_left]++;
      index++;

    } else if(opcode == "dec") {
      registers[instructions[index].reg_left]--;
      index++;

    } else if(opcode == "jnz") {
      int check = instructions[index].get_left_value(registers);
      if(check != 0) {
        int offset = instructions[index].get_right_value(registers);
        index += offset;
      } else {
        index++;
      }

    } else if(opcode == "out") {
      int value = instructions[index].get_left_value(registers);
      out.push_back(value);
      index++;

    } else {
      std::cout << "Opcode " << opcode << " not recognised" << std::endl;
    }
  }

  return is_alternating(out);
}

int try_run(const std::vector<Instruction> &instructions) {
  /*
    While trying different inputs and looking at the registers when it reaches the
    output instruction, we can see the following
    d = input + 2534
    a = the current total after integer division by 2
    b = the remainder after integer division by 2
    c = 0;

    Hence, we need a number that produces an alternating remainder
    of 0 and 1 after integer division by 2 that is greater or equal to 2534

    Our answer will be that number - 2534.
  */
  bool rem_one = false;
  int i = 1;
  while(i < 2345) {
    if(rem_one) {
      i = (i * 2) + 1;
    } else {
      i = i * 2;
    }
    rem_one = !rem_one;
  }

  int answer =  i - 2534;

  /* 
    We can verify this by running the program with the answer and 
    checking the output is alternating
  */
  assert(run(instructions, answer));
  return answer;
}

int main() {
  std::ifstream ifstrm("./data/q25.txt", std::ios::in);
  std::vector<Instruction> instructions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(cpy|inc|dec|jnz|out) ([a-z]|-?[\\d]+)(:? ([a-z]|-?[\\d]+))?\\s?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        if(sm[4] == "") {
          instructions.emplace_back(sm[1], sm[2]);
        } else {
          if(is_numeric(sm[2]) && is_numeric(sm[4])) {
            instructions.emplace_back(sm[1], std::stoi(sm[2]), std::stoi(sm[4]));
          } else if(is_numeric(sm[4])) {
            instructions.emplace_back(sm[1], sm[2], std::stoi(sm[4]));
          } else if(is_numeric(sm[2])) {
            instructions.emplace_back(sm[1], std::stoi(sm[2]), sm[4]);
          } else {
            instructions.emplace_back(sm[1], sm[2], sm[4]);
          }
        }
      } else {
        std::cout << "Unable to match line: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << try_run(instructions) << std::endl;
  return 0;
}