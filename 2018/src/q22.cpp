#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <climits>

enum Tool {
  Torch,
  ClimbingGear,
  None
};

typedef struct State {
  State(int time, int i, int j, Tool curr_tool) :
    time(time), i(i), j(j), curr_tool(curr_tool) {}

  inline bool operator<(const State& other) const {
    return time > other.time;
  }

  std::string get_hash() {
    return std::to_string(i) + "," + std::to_string(j) + "-" + std::to_string(curr_tool);
  }

  int time, i, j;
  Tool curr_tool;
} State;

std::vector<std::vector<int>> get_erosion_matrix(const int depth, const int x, const int y,
  const int target_x, const int target_y) 
{
  std::vector<std::vector<int>> erosion(y + 1, std::vector<int>(x + 1, 0));
  
  for(int i = 0; i < y + 1; i++) {
    for(int j = 0; j < x + 1; j++) {
      int geological = 0;
      if(i == 0) {
        geological = j * 16807;
      } else if(j == 0) {
        geological = i * 48271;
      } else if(i == target_y && j == target_x) {
        geological = 0;
      } else {
        geological = erosion[i][j - 1] * erosion[i - 1][j];
      }
      erosion[i][j] = (geological + depth) % 20183;
    }
  }

  return erosion;
}

int calc_risk(const int depth, const int x, const int y) {
  std::vector<std::vector<int>> erosion = get_erosion_matrix(depth, x, y, x, y);

  int risk = 0;
  for(auto row : erosion) {
    for(auto col : row) {
      risk += (col % 3);
    }
  }
  risk -= (erosion[0][0] % 3);
  risk -= (erosion[y][x] % 3);

  return risk;
}

inline bool in_bounds(int i, int j, int length, int width) {
  return i >= 0 && i <= length && j >= 0 && j <= width;
}

int find_shortest_time_to_end(const int depth, const int x, const int y) {
  int length = 10 * y, width = 10 * x;
  std::vector<std::vector<int>> erosion = get_erosion_matrix(depth, width, length, x, y);
  std::vector<std::vector<int>> terrain(length + 1, std::vector<int>(width + 1, 0));

  for(int i = 0; i <= length; i++) {
    for(int j = 0; j <= width; j++) {
      terrain[i][j] = erosion[i][j] % 3;
    }
  }

  std::priority_queue<State> to_visit;
  to_visit.push(State(0, 0, 0, Tool::Torch));

  const std::unordered_map<int, std::vector<Tool>> viable_tools = {
    {0, {Tool::ClimbingGear, Tool::Torch}},
    {1, {Tool::ClimbingGear, Tool::None}},
    {2, {Tool::Torch, Tool::None}}
  };

  const std::vector<std::pair<int, int>> directions = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
  };

  std::unordered_set<std::string> visited;

  int min_time = INT_MAX;

  while(!to_visit.empty()) {
    auto curr_state = to_visit.top();
    to_visit.pop();

    std::string key = curr_state.get_hash();

    if(visited.find(key) != visited.end()) continue;
    visited.insert(key);

    if(curr_state.i == y && curr_state.j == x) {
      int total_time = curr_state.time + (curr_state.curr_tool == Tool::Torch ? 0 : 7);
      min_time = std::min(min_time, total_time);
      continue;
    }

    for(auto direction : directions) {
      int next_i = curr_state.i + direction.first, next_j = curr_state.j + direction.second;
      if(in_bounds(next_i, next_j, length, width)) {
        auto curr_viable_tools = viable_tools.find(terrain[curr_state.i][curr_state.j])->second;
        auto next_viable_tools = viable_tools.find(terrain[next_i][next_j])->second;
        std::sort(curr_viable_tools.begin(), curr_viable_tools.end());
        std::sort(next_viable_tools.begin(), next_viable_tools.end());

        std::vector<Tool> viable;
        std::set_intersection(curr_viable_tools.begin(), curr_viable_tools.end(),
          next_viable_tools.begin(), next_viable_tools.end(), std::back_inserter(viable));

        for(Tool t : viable) {
          int next_time = curr_state.time + (t == curr_state.curr_tool ? 1 : 8);
          to_visit.push(State(next_time, next_i, next_j, t));
        }
      }
    }
  }

  return min_time;
}

int main() {
  std::ifstream ifstrm("./data/q22.txt", std::ios::in);
  int depth = 0, x = 0, y = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex match_depth("depth: ([\\d]+)[\\s]?");
    std::smatch sm_depth;
    if(std::regex_match(line, sm_depth, match_depth)) {
      depth = stoi(sm_depth[1]);
    }
    
    std::getline(ifstrm, line);
    std::regex match_coords("target: ([\\d]+),([\\d]+)[\\s]?");
    std::smatch sm_coords;
    if(std::regex_match(line, sm_coords, match_coords)) {
      x = stoi(sm_coords[1]);
      y = stoi(sm_coords[2]);
    }
  }

  std::cout << "Answer to part 1: " << calc_risk(depth, x, y) << std::endl;
  std::cout << "Answer to part 2: " << find_shortest_time_to_end(depth, x, y) << std::endl;
  return 0;
}