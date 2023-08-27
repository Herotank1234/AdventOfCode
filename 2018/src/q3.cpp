#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

int overlap(std::vector<std::vector<int>> data) {
  std::vector<std::vector<int>> fabric(1000, std::vector<int>(1000, 0));
  for(std::vector<int> coords : data) {
    int left = coords[0], top = coords[1], width = coords[2], length = coords[3];
    for(int i = top; i < top + length; i++) {
      for(int j = left; j < left + width; j++) {
        fabric[i][j]++;
      }
    }
  }
  int overlaps = 0;
  for(std::vector<int> row : fabric) {
    for(int field : row) {
      if(field >= 2) overlaps++;
    }
  }
  
  return overlaps;
}

int soloClaim(std::vector<std::vector<int>> data) {
  std::vector<std::vector<int>> fabric(1000, std::vector<int>(1000, 0));
  for(std::vector<int> coords : data) {
    int left = coords[0], top = coords[1], width = coords[2], length = coords[3];
    for(int i = top; i < top + length; i++) {
      for(int j = left; j < left + width; j++) {
        fabric[i][j]++;
      }
    }
  }

  int claim = 1;
  for(std::vector<int> coords : data) {
    int left = coords[0], top = coords[1], width = coords[2], length = coords[3];
    bool found = true;
    for(int i = top; i < top + length; i++) {
      for(int j = left; j < left + width; j++) {
        if(fabric[i][j] >= 2) {
          found = false;
          break; 
        }
      }
      if(!found) break;
    }
    if(found) {
      return claim;
    } else {
      claim++;
    }
  }
  return -1;
}

int main() {
  std::ifstream ifstrm("./data/q3.txt", std::ios::in);
  std::vector<std::vector<int>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<int> coords;
      std::regex match("#[\\d]+ @ ([\\d]+),([\\d]+): ([\\d]+)x([\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        for(int i = 1; i <= 4; i++) {
          coords.push_back(stoi(sm[i]));
        }
      } 
      data.push_back(coords);
    }
  }

  std::cout << "Answer to part 1: " << overlap(data) << std::endl;
  std::cout << "Answer to part 2: " << soloClaim(data) << std::endl;
  return 0;
}