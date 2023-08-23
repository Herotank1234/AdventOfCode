#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <climits>

int checksum(const std::vector<std::vector<int>> data) {
  int res = 0;
  for(std::vector<int> row : data) {
    int max = INT_MIN, min = INT_MAX;
    for(int i : row) {
      if(i > max) max = i;
      if(i < min) min = i;
    }
    res += max - min;
  }
  return res;
}

int evenDiv(const std::vector<std::vector<int>> data) {
  int res = 0;
  for(auto row : data) {
    for(size_t i = 0; i < row.size(); i++) {
      for(size_t j = 0; j < row.size(); j++) {
        if(i != j && row[i] >= row[j] && (row[i] % row[j] == 0)) {
          res += row[i] / row[j];
        }
      }
    }
  }
  return res;
}

int main() {
  std::ifstream ifstrm("./data/q2.txt", std::ios::in);
  std::vector<std::vector<int>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<int> row;
      std::regex match("([\\d]+)");
      std::smatch sm;
      while(std::regex_search(line, sm, match)) {
        row.push_back(stoi(sm.str()));
        line = sm.suffix();
      }
      data.push_back(row);
    }
  }

  std::cout << "Answer to part 1: " << checksum(data) << std::endl;
  std::cout << "Answer to part 2: " << evenDiv(data) << std::endl;
  return 0;
}