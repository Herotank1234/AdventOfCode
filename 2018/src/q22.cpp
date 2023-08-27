#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

int calcRisk(const int depth, const int x, const int y) {
  std::vector<std::vector<int>> erosion(y + 1, std::vector<int>(x + 1, 0));
  for(int i = 0; i < y + 1; i++) {
    for(int j = 0; j < x + 1; j++) {
      int geological = 0;
      if(i == 0) {
        geological = j * 16807;
      } else if(j == 0) {
        geological = i * 48271;
      } else {
        geological = erosion[i][j - 1] * erosion[i - 1][j];
      }
      erosion[i][j] = (geological + depth) % 20183;
    }
  }

  int risk = 0;
  for(auto row : erosion) {
    for(auto col : row) {
      risk += (col % 3);
    }
  }
  risk -= (erosion[0][0] % 3);
  risk -= (erosion[y][x] % 3);

  return risk;
}

int main() {
  std::ifstream ifstrm("./data/q22.txt", std::ios::in);
  int depth = 0, x = 0, y = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex matchDepth("depth: ([\\d]+)[\\s]?");
    std::smatch smDepth;
    if(std::regex_match(line, smDepth, matchDepth)) {
      depth = stoi(smDepth[1]);
    }
    
    std::getline(ifstrm, line);
    std::regex matchCoords("target: ([\\d]+),([\\d]+)[\\s]?");
    std::smatch smCoords;
    if(std::regex_match(line, smCoords, matchCoords)) {
      x = stoi(smCoords[1]);
      y = stoi(smCoords[2]);
    }
  }

  std::cout << depth << " " << x << " " << y << std::endl;

  std::cout << "Answer to part 1: " << calcRisk(depth, x, y) << std::endl;
  std::cout << "Answer to part 2: " << std::endl;
  return 0;
}