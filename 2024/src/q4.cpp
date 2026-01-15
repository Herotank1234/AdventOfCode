#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef struct Direction {
  int di;
  int dj;
} Direction;

const std::vector<Direction> directions = {
  {-1, -1},
  {-1, 0},
  {-1, 1},
  {0, -1},
  {0, 1},
  {1, -1},
  {1, 0},
  {1, 1},
};

const std::vector<Direction> diags = {
  {-1, -1},
  {-1, 1},
  {1, -1},
  {1, 1},
};

const std::string word = "XMAS";

bool in_bounds(int i, int j, int height, int width) {
  return i >= 0 && i < height && j >= 0 && j < width;
}

bool search_direction(std::vector<std::vector<char>> &grid, int i, int j, int height, int width, Direction d) {
  for(char c : word) {
    if(!in_bounds(i, j, height, width)) return false;
    if(grid[i][j] != c) return false;
    i += d.di;
    j += d.dj;
  }
  return true;
}

int search_all_directions(std::vector<std::vector<char>> &grid, int i, int j, int height, int width) {
  int count = 0;
  for(Direction d : directions) {
    if(search_direction(grid, i, j, height, width, d)) {
      count++;
    }
  }
  return count;
}

bool search_diags(std::vector<std::vector<char>> &grid, int i, int j, int height, int width) {
  int m_count = 0;
  int s_count = 0;

  int m1_i;
  int m1_j;
  int m2_i;
  int m2_j;

  bool m1_found = false;

  for(Direction d : diags) {
    int curr_i = i + d.di;
    int curr_j = j + d.dj;

    if(!in_bounds(curr_i, curr_j, height, width)) return false;

    char curr_c = grid[curr_i][curr_j];
    if(curr_c != 'M' && curr_c != 'S') return false;

    if(curr_c == 'M') {
      m_count++;
      if(!m1_found) {
        m1_found = true;
        m1_i = curr_i;
        m1_j = curr_j;
      } else {
        m2_i = curr_i;
        m2_j = curr_j;
      }
    } else {
      s_count++;
    }
  }

  if(m_count != 2 || s_count != 2) return false;

  int m_dist = std::abs(m1_i - m2_i) + std::abs(m1_j - m2_j);
  if(m_dist != 2) return false;

  return true;
}


int count_xmas(std::vector<std::vector<char>> &grid) {
  int count = 0;
  int height = (int) grid.size();
  int width = (int) grid[0].size();

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(grid[i][j] == 'X') {
        count += search_all_directions(grid, i, j, height, width);
      }
    }
  }

  return count;
}

int count_cross_mas(std::vector<std::vector<char>> &grid) {
  int count = 0;
  int height = (int) grid.size();
  int width = (int) grid[0].size();

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(grid[i][j] == 'A') {
        if(search_diags(grid, i, j, height, width)) count++;
      }
    }
  }

  return count;
}

int main() {
  std::ifstream input("./data/q4.txt");
  std::string line;

  std::vector<std::vector<char>> grid;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }

  std::cout << "Answer to part 1: " << count_xmas(grid) << std::endl;
  std::cout << "Answer to part 2: " << count_cross_mas(grid) << std::endl;
  return 0;
}
