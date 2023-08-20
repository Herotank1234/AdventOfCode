#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>

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

int run_and_get_reg(
  const std::vector<Instruction> &instructions,
  std::string result_reg,
  int initial_value)
{
  std::unordered_map<std::string, int> registers = {
    {"a", 0},
    {"b", 0},
    {"c", initial_value},
    {"d", 0}
  };

  size_t index = 0;
  while(index < instructions.size()) {
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

    } else {
      std::cout << "Opcode " << opcode << " not recognised" << std::endl;
    }
  }

  return registers[result_reg];
}

int main() {
  std::ifstream ifstrm("./data/q12.txt", std::ios::in);
  std::vector<Instruction> instructions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(cpy|inc|dec|jnz) ([a-z]|-?[\\d]+)(:? ([a-z]|-?[\\d]+))?\\s?");
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

  std::cout << "Answer to part 1: " << run_and_get_reg(instructions, "a", 0) << std::endl;
  std::cout << "Answer to part 2: " << run_and_get_reg(instructions, "a", 1) << std::endl;
  return 0;
}