#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <memory>

class Instruction {
  public:
    virtual std::string exec(std::string input) = 0;
    virtual std::string unexec(std::string input) = 0;
    virtual std::string to_string() = 0;
};

class SwapPosition : public Instruction {
  public:
    SwapPosition(int index1, int index2) : _index1(index1), _index2(index2) {}

    std::string exec(std::string input) {
      std::swap(input[_index1], input[_index2]);
      return input;
    }

    std::string unexec(std::string input) {
      std::swap(input[_index1], input[_index2]);
      return input;
    }

    std::string to_string() {
      return "swap position " + std::to_string(_index1) + 
        " with position " + std::to_string(_index2);
    }
  private:
    int _index1;
    int _index2;
};

class SwapLetter : public Instruction {
  public:
    SwapLetter(char letter1, char letter2) : _letter1(letter1), _letter2(letter2) {}

    std::string exec(std::string input) {
      size_t index1 = input.find(_letter1);
      size_t index2 = input.find(_letter2);
      std::swap(input[index1], input[index2]);
      return input;
    }

    std::string unexec(std::string input) {
      size_t index1 = input.find(_letter1);
      size_t index2 = input.find(_letter2);
      std::swap(input[index1], input[index2]);
      return input;
    }

    std::string to_string() {
      std::stringstream ss;
      ss << "swap letter " << _letter1 << " with letter " << _letter2;
      return ss.str();
    }
  private:
    char _letter1;
    char _letter2;
};

class RotateLeft : public Instruction {
  public:
    RotateLeft(int steps) : _steps(steps) {}

    std::string exec(std::string input) {
      for(int _ = 0; _ < _steps; _++) {
        char first = input.front();
        input.erase(input.begin());
        input = input + first;
      }
      return input;
    }

    std::string unexec(std::string input) {
      for(int _ = 0; _ < _steps; _++) {
        char last = input.back();
        input.pop_back();
        input = last + input;
      }
      return input;
    }

    std::string to_string() {
      return "rotate left " + std::to_string(_steps) + " steps";
    }

  private:
    int _steps;
};

class RotateRight : public Instruction {
  public:
    RotateRight(int steps) : _steps(steps) {}

    std::string exec(std::string input) {
      for(int _ = 0; _ < _steps; _++) {
        char last = input.back();
        input.pop_back();
        input = last + input;
      }
      return input;
    }

    std::string unexec(std::string input) {
      for(int _ = 0; _ < _steps; _++) {
        char first = input.front();
        input.erase(input.begin());
        input = input + first;
      }
      return input;
    }

    std::string to_string() {
      return "rotate right " + std::to_string(_steps) + " steps";
    }
  
  private:
    int _steps;
};

class RotateBased : public Instruction {
  public:
    RotateBased(char letter) : _letter(letter) {}

    std::string exec(std::string input) {
      int index = input.find(_letter);
      int reps = 1 + index + (index >= 4 ? 1 : 0);
      for(int _ = 0; _ < reps; _++) {
        char last = input.back();
        input.pop_back();
        input = last + input;
      }
      return input;
    }

    std::string unexec(std::string input) {
      std::string next = input;
      while(input != exec(next)) {
        char first = next.front();
        next.erase(next.begin());
        next = next + first;
      }
      return next;
    }

    std::string to_string() {
      return "rotate based on position of letter " + std::string(&_letter);
    }
  
  private:
    char _letter;
};

class ReversePosition : public Instruction {
  public:
    ReversePosition(int index1, int index2) : _index1(index1), _index2(index2) {}

    std::string exec(std::string input) {
      std::reverse(input.begin() + _index1, input.begin() + _index2 + 1);
      return input;
    }

    std::string unexec(std::string input) {
      std::reverse(input.begin() + _index1, input.begin() + _index2 + 1);
      return input;
    }

    std::string to_string() {
      return "reverse positions " + std::to_string(_index1) + 
        " through " + std::to_string(_index2);
    }

  private:
    int _index1;
    int _index2;
};

class MovePosition : public Instruction {
  public:
    MovePosition(int index1, int index2) : _index1(index1), _index2(index2) {}

    std::string exec(std::string input) {
      char letter = input[_index1];
      input.erase(input.begin() + _index1);
      input.insert(input.begin() + _index2, letter);
      return input;
    }

    std::string unexec(std::string input) {
      char letter = input[_index2];
      input.erase(input.begin() + _index2);
      input.insert(input.begin() + _index1, letter);
      return input;
    }

    std::string to_string() {
      return "move position " + std::to_string(_index1) + 
        " to position " + std::to_string(_index2);
    }
  private:
    int _index1;
    int _index2;
};

std::string execute_instructions(
  const std::vector<std::unique_ptr<Instruction>>& instructions,
  std::string str) 
{
  for(auto& instruction : instructions) {
    str = instruction->exec(str);
  }
  return str;
}

std::string reverse_instructions(
  const std::vector<std::unique_ptr<Instruction>>& instructions,
  std::string str) 
{
  for(int i = static_cast<int>(instructions.size() - 1); i >= 0; i--) {
    str = instructions[i]->unexec(str);
  }
  return str;
}

int main() {
  std::ifstream ifstrm("./data/q21.txt", std::ios::in);
  std::vector<std::unique_ptr<Instruction>> instructions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match_instr("(swap position|swap letter|rotate left|"
      "rotate right|rotate based|reverse position|move position)");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_search(line, sm, match_instr)) {
        if(sm[1] == "swap position") {
          std::regex match_swap_pos("swap position ([\\d]+) with position ([\\d]+)\\s?");
          if(std::regex_match(line, sm, match_swap_pos)) {
            instructions.push_back(std::move(
              std::make_unique<SwapPosition>(std::stoi(sm[1]), std::stoi(sm[2]))));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "swap letter") {
          std::regex match_swap_letter("swap letter ([a-z]) with letter ([a-z])\\s?");
          if(std::regex_match(line, sm, match_swap_letter)) {
            instructions.push_back(std::move(
              std::make_unique<SwapLetter>(sm[1].str()[0], sm[2].str()[0])));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "rotate left") {
          std::regex match_rot_l("rotate left ([\\d]+) step(|s)\\s?");
          if(std::regex_match(line, sm, match_rot_l)) {
            instructions.push_back(std::move(
              std::make_unique<RotateLeft>(std::stoi(sm[1]))));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "rotate right") {
          std::regex match_swap_pos("rotate right ([\\d]+) step(|s)\\s?");
          if(std::regex_match(line, sm, match_swap_pos)) {
            instructions.push_back(std::move(
              std::make_unique<RotateRight>(std::stoi(sm[1]))));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "rotate based") {
          std::regex match_rot_based("rotate based on position of letter ([a-z])\\s?");
          if(std::regex_match(line, sm, match_rot_based)) {
            instructions.push_back(std::move(
              std::make_unique<RotateBased>(sm[1].str()[0])));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "reverse position") {
          std::regex match_rev("reverse positions ([\\d]+) through ([\\d]+)\\s?");
          if(std::regex_match(line, sm, match_rev)) {
            instructions.push_back(std::move(
              std::make_unique<ReversePosition>(std::stoi(sm[1]), std::stoi(sm[2]))));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }

        } else if(sm[1] == "move position") {
          std::regex match_move("move position ([\\d]+) to position ([\\d]+)\\s?");
          if(std::regex_match(line, sm, match_move)) {
            instructions.push_back(std::move(
              std::make_unique<MovePosition>(std::stoi(sm[1]), std::stoi(sm[2]))));
          } else {
            std::cout << "Was unable to match line: " << line << std::endl;
          }
        }
      }
    }
  }

  std::cout << "Answer to part 1: " << execute_instructions(instructions, "abcdefgh") << std::endl;
  std::cout << "Answer to part 2: " << reverse_instructions(instructions, "fbgdceah") << std::endl;
  return 0;
}