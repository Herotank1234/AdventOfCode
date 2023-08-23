#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

enum Direction {
  DOWN,
  LEFT,
  UP,
  RIGHT
};

std::pair<size_t, size_t> findEntrance(const std::vector<std::vector<char>>& grid) {
  for(size_t i = 0; i < grid[0].size(); i++) {
    if(grid[0][i] == '|') return {i, 0};
  }
  return {-1, -1};
}

bool inBounds(size_t x, size_t y, size_t width, size_t length) {
  return x >= 0 && x < width && y >= 0 && y < length;
}

bool isLetter(const char& c) {
  return c >= 'A' && c <= 'Z';
}

std::string walk(const std::vector<std::vector<char>>& grid) {
  std::pair<size_t, size_t> start = findEntrance(grid);

  size_t x = start.first, y = start.second;
  Direction dir = DOWN;
  size_t width = grid[0].size(), length = grid.size();
  std::string res = "";

  while(grid[y][x] != ' ') {
    // std::cout << x << " " << y << " " << dir << std::endl;
    // See if we need to change direction or we walked on a letter
    if(grid[y][x] == '+') {
      switch(dir) {
        case DOWN:
          if(inBounds(x + 1, y, width, length) && 
            (isLetter(grid[y][x + 1]) || grid[y][x + 1] == '-')) {
            dir = RIGHT;
          } else {
            dir = LEFT;
          }
          break;
        case LEFT:
          if(inBounds(x, y + 1, width, length) && 
            (isLetter(grid[y + 1][x]) || grid[y + 1][x] == '|')) {
            dir = DOWN;
          } else {
            dir = UP;
          }
          break;
        case UP:
          if(inBounds(x + 1, y, width, length) && 
            (isLetter(grid[y][x + 1]) || grid[y][x + 1] == '-')) {
            dir = RIGHT;
          } else {
            dir = LEFT;
          }
          break;
        case RIGHT:
          if(inBounds(x, y + 1, width, length) && 
            (isLetter(grid[y + 1][x]) || grid[y + 1][x] == '|')) {
            dir = DOWN;
          } else {
            dir = UP;
          }
          break;
      }
    } else if(isLetter(grid[y][x])) {
      res += grid[y][x];
    }

    // Move to next position
    switch(dir) {
      case DOWN:
        y++;
        break;
      case RIGHT:
        x++;
        break;
      case UP:
        y--;
        break;
      case LEFT:
        x--;
        break;
    }
  }

  return res;
}

int countSteps(const std::vector<std::vector<char>>& grid) {
  std::pair<size_t, size_t> start = findEntrance(grid);

  size_t x = start.first, y = start.second;
  Direction dir = DOWN;
  size_t width = grid[0].size(), length = grid.size();
  int res = 0;

  while(grid[y][x] != ' ') {
    // See if we need to change direction or we walked on a letter
    if(grid[y][x] == '+') {
      switch(dir) {
        case DOWN:
          if(inBounds(x + 1, y, width, length) && 
            (isLetter(grid[y][x + 1]) || grid[y][x + 1] == '-')) {
            dir = RIGHT;
          } else {
            dir = LEFT;
          }
          break;
        case LEFT:
          if(inBounds(x, y + 1, width, length) && 
            (isLetter(grid[y + 1][x]) || grid[y + 1][x] == '|')) {
            dir = DOWN;
          } else {
            dir = UP;
          }
          break;
        case UP:
          if(inBounds(x + 1, y, width, length) && 
            (isLetter(grid[y][x + 1]) || grid[y][x + 1] == '-')) {
            dir = RIGHT;
          } else {
            dir = LEFT;
          }
          break;
        case RIGHT:
          if(inBounds(x, y + 1, width, length) && 
            (isLetter(grid[y + 1][x]) || grid[y + 1][x] == '|')) {
            dir = DOWN;
          } else {
            dir = UP;
          }
          break;
      }
    }

    // Move to next position
    switch(dir) {
      case DOWN:
        y++;
        break;
      case RIGHT:
        x++;
        break;
      case UP:
        y--;
        break;
      case LEFT:
        x--;
        break;
    }
    res++;
  }

  return res;
}

int main() {
  std::ifstream ifstrm("./data/q19.txt", std::ios::in);
  std::vector<std::vector<char>> grid;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<char> row;
      for(char c : line) {
        row.push_back(c);
      }
      grid.push_back(row);
    }
  }

  std::cout << "Answer to part 1: " << walk(grid) << std::endl;
  std::cout << "Answer to part 2: " << countSteps(grid) << std::endl;
  return 0;
}