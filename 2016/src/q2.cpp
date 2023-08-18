#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

bool inBounds(size_t x, size_t y) {
  return x >= 0 && x < 3 && y >= 0 && y < 3;
}

std::string getPassword(const std::vector<std::string>& data) {
  std::vector<std::vector<int>> keypad = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  std::unordered_map<char, std::pair<int, int>> steps = {
    {'U', {0, -1}}, {'R', {1, 0}},
    {'D', {0, 1}}, {'L', {-1, 0}}
  };
  size_t x = 1, y = 1;

  std::string password = "";
  for(std::string move : data) {
    for(char c : move) {
      if(c == '\r') continue;
      std::pair<int, int> step = steps.find(c)->second;
      size_t nextX = x + step.first, nextY = y + step.second;
      if(inBounds(nextX, nextY)) {
        x = nextX;
        y = nextY;
      }
    }
    password += std::to_string(keypad[y][x]);
  }
  return password;
}

bool inBoundsDiamond(size_t x, size_t y, const std::vector<std::vector<char>>& keypad) {
  return x >= 0 && x < 5 && y >= 0 && y < 5 && keypad[y][x] != '.';
}

std::string getPasswordFromDiamondKeypad(const std::vector<std::string>& data) {
  std::vector<std::vector<char>> keypad = {
    {'.', '.', '1', '.', ','},
    {'.', '2', '3', '4', '.'},
    {'5', '6', '7', '8', '9'},
    {'.', 'A', 'B', 'C', '.'},
    {'.', '.', 'D', '.', '.'}
  };
  std::unordered_map<char, std::pair<int, int>> steps = {
    {'U', {0, -1}}, {'R', {1, 0}},
    {'D', {0, 1}}, {'L', {-1, 0}}
  };
  size_t x = 0, y = 2;

  std::string password = "";
  for(std::string move : data) {
    for(char c : move) {
      if(c == '\r') continue;
      std::pair<int, int> step = steps.find(c)->second;
      size_t nextX = x + step.first, nextY = y + step.second;
      if(inBoundsDiamond(nextX, nextY, keypad)) {
        x = nextX;
        y = nextY;
      }
    }
    password += keypad[y][x];
  }
  return password;
}

int main() {
  std::ifstream ifstrm("./data/q2.txt", std::ios::in);
  std::vector<std::string> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      data.push_back(line);
    }
  }

  std::cout << "Answer to part 1: " << getPassword(data) << std::endl;
  std::cout << "Answer to part 2: " << getPasswordFromDiamondKeypad(data) << std::endl;
  return 0;
}