#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int dist(const int end) {
  int up = 0, left = 0, down = 0, right = 0;
  int curr = 1;
  int x = 0, y = 0, dir = 0;
  while(curr != end) {
    switch(dir) {
      case 0:
        right++;
        while(curr != end && x != right) {
          x++;
          curr++;
        }
        dir++;
        break;
      case 1:
        up++;
        while(curr != end && y != up) {
          y++;
          curr++;
        }
        dir++;
        break;
      case 2:
        left--;
        while(curr != end && x != left) {
          x--;
          curr++;
        }
        dir++;
        break;
      case 3:
        down--;
        while(curr != end && y != down) {
          y--;
          curr++;
        }
        dir = 0;
        break;
    }
  }

  return abs(x) + abs(y);
}

int sumSquare(const std::vector<std::vector<int>> &data, const int x, const int y) {
  int res = 0;
  for(int i = -1; i < 2; i++) {
    for(int j = -1; j < 2; j++) {
      res += data[y + j][x + i];
    }
  }
  return res;
}

int firstValLarger(const int end) {
  std::vector<std::vector<int>> data(1000, std::vector<int>(1000, 0));
  int x = 500, y = 500;
  int hor = 0, ver = 0, dir = 0;
  int res = 0;
  bool found = false;
  data[y][x] = 1;
  while(!found) {
    switch(dir) {
      case 0:
        hor++;
        for(int i = 0; i < hor; i++) {
          x++;
          int result = sumSquare(data, x, y);
          if(result > end) {
            res = result;
            found = true;
            break;
          }
          data[y][x] = result;
        }
        dir++;
        break;
      case 1:
        ver++;
        for(int i = 0; i < ver; i++) {
          y++;
          int result = sumSquare(data, x, y);
          if(result > end) {
            res = result;
            found = true;
            break;
          }
          data[y][x] = result;
        }
        dir++;
        break;
      case 2:
        hor++;
        for(int i = 0; i < hor; i++) {
          x--;
          int result = sumSquare(data, x, y);
          if(result > end) {
            res = result;
            found = true;
            break;
          }
          data[y][x] = result;
        }
        dir++;
        break;
      case 3:
        ver++;
        for(int i = 0; i < hor; i++) {
          y--;
          int result = sumSquare(data, x, y);
          if(result > end) {
            res = result;
            found = true;
            break;
          }
          data[y][x] = result;
        }
        dir = 0;
        break;
    }
  }
  return res;
}

int main() {
  std::ifstream ifstrm("./data/q3.txt", std::ios::in);
  int data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    data = stoi(line);
  }

  std::cout << "Answer to part 1: " << dist(data) << std::endl;
  std::cout << "Answer to part 2: " << firstValLarger(data) << std::endl;
  return 0;
}