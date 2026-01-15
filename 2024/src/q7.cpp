#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <utility>

bool is_solvable(uint64_t target, std::stack<uint64_t> numbers) {
  if(numbers.size() == 1) return target == numbers.top();

  uint64_t left = numbers.top();
  numbers.pop();
  uint64_t right = numbers.top();
  numbers.pop();

  uint64_t plus = left + right;
  uint64_t mul = left * right;

  numbers.push(plus);
  if(is_solvable(target, numbers)) return true;
  numbers.pop();

  numbers.push(mul);
  return is_solvable(target, numbers);
}

bool is_new_solvable(uint64_t target, std::stack<uint64_t> numbers) {
  if(numbers.size() == 1) return target == numbers.top();

  uint64_t left = numbers.top();
  numbers.pop();
  uint64_t right = numbers.top();
  numbers.pop();

  uint64_t plus = left + right;
  uint64_t mul = left * right;

  std::stringstream ss;
  ss << left << right;
  uint64_t concat = std::stoul(ss.str());

  numbers.push(plus);
  if(is_new_solvable(target, numbers)) return true;
  numbers.pop();

  numbers.push(mul);
  if(is_new_solvable(target, numbers)) return true;
  numbers.pop();

  numbers.push(concat);
  return is_new_solvable(target, numbers);
}


uint64_t total_calibration_result(std::vector<std::pair<uint64_t, std::stack<uint64_t>>> equations) {
  uint64_t calibration_result = 0;

  for(std::pair<uint64_t, std::stack<uint64_t>> equation : equations) {
    if(is_solvable(equation.first, equation.second)) calibration_result += equation.first;
  }

  return calibration_result;
}

uint64_t total_new_calibration_result(std::vector<std::pair<uint64_t, std::stack<uint64_t>>> equations) {
  uint64_t calibration_result = 0;

  for(std::pair<uint64_t, std::stack<uint64_t>> equation : equations) {
    if(is_new_solvable(equation.first, equation.second)) calibration_result += equation.first;
  }

  return calibration_result;
}

int main() {
  std::ifstream input("./data/q7.txt");

  std::vector<std::pair<uint64_t, std::stack<uint64_t>>> equations;
  std::string line;

  std::regex rgx("([\\d]+)");
  std::smatch match;

  while(std::getline(input, line)) {
    std::vector<uint64_t> values;
    while(std::regex_search(line, match, rgx)) {
      // std::cout << match[0].str() << std::endl;
      values.push_back(std::stoul(match[0].str()));
      line = match.suffix().str();
    }

    uint64_t target = values[0];
    std::stack<uint64_t> numbers;
    for(size_t i = values.size() - 1; i > 0; i--) {
      numbers.push(values[i]);
    }

    equations.push_back({target, numbers});
  }

  std::cout << "Answer to part 1: " << total_calibration_result(equations) << std::endl;
  std::cout << "Answer to part 2: " << total_new_calibration_result(equations) << std::endl;
  return 0;
}