#include <fstream>
#include <iostream>
#include <vector>
#include <string>
/*
* DOWN = 0
* RIGHT = 1
* UP = 2
* LEFT = 3
*/

void enlarge(std::vector<std::vector<char>>& fullData, const std::vector<std::vector<char>>& data) {
  for(int i = 0; i < 500; i++) {
    fullData.push_back(std::vector<char>(1025, '.'));
  }

  for(std::vector<char> row : data) {
    std::vector<char> r(500, '.');
    r.insert(r.end(), row.begin(), row.end());
    for(int i = 0; i < 500; i++) {
      r.push_back('.');
    }
    fullData.push_back(r);
  }

  for(int i = 0; i < 500; i++) {
    fullData.push_back(std::vector<char>(1025, '.'));
  }
}

int calcInfected(const std::vector<std::vector<char>>& data) {
  std::vector<std::vector<char>> fullData;
  enlarge(fullData, data);
  size_t x = 512, y = 512;
  int dir = 2;
  int infected = 0;

  for(int i = 0; i < 10000; i++) {
    if(fullData[y][x] == '#') {
      dir = (dir - 1 + 4) % 4;
      fullData[y][x] = '.';
    } else {
      dir = ((int) dir + 1) % 4;
      fullData[y][x] = '#';
      infected++;
    }
    switch(dir) {
      case 0:
        y++;
        break;
      case 1:
        x++;
        break;
      case 2:
        y--;
        break;
      case 3:
        x--;
        break;
    }
  }
  return infected;
}

int calcEvolvedInfected(const std::vector<std::vector<char>>& data) {
  std::vector<std::vector<char>> fullData;
  enlarge(fullData, data);
  size_t x = 512, y = 512;
  int dir = 2;
  int infected = 0;

  for(int i = 0; i < 10000000; i++) {
    if(fullData[y][x] == '.') {
      dir = (dir + 1) % 4;
      fullData[y][x] = 'W';
    } else if(fullData[y][x] == 'W') {
      fullData[y][x] = '#';
      infected++;
    } else if(fullData[y][x] == '#') {
      dir = (dir - 1 + 4) % 4;
      fullData[y][x] = 'F';
    } else if(fullData[y][x] == 'F') {
      dir = (dir + 2) % 4;
      fullData[y][x] = '.';
    } else {
      // std::cout << "Char not recognised: " << (int) fullData[y][x] << std::endl;
    }
    switch(dir) {
      case 0:
        y++;
        break;
      case 1:
        x++;
        break;
      case 2:
        y--;
        break;
      case 3:
        x--;
        break;
      default:
        std::cout << "Dir not recognised" << std::endl;
    }
  }
  return infected;
}

int main() {
  std::ifstream ifstrm("./data/q22.txt", std::ios::in);
  std::vector<std::vector<char>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<char> row;
      for(char c : line) {
        if(c != 13) row.push_back(c);
      }
      data.push_back(row);
    }
  }

  std::cout << "Answer to part 1: " << calcInfected(data) << std::endl;
  std::cout << "Answer to part 2: " << calcEvolvedInfected(data) << std::endl;
  return 0;
}