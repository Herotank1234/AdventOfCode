#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>

typedef struct Point {
  int i;
  int j;

  bool operator <(const Point &other) const {
    if(i == other.i) return j < other.j;
    return i < other.i;
  }
} Point;

std::unordered_map<char, Point> dirs = {
  {'>', {0, 1}},
  {'v', {1, 0}},
  {'<', {0, -1}},
  {'^', {-1, 0}},
};

Point get_robot_position(std::vector<std::vector<char>> &grid, int h, int w) {
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < w; j++) {
      if(grid[i][j] == '@') return {i, j};
    }
  }
  return {0, 0};
}

std::vector<std::vector<char>> scale_grid(std::vector<std::vector<char>> &grid) {
  std::vector<std::vector<char>> new_grid;
  for(std::vector<char> row : grid) {
    std::vector<char> new_row;
    for(char c : row) {
      switch(c) {
        case '#':
          new_row.push_back('#');
          new_row.push_back('#');
          break;
        case 'O':
          new_row.push_back('[');
          new_row.push_back(']');
          break;
        case '.':
          new_row.push_back('.');
          new_row.push_back('.');
          break;
        case '@':
          new_row.push_back('@');
          new_row.push_back('.');
          break;
      }
    }
    new_grid.push_back(new_row);
  }
  return new_grid;
}

int sum_gps_coords(std::vector<std::vector<char>> grid, std::string &commands) {
  int height = grid.size();
  int width = grid[0].size();
  Point robot_pos = get_robot_position(grid, height, width);
  Point curr_dir;

  for(char command : commands) {
    std::unordered_map<char, Point>::const_iterator it = dirs.find(command);
    if(it != dirs.end()) curr_dir = it->second;

    Point next_pos = robot_pos;
    next_pos.i += curr_dir.i;
    next_pos.j += curr_dir.j;

    while(grid[next_pos.i][next_pos.j] != '.' && grid[next_pos.i][next_pos.j] != '#') {
      next_pos.i += curr_dir.i;
      next_pos.j += curr_dir.j;
    }

    if(grid[next_pos.i][next_pos.j] == '#') continue;

    switch(command) {
      case '>':
        for(int j = next_pos.j; j > robot_pos.j; j--) 
          grid[next_pos.i][j] = grid[next_pos.i][j - 1];
        break;
      case 'v':
        for(int i = next_pos.i; i > robot_pos.i; i--) 
          grid[i][next_pos.j] = grid[i - 1][next_pos.j];
        break;
      case '<':
        for(int j = next_pos.j; j < robot_pos.j; j++) 
          grid[next_pos.i][j] = grid[next_pos.i][j + 1];
        break;
      case '^':
        for(int i = next_pos.i; i < robot_pos.i; i++) 
          grid[i][next_pos.j] = grid[i + 1][next_pos.j];
        break;
    }

    grid[robot_pos.i][robot_pos.j] = '.';
    robot_pos.i += curr_dir.i;
    robot_pos.j += curr_dir.j;
  }

  int gps_coords = 0;
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(grid[i][j] == 'O') gps_coords += (i * 100) + j;
    }
  }

  return gps_coords;
}

int sum_gps_coords_scaled(std::vector<std::vector<char>> grid, std::string &commands) {
  grid = scale_grid(grid);
  int height = grid.size();
  int width = grid[0].size();

  Point robot_pos = get_robot_position(grid, height, width);
  Point curr_dir;

  for(char command : commands) {
    std::unordered_map<char, Point>::const_iterator it = dirs.find(command);
    if(it != dirs.end()) curr_dir = it->second;

    std::stack<std::vector<Point>> to_be_moved;
    to_be_moved.push({robot_pos});
    bool moveable = true;

    switch(command) {
      case '>':
      case '<':
        {
          Point curr_pos = to_be_moved.top()[0];
          int next_i = curr_pos.i + curr_dir.i;
          int next_j = curr_pos.j + curr_dir.j;
          while(grid[next_i][next_j] != '.' && grid[next_i][next_j] != '#') {
            to_be_moved.push({{next_i, next_j}});
            next_i += curr_dir.i;
            next_j += curr_dir.j;
          }
          if(grid[next_i][next_j] == '#') moveable = false;
        }
        break;
      case 'v':
      case '^':
        {
          while(!to_be_moved.top().empty()) {
            std::vector<Point> curr_edge = to_be_moved.top();
            std::set<Point> next_edge;

            for(Point p : curr_edge) {
              char curr_sym = grid[p.i][p.j];
              int next_i = p.i + curr_dir.i;
              int next_j = p.j + curr_dir.j;

              if(curr_sym == '@' || curr_sym == '[') {
                if(grid[next_i][next_j - 1] == '[') {
                  next_edge.insert({next_i, next_j - 1});
                  next_edge.insert({next_i, next_j});
                }
                if(grid[next_i][next_j] == '[') {
                  next_edge.insert({next_i, next_j});
                  next_edge.insert({next_i, next_j + 1});
                } else if(grid[next_i][next_j] == '#') {
                  moveable = false;
                  break;
                }
                if(curr_sym == '[') {
                  if(grid[next_i][next_j + 1] == '[') {
                    next_edge.insert({next_i, next_j + 1});
                    next_edge.insert({next_i, next_j + 2});
                  } else if(grid[next_i][next_j + 1] == '#') {
                    moveable = false;
                    break;
                  }
                }
              }
            }

            if(!moveable) break;
            to_be_moved.push(std::vector<Point>(next_edge.begin(), next_edge.end()));
          }
        }
        break;
    }

    if(!moveable) continue;

    while(!to_be_moved.empty()) {
      std::vector<Point> move_edge = to_be_moved.top();
      to_be_moved.pop();

      for(Point p : move_edge) {
        grid[p.i + curr_dir.i][p.j + curr_dir.j] = grid[p.i][p.j];
        grid[p.i][p.j] = '.';
      }

    }
    robot_pos.i += curr_dir.i;
    robot_pos.j += curr_dir.j;
  }

  int gps_coords = 0;
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if(grid[i][j] == '[') gps_coords += (i * 100) + j;
    }
  }

  return gps_coords;
}

int main() {
  std::ifstream input("./data/q15.txt");
  std::string line;
  
  std::vector<std::vector<char>> grid;
  std::string commands;

  while(std::getline(input, line)) {
    if(line == "") break;
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::stringstream ss;
  while(std::getline(input, line)) {
    ss << line;
  }
  commands = ss.str();

  std::cout << "Answer to part 1: " << sum_gps_coords(grid, commands) << std::endl;
  std::cout << "Answer to part 2: " << sum_gps_coords_scaled(grid, commands) << std::endl;
  return 0;
}