#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

typedef struct Point {
  int i;
  int j;
} Point;

const std::vector<Point> directions = {
  {-1, 0},
  {0, 1},
  {0, -1},
  {1, 0}
};

std::vector<Point> get_trailheads(std::vector<std::vector<char>> &grid, int h, int w) {
  std::vector<Point> trailheads;

  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      if(grid[i][j] == '0') trailheads.push_back({i, j});
    }
  }

  return trailheads;
} 

bool is_in_bounds(int i, int j, int h, int w) {
  return i >= 0 && i < h && j >= 0 && j < w;
}

int sum_scores(std::vector<std::vector<char>> &grid) {
  int score = 0;
  int height = grid.size();
  int width = grid[0].size();
  std::vector<Point> trailheads = get_trailheads(grid, height, width);

  for(Point trailhead : trailheads) {
    std::queue<Point> to_be_visited;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    to_be_visited.push(trailhead);

    while(!to_be_visited.empty()) {
      Point curr_pos = to_be_visited.front();
      to_be_visited.pop();
      if(visited[curr_pos.i][curr_pos.j]) continue;

      visited[curr_pos.i][curr_pos.j] = true;

      char curr_level = grid[curr_pos.i][curr_pos.j];

      if(curr_level == '9') {
        score++;
        continue;
      }

      char next_level = curr_level + 1;
      for(Point direction : directions) {
        int next_i = curr_pos.i + direction.i;
        int next_j = curr_pos.j + direction.j;
        if(is_in_bounds(next_i, next_j, height, width) && grid[next_i][next_j] == next_level) {
          to_be_visited.push({next_i, next_j});
        }
      }
    }
  }

  return score;
}

int sum_ratings(std::vector<std::vector<char>> &grid) {
  int ratings = 0;
  int height = grid.size();
  int width = grid[0].size();
  std::vector<Point> trailheads = get_trailheads(grid, height, width);

  for(Point trailhead : trailheads) {
    std::queue<Point> to_be_visited;

    to_be_visited.push(trailhead);

    while(!to_be_visited.empty()) {
      Point curr_pos = to_be_visited.front();
      to_be_visited.pop();

      char curr_level = grid[curr_pos.i][curr_pos.j];

      if(curr_level == '9') {
        ratings++;
        continue;
      }

      char next_level = curr_level + 1;
      for(Point direction : directions) {
        int next_i = curr_pos.i + direction.i;
        int next_j = curr_pos.j + direction.j;
        if(is_in_bounds(next_i, next_j, height, width) && grid[next_i][next_j] == next_level) {
          to_be_visited.push({next_i, next_j});
        }
      }
    }
  }

  return ratings;
}

int main() {
  std::ifstream input("./data/q10.txt");
  
  std::vector<std::vector<char>> grid;
  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }

  std::cout << "Answer to part 1: " << sum_scores(grid) << std::endl;
  std::cout << "Answer to part 2: " << sum_ratings(grid) << std::endl;
  return 0;
}