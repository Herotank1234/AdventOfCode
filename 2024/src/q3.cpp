#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

typedef struct Mul {
  int left;
  int right;
} Mul;

std::optional<Mul> parse_mul(std::string &instruction, size_t index) {
  assert(instruction[index++] == 'm');
  if(instruction[index++] != 'u') return {};
  if(instruction[index++] != 'l') return {};
  if(instruction[index++] != '(') return {};

  std::string left_str;
  char curr_letter = instruction[index];

  while(curr_letter >= '0' && curr_letter <= '9') {
    left_str += curr_letter;
    curr_letter = instruction[++index];
  }

  if(left_str.size() == 0) return {};
  int left = std::stoi(left_str);

  if(instruction[index++] != ',') return {};

  std::string right_str;
  curr_letter = instruction[index];

  while(curr_letter >= '0' && curr_letter <= '9') {
    right_str += curr_letter;
    curr_letter = instruction[++index];
  }

  if(right_str.size() == 0) return {};
  int right = std::stoi(right_str);

  if(instruction[index++] != ')') return {};
  
  return Mul{left, right};
}

std::optional<bool> parse_condition(std::string &instruction, size_t index) {
  assert(instruction[index++] == 'd');
  if(instruction[index++] != 'o') return {};

  if(instruction[index] == '(') {
    index++;
    if(instruction[index++] != ')') return {};

    return true;
  } else if(instruction[index] == 'n') {
    index++;
    if(instruction[index++] != '\'') return {};
    if(instruction[index++] != 't') return {};
    if(instruction[index++] != '(') return {};
    if(instruction[index++] != ')') return {};

    return false;
  }

  return {};
}

std::vector<Mul> parse_instructions(std::vector<std::string> &instructions) {
  std::vector<Mul> mul_instructions;

  for(std::string instruction : instructions) {
    for(size_t i = 0; i < instruction.size(); i++) {
      if(instruction[i] == 'm') {
        std::optional<Mul> parsed = parse_mul(instruction, i);
        if(parsed) {
          mul_instructions.push_back(*parsed);
        }
      }
    }
  }

  return mul_instructions;
}

std::vector<Mul> parse_instructions_with_conditional(std::vector<std::string> &instructions) {
  std::vector<Mul> mul_instructions;
  bool condition = true;

  for(std::string instruction : instructions) {
    for(size_t i = 0; i < instruction.size(); i++) {
      if(instruction[i] == 'm') {
        std::optional<Mul> parsed = parse_mul(instruction, i);
        if(parsed && condition) {
          mul_instructions.push_back(*parsed);
        }
      } else if(instruction[i] == 'd') {
        std::optional<bool> parsed_condition = parse_condition(instruction, i);
        if(parsed_condition) {
          condition = *parsed_condition;
        }
      }
    }
  }

  return mul_instructions;
}


int sum_muls(std::vector<std::string> &instructions) {
  std::vector<Mul> mul_instructions = parse_instructions(instructions);

  int sum = 0;
  for(Mul m : mul_instructions) {
    sum += m.left * m.right;
  }

  return sum;
}

int sum_muls_with_conditional(std::vector<std::string> &instructions) {
  std::vector<Mul> mul_instructions = parse_instructions_with_conditional(instructions);

  int sum = 0;
  for(Mul m : mul_instructions) {
    sum += m.left * m.right;
  }

  return sum;
}

int main() {
  std::ifstream input("./data/q3.txt");

  std::vector<std::string> instructions;
  std::string line;

  while(std::getline(input, line)) {
    instructions.push_back(line);
  }

  std::cout << "Answer to part 1: " << sum_muls(instructions) << std::endl;
  std::cout << "Answer to part 2: " << sum_muls_with_conditional(instructions) << std::endl;
  return 0;
}