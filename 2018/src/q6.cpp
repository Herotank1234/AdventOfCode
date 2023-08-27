#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <climits>
#include <unordered_map>
#include <unordered_set>

int findSmallestArea(std::vector<std::pair<int, int>> data) {
  std::unordered_map<std::string, int> pointToIndex;

  int index = 0;
  for(auto pair : data) {
    std::string pairStr = std::to_string(pair.first) + "_" + std::to_string(pair.second);
    pointToIndex.insert({pairStr, index});
    index++;
  }

  int length = 1000, width = 1000;
  std::vector<std::vector<int>> field(length, std::vector<int>(width, -1));
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < width; j++) {
      int minLength = INT_MAX;
      int pointIndex = -1;
      int count = 0;
      for(auto pair : data) {
        int distance = abs(i - pair.second) + abs(j - pair.first);
        if(distance < minLength) {
          minLength = distance;
          std::string pairStr = std::to_string(pair.first) + "_" + std::to_string(pair.second);
          pointIndex = pointToIndex.find(pairStr)->second;
          count = 1;
        } else if(distance == minLength) {
          count++;
        }
      }
      if(count == 1) field[i][j] = pointIndex;
    }
  }

  std::unordered_set<int> edgeIndex;
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < width; j++) {
      if(i == 0 || j == 0 || i == length - 1 || j == width - 1) {
        edgeIndex.insert(field[i][j]);
      }
    }
  }

  std::unordered_map<int, int> areaMap;
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < width; j++) {
      if(edgeIndex.find(field[i][j]) == edgeIndex.end() && field[i][j] != -1) {
        areaMap[field[i][j]]++;
      }
    }
  }

  int biggestArea = 0;
  for(auto it = areaMap.begin(); it != areaMap.end(); it++) {
    if(it->second > biggestArea) {
      biggestArea = it->second;
    }
  }

  return biggestArea;
} 

int withinArea(std::vector<std::pair<int, int>> data) {
  int inRange = 0, maxRange = 10000, length = 2000, width = 2000;
  for(int i = -length; i < length; i++) {
    for(int j = -width; j < width; j++) {
      int distance = 0;
      for(auto pair : data) {
        distance += abs(i - pair.second) + abs(j - pair.first);
      }
      if(distance < maxRange) inRange++;
    }
  }
  return inRange;
}

int main() {
  std::ifstream ifstrm("./data/q6.txt", std::ios::in);
  std::vector<std::pair<int, int>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("([\\d]+), ([\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        data.push_back({stoi(sm[1]), stoi(sm[2])});
      }
    }
  }

  std::cout << "Answer to part 1: " << findSmallestArea(data) << std::endl;
  std::cout << "Answer to part 2: " << withinArea(data) << std::endl;
  return 0;
}