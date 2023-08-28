#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int main() {
  std::ifstream ifstrm("./data/q1.txt", std::ios::in);
  std::vector<int> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      continue;
    }
  }

  std::cout << "Answer to part 1: " << std::endl;
  std::cout << "Answer to part 2: " << std::endl;
  return 0;
}