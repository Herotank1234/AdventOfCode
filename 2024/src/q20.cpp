#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct Point {
  int i;
  int j;
} Point;

const std::vector<Point> directions = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

void set_endpoints(std::vector<std::vector<char>> &grid, Point &start, Point &end) {
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == 'S') {
        grid[i][j] = '.';
        start.i = i;
        start.j = j;
      }
      if(grid[i][j] == 'E') {
        grid[i][j] = '.';
        end.i = i;
        end.j = j;
      }
    }
  }
}

std::vector<Point> get_path(std::vector<std::vector<char>> &grid, Point &start, Point &end) {
  std::vector<Point> path;
  Point curr_pos = start;
  std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));

  while(curr_pos.i != end.i || curr_pos.j != end.j) {
    path.push_back(curr_pos);
    visited[curr_pos.i][curr_pos.j] = true;

    for(const Point &d : directions) {
      int next_i = curr_pos.i + d.i;
      int next_j = curr_pos.j + d.j;
      if(!visited[next_i][next_j] && grid[next_i][next_j] == '.') {
        curr_pos.i = next_i;
        curr_pos.j = next_j;
        break;
      }
    }
  }
  path.push_back(curr_pos);

  return path;
}

int count_shortcuts(const std::vector<Point> &path) {
  std::unordered_map<int, int> cheat_frequency;

  for(size_t i = 0; i < path.size() - 3; i++) {
    for(size_t j = i + 3; j < path.size(); j++) {
      const Point &a = path[i];
      const Point &b = path[j];
      if((a.i == b.i && std::abs(a.j - b.j) == 2) || (a.j == b.j && std::abs(a.i - b.i) == 2)) {
        cheat_frequency[j - i - 2]++;
      }
    }
  }

  int cheats_more_than_hundred = 0;
  for(const std::pair<const int, int> &kv : cheat_frequency) {
    if(kv.first >= 100) cheats_more_than_hundred += kv.second;
  }
  return cheats_more_than_hundred;
}

int count_longer_shortcuts(const std::vector<Point> &path) {
  std::unordered_map<int, int> cheat_frequency;

  for(size_t i = 0; i < path.size() - 3; i++) {
    for(size_t j = i + 3; j < path.size(); j++) {
      const Point &a = path[i];
      const Point &b = path[j];
      size_t distance = std::abs(a.i - b.i) + std::abs(a.j - b.j);
      if(distance <= 20 && distance < j - i) {
        cheat_frequency[j - i - distance]++;
      }
    }
  }

  int cheats_more_than_hundred = 0;
  for(const std::pair<const int, int> &kv : cheat_frequency) {
    if(kv.first >= 100) cheats_more_than_hundred += kv.second;
  }
  return cheats_more_than_hundred;
}

int main() {
  std::ifstream input("./data/q20.txt");
  std::vector<std::vector<char>> grid;

  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }

  Point start;
  Point end;
  set_endpoints(grid, start, end);
  std::vector<Point> path = get_path(grid, start, end);

  std::cout << "Answer to part 1: " << count_shortcuts(path) << std::endl;
  std::cout << "Answer to part 2: " << count_longer_shortcuts(path) <<std::endl;
  return 0;
}