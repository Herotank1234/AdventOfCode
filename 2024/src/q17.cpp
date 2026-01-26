#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class Computer {
  public:
    Computer(uint64_t a, uint64_t b, uint64_t c, std::vector<int> program) :
      _reg_a(a), _reg_b(b), _reg_c(c), _program(program) {}

    bool has_halted() {
      return _pc >= _program.size();
    }

    void print_state() {
      std::cout << "Registers: A:" << _reg_a << ", B:" << _reg_b << ", C:" << _reg_c << ", Output: " << get_output() << std::endl;
    }

    void step() {
      int opcode = _program[_pc++];
      int operand = _program[_pc++];
      switch(opcode) {
        case 0:
          {
            double numerator = _reg_a;
            double denominator = std::pow(2, combo_op(operand));
            double result = numerator / denominator;
            uint64_t result_trunc = std::trunc(result);
            _reg_a = result_trunc;
          }
          break;

        case 1:
          _reg_b ^= operand;
          break;

        case 2:
          {
            uint64_t value = combo_op(operand);
            value = value % 8;
            _reg_b = value;
          }
          break;

        case 3:
          if(_reg_a != 0) _pc = operand;
          break;

        case 4:
          _reg_b ^= _reg_c;
          break;

        case 5:
          {
            uint64_t value = combo_op(operand);
            value = value % 8;
            _output.push_back(value);
          }
          break;

        case 6:
          {
            double numerator = _reg_a;
            double denominator = std::pow(2, combo_op(operand));
            double result = numerator / denominator;
            uint64_t result_trunc = std::trunc(result);
            _reg_b = result_trunc;
          }
          break;

        case 7:
          {
            double numerator = _reg_a;
            double denominator = std::pow(2, combo_op(operand));
            double result = numerator / denominator;
            uint64_t result_trunc = std::trunc(result);
            _reg_c = result_trunc;
          }
          break;
      }
    }

    std::string get_output() {
      std::stringstream ss;
      for(size_t i = 0; i < _output.size(); i++) {
        ss << _output[i];
        if(i < _output.size() - 1) ss << ',';
      }
      return ss.str();
    }

    std::string get_program() {
      std::stringstream ss;
      for(size_t i = 0; i < _program.size(); i++) {
        ss << _program[i];
        if(i < _program.size() - 1) ss << ',';
      }
      return ss.str();
    }
    
    void set_a(uint64_t value) {
      _reg_a = value;
    }

  private:
    uint64_t combo_op(int operand) {
      switch(operand) {
        case 0:
        case 1:
        case 2:
        case 3:
          return operand;
        case 4:
          return _reg_a;
        case 5:
          return _reg_b;
        case 6:
          return _reg_c;
      }
      return -1;
    }

    uint64_t _reg_a;
    uint64_t _reg_b;
    uint64_t _reg_c;
    std::vector<int> _program;
    std::vector<int> _output = std::vector<int>();
    size_t _pc = 0;
};

std::string run_and_get_output(Computer c) {
  while(!c.has_halted()) c.step();
  return c.get_output();
}

uint64_t get_lowest_value_to_reproduce_prog(Computer c) {
  uint64_t value = 0;
  std::string program = c.get_program();
  for(int i = program.size() - 1; i >= 0; i -= 2) {
    c.set_a(value);
    std::string output = run_and_get_output(c);
    while(output != program.substr(i)) {
      value++;
      c.set_a(value);
      output = run_and_get_output(c);
    }
    if(i != 0) value *= 8;
  }
  return value;
}

int main() {
  std::ifstream input("./data/q17.txt");
  
  uint64_t reg_a = 0;
  uint64_t reg_b = 0;
  uint64_t reg_c = 0;
  std::vector<int> program;

  std::regex rgx("([\\d]+)");
  std::smatch match;

  std::string line;

  std::getline(input, line);
  std::regex_search(line, match, rgx);
  reg_a = std::stoi(match[1].str());

  std::getline(input, line);
  std::regex_search(line, match, rgx);
  reg_b = std::stoi(match[1].str());

  std::getline(input, line);
  std::regex_search(line, match, rgx);
  reg_c = std::stoi(match[1].str());

  std::getline(input, line);
  std::getline(input, line);
  while(std::regex_search(line, match, rgx)) {
    program.push_back(std::stoi(match[1].str()));
    line = match.suffix();
  }

  Computer computer(reg_a, reg_b, reg_c, program);

  std::cout << "Answer to part 1: " << run_and_get_output(computer) << std::endl;
  std::cout << "Answer to part 2: " << get_lowest_value_to_reproduce_prog(computer) << std::endl;
  return 0;
}
