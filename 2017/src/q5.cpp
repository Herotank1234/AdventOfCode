#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int run(std::vector<int> data) {
  int steps = 0;
  size_t index = 0;
  while(index < data.size()) {
    int curr = data[index];
    data[index]++;
    index += curr;
    steps++;
  }
  return steps;
}

int runAdvanced(std::vector<int> data) {
  int steps = 0;
  size_t index = 0;
  while(index < data.size()) {
    int curr = data[index];
    if(curr >= 3) {
      data[index]--;
    } else {
      data[index]++;
    }
    index += curr;
    steps++;
  }
  return steps;
}

int main() {
  std::ifstream ifstrm("./data/q5.txt", std::ios::in);
  std::vector<int> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      data.push_back(stoi(line));
    }
  }

  std::cout << "Answer to part 1: " << run(data) << std::endl;
  std::cout << "Answer to part 2: " << runAdvanced(data) << std::endl;
  return 0;
}