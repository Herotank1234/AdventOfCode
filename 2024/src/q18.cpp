#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#define HEIGHT 71
#define WIDTH 71

typedef struct Point {
  int i;
  int j;
} Point;

typedef struct State {
  Point p;
  int dist;
} State;

const std::vector<Point> dirs = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

std::vector<std::vector<bool>> construct_grid(std::vector<Point> &ps, int points_to_fall) {
  std::vector<std::vector<bool>> grid(HEIGHT, std::vector<bool>(WIDTH, false));
  for(int c = 0; c < points_to_fall; c++) {
    grid[ps[c].i][ps[c].j] = true;
  }
  return grid;
}

bool is_in_bounds(int i, int j) {
  return i >= 0 && i < HEIGHT && j >= 0 && j < WIDTH;
}

int find_shortest_path(std::vector<std::vector<bool>> &grid) {
  std::queue<State> to_be_visited;
  std::vector<std::vector<bool>> visited(HEIGHT, std::vector<bool>(WIDTH, false));
  to_be_visited.push({{0, 0}, 0});

  while(!to_be_visited.empty()) {
    State s = to_be_visited.front();
    to_be_visited.pop();

    if(visited[s.p.i][s.p.j]) continue;
    visited[s.p.i][s.p.j] = true;

    if(s.p.i == HEIGHT - 1 && s.p.j == WIDTH - 1) return s.dist;
    
    for(Point dir : dirs) {
      int next_i = s.p.i + dir.i;
      int next_j = s.p.j + dir.j;
      if(is_in_bounds(next_i, next_j) && !grid[next_i][next_j]) {
        to_be_visited.push({{next_i, next_j}, s.dist + 1});
      }
    }
  }
  return -1;
}

int find_shortest_path_length(std::vector<Point> &corrupted_points) {
  std::vector<std::vector<bool>> grid = construct_grid(corrupted_points, 1024);
  return find_shortest_path(grid);
}

std::string find_first_byte_to_cut_off_exit(std::vector<Point> &corrupted_points) {
  int falling_bytes = 1;
  std::vector<std::vector<bool>> grid = construct_grid(corrupted_points, falling_bytes);

  while(find_shortest_path(grid) != -1) {
    falling_bytes++;
    grid = construct_grid(corrupted_points, falling_bytes);
  }

  std::stringstream ss;
  ss << corrupted_points[falling_bytes - 1].j << "," << corrupted_points[falling_bytes - 1].i;
  return ss.str();
}

int main() {
  std::ifstream input("./data/q18.txt");
  std::vector<Point> corrupted_points;

  std::string line;
  while(std::getline(input, line)) {
    std::stringstream ss;
    ss << line;
    std::string coord;
    std::getline(ss, coord, ',');
    int j = std::stoi(coord);
    std::getline(ss, coord, ',');
    int i = std::stoi(coord);
    corrupted_points.push_back({i, j});
  }

  std::cout << "Answer to part 1: " << find_shortest_path_length(corrupted_points) << std::endl;
  std::cout << "Answer to part 2: " << find_first_byte_to_cut_off_exit(corrupted_points) << std::endl;
  return 0;
}
