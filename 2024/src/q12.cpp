#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

typedef struct Point {
  int i;
  int j;
} Point;

typedef struct Edges {
  bool up;
  bool right;
  bool down;
  bool left;
} Edges;

const std::vector<Point> dirs = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

bool is_in_bounds(int i, int j, int h, int w) {
  return i >= 0 && i < h && j >= 0 && j < w;
}

int get_island_price(std::vector<std::vector<char>> &grid, int i, int j, int h, int w, 
  std::vector<std::vector<bool>> &visited) 
{
  std::queue<Point> to_be_visited;
  to_be_visited.push({i, j});
  int area = 0;
  int perimeter = 0;
  char flower_type = grid[i][j];

  while(!to_be_visited.empty()) {
    Point pos = to_be_visited.front();
    to_be_visited.pop();

    if(visited[pos.i][pos.j]) continue;
 
    visited[pos.i][pos.j] = true;
    area++;

    for(Point dir : dirs) {
      int next_i = pos.i + dir.i;
      int next_j = pos.j + dir.j;
      if(!is_in_bounds(next_i, next_j, h, w)) {
        perimeter++;
        continue;
      }

      if(grid[next_i][next_j] == flower_type) {
        to_be_visited.push({next_i, next_j});
      } else {
        perimeter++;
      }
    }
  }

  return area * perimeter;
}

int get_island_price_with_edge(std::vector<std::vector<char>> &grid, 
  int i, int j, int h, int w, 
  std::vector<std::vector<bool>> &visited,
  std::vector<std::vector<Edges>> &visited_edges) 
{
  std::queue<Point> to_be_visited;
  to_be_visited.push({i, j});
  int area = 0;
  int sides = 0;
  char flower_type = grid[i][j];
  std::vector<Point> island_poss;

  while(!to_be_visited.empty()) {
    Point pos = to_be_visited.front();
    to_be_visited.pop();

    if(visited[pos.i][pos.j]) continue;
 
    visited[pos.i][pos.j] = true;
    island_poss.push_back(pos);
    area++;

    for(Point dir : dirs) {
      int next_i = pos.i + dir.i;
      int next_j = pos.j + dir.j;
      if(!is_in_bounds(next_i, next_j, h, w)) continue;

      if(grid[next_i][next_j] == flower_type) to_be_visited.push({next_i, next_j});
    }
  }

  std::sort(island_poss.begin(), island_poss.end(), [](const Point &a, const Point &b)
  {
    if(a.i == b.i) return a.j < b.j;
    return a.i < b.i;
  });

  for(Point p : island_poss) {
    int next_i;
    int next_j;

    if(!visited_edges[p.i][p.j].up) {
      next_i = p.i;
      next_j = p.j;
      while(is_in_bounds(next_i, next_j, h, w) && grid[next_i][next_j] == flower_type && (
        !is_in_bounds(next_i - 1, next_j, h, w) || 
        grid[next_i][next_j] != grid[next_i - 1][next_j]))
      {
        visited_edges[next_i][next_j].up = true;
        next_j++;
      }
      if(visited_edges[p.i][p.j].up) {
        sides++;
      }
    }

    if(!visited_edges[p.i][p.j].right) {
      next_i = p.i;
      next_j = p.j;
      while(is_in_bounds(next_i, next_j, h, w) && grid[next_i][next_j] == flower_type && (
        !is_in_bounds(next_i, next_j + 1, h, w) || 
        grid[next_i][next_j] != grid[next_i][next_j + 1]))
      {
        visited_edges[next_i][next_j].right = true;
        next_i++;
      }
      if(visited_edges[p.i][p.j].right) {
        sides++;
      }
    }

    if(!visited_edges[p.i][p.j].down) {
      next_i = p.i;
      next_j = p.j;
      while(is_in_bounds(next_i, next_j, h, w) && grid[next_i][next_j] == flower_type && (
        !is_in_bounds(next_i + 1, next_j, h, w) || 
        grid[next_i][next_j] != grid[next_i + 1][next_j]))
      {
        visited_edges[next_i][next_j].down = true;
        next_j++;
      }
      if(visited_edges[p.i][p.j].down) {
        sides++;
      }
    }

    if(!visited_edges[p.i][p.j].left) {
      next_i = p.i;
      next_j = p.j;
      while(is_in_bounds(next_i, next_j, h, w) && grid[next_i][next_j] == flower_type && (
        !is_in_bounds(next_i, next_j - 1, h, w) || 
        grid[next_i][next_j] != grid[next_i][next_j - 1]))
      {
        visited_edges[next_i][next_j].left = true;
        next_i++;
      }
      if(visited_edges[p.i][p.j].left) {
        sides++;
      }
    }
  }
  
  return area * sides;
}

int get_total_price(std::vector<std::vector<char>> &grid) {
  int height = grid.size();
  int width = grid[0].size();
  std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

  int total_price = 0;

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(visited[i][j]) continue;
      total_price += get_island_price(grid, i, j, height, width, visited);
    }
  }

  return total_price;
}

int get_total_price_with_side(std::vector<std::vector<char>> &grid) {
  int height = grid.size();
  int width = grid[0].size();
  std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
  std::vector<std::vector<Edges>> visited_edges(height, 
    std::vector<Edges>(width, {false, false, false, false}));

  int total_price = 0;

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(visited[i][j]) continue;
      total_price += get_island_price_with_edge(grid, i, j, height, width, 
        visited, visited_edges);
    }
  }

  return total_price;
}

int main() {
  std::ifstream input("./data/q12.txt");

  std::vector<std::vector<char>> grid;

  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }

  std::cout << "Answer to part 1: " << get_total_price(grid) << std::endl;
  std::cout << "Answer to part 2: " << get_total_price_with_side(grid) <<std::endl;
  return 0;
}