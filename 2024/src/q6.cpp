#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

class Guard {
  public:
    Guard(std::pair<int, int> pos) : _dir_index(0), _pos(pos), _dir(_directions[0]) {}

    std::pair<int, int> get_pos() {
      return _pos;
    }

    std::pair<int, int> get_dir() {
      return _dir;
    }

    std::pair<int, int> get_next_pos() {
      return {_pos.first + _dir.first, _pos.second + _dir.second};
    }

    void move() {
      _pos.first = _pos.first + _dir.first;
      _pos.second = _pos.second + _dir.second;
    }

    void turn() {
      _dir_index++;
      if(_dir_index == _directions.size()) _dir_index = 0;
      _dir = _directions[_dir_index];
    }

  private:
    const std::vector<std::pair<int, int>> _directions = {
      {-1, 0},
      {0, 1},
      {1, 0},
      {0, -1}
    };
    size_t _dir_index;
    std::pair<int, int> _pos;
    std::pair<int, int> _dir;
};

std::pair<int, int> get_initial_pos(std::vector<std::vector<char>> &grid) {
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == '^') {
        grid[i][j] = '.';
        return {i, j};
      }
    }
  }
  return{-1, -1};
}

bool in_bound(std::pair<int, int> pos, int height, int width) {
  return pos.first >= 0 && pos.first < height && pos.second >= 0 && pos.second < width;
}

size_t count_spaces_visited(std::vector<std::vector<char>> &grid, Guard guard) {
  std::set<int> visited;
  int height = grid.size();
  int width = grid[0].size();

  while(true) {
    std::pair<int, int> curr_pos = guard.get_pos();
    visited.insert(curr_pos.first * width + curr_pos.second);

    std::pair<int, int> next_pos = guard.get_next_pos();
    if(!in_bound(next_pos, height, width)) break;

    if(grid[next_pos.first][next_pos.second] == '#') {
      guard.turn();
    } else {
      guard.move();
    }
  }

  return visited.size();
}

bool check_for_loop(std::vector<std::vector<char>> &grid, Guard guard, int height, int width) {
  std::set<std::string> visited;
  
  while(true) {
    std::pair<int, int> curr_pos = guard.get_pos();
    std::pair<int, int> curr_dir = guard.get_dir();

    std::stringstream ss;
    ss << curr_pos.first << "-" << curr_pos.second << "-" << curr_dir.first << "-" << curr_dir.second;
    std::string hash = ss.str();
    if(visited.find(hash) != visited.end()) break;
    visited.insert(hash);

    std::pair<int, int> next_pos = guard.get_next_pos();
    if(!in_bound(next_pos, height, width)) return false;

    if(grid[next_pos.first][next_pos.second] == '#') {
      guard.turn();
    } else {
      guard.move();
    }
  }

  return true;
}

size_t count_obstacles(std::vector<std::vector<char>> &grid, Guard guard) {
  std::set<std::pair<int, int>> visited;
  int obstacles = 0;
  int height = grid.size();
  int width = grid[0].size();

  Guard curr_guard = guard;

  while(true) {
    std::pair<int, int> curr_pos = curr_guard.get_pos();
    visited.insert(curr_pos);

    std::pair<int, int> next_pos = curr_guard.get_next_pos();
    if(!in_bound(next_pos, height, width)) break;

    if(grid[next_pos.first][next_pos.second] == '#') {
      curr_guard.turn();
    } else {
      curr_guard.move();
    }
  }
  
  // Remove initial position from obstacle candidates
  visited.erase(guard.get_pos());

  for(std::pair<int, int> candidate : visited) {
    Guard new_guard = guard;
    grid[candidate.first][candidate.second] = '#';
    if(check_for_loop(grid, new_guard, height, width)) obstacles++;
    grid[candidate.first][candidate.second] = '.';
  }

  return obstacles;
}

int main() {
  std::ifstream input("./data/q6.txt");

  std::vector<std::vector<char>> grid;

  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end() - (line[line.size() - 1] == '\r' ? 1 : 0));
    grid.push_back(row);
  }

  std::pair<int, int> initial_pos = get_initial_pos(grid);
  Guard guard(initial_pos);

  std::cout << "Answer to part 1: " << count_spaces_visited(grid, guard) << std::endl;
  std::cout << "Answer to part 2: " << count_obstacles(grid, guard) << std::endl;
  return 0;
}