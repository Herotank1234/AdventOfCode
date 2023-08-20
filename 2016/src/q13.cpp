#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <bitset>

typedef struct State {
  State(int x, int y, int steps) : x(x), y(y), steps(steps) {}
  int x;
  int y;
  int steps;
} State;

int find_min_steps(int fav_num) {
  std::queue<State> to_visit;
  std::unordered_set<std::string> visited;
  std::vector<std::pair<int, int>> directions = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
  };

  to_visit.push(State(1, 1, 0));

  while(!to_visit.empty()) {
    State curr_state = to_visit.front();
    to_visit.pop();

    if(curr_state.x == 31 && curr_state.y == 39) {
      return curr_state.steps;
    }

    std::string hash = std::to_string(curr_state.x) + std::to_string(curr_state.y);
    if(visited.find(hash) != visited.end()) { continue; }
    visited.insert(hash);

    for(auto direction : directions) {
      int next_x = curr_state.x + direction.first, next_y = curr_state.y + direction.second;

      if(next_x >= 0 && next_y >= 0) {
        int wall_rep = (next_x * next_x) + (3 * next_x) + (2 * next_x * next_y) + 
          next_y + (next_y * next_y) + fav_num;
        std::bitset<64> bin_rep(wall_rep);

        if(bin_rep.count() % 2 == 0) {
          to_visit.push(State(next_x, next_y, curr_state.steps + 1));
        }
      }
    }
  }

  return -1;
}

int count_reachable_locations_in_fifty_steps(int fav_num) {
  std::queue<State> to_visit;
  std::unordered_set<std::string> visited;
  std::vector<std::pair<int, int>> directions = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
  };

  to_visit.push(State(1, 1, 0));

  while(!to_visit.empty()) {
    State curr_state = to_visit.front();
    to_visit.pop();

    if(curr_state.steps > 50) { continue; }

    std::string hash = std::to_string(curr_state.x) + std::to_string(curr_state.y);
    if(visited.find(hash) != visited.end()) { continue; }
    visited.insert(hash);

    for(auto direction : directions) {
      int next_x = curr_state.x + direction.first, next_y = curr_state.y + direction.second;

      if(next_x >= 0 && next_y >= 0) {
        int wall_rep = (next_x * next_x) + (3 * next_x) + (2 * next_x * next_y) + 
          next_y + (next_y * next_y) + fav_num;
        std::bitset<64> bin_rep(wall_rep);

        if(bin_rep.count() % 2 == 0) {
          to_visit.push(State(next_x, next_y, curr_state.steps + 1));
        }
      }
    }
  }

  return visited.size();
}

int main() {
  std::ifstream ifstrm("./data/q13.txt", std::ios::in);
  int fav_num = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    fav_num = std::stoi(line);
  }

  std::cout << "Answer to part 1: " << find_min_steps(fav_num) << std::endl;
  std::cout << "Answer to part 2: " << count_reachable_locations_in_fifty_steps(fav_num) << std::endl;
  return 0;
}