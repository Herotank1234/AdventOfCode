#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <queue>
#include <unordered_set>

typedef struct Node {
  Node(int x, int y, int size, int used, int avail, int use_percentage) :
    x(x), y(y), size(size), used(used), avail(avail), use_percentage(use_percentage) 
  {
    if(used != 0) data_stored = std::to_string(x) + "," + std::to_string(y);
  }

  int x;
  int y;
  int size;
  int used;
  int avail;
  int use_percentage;
  std::string data_stored = "";
} Node;

typedef struct State {
  State(std::map<std::string, Node> storage, int steps) : storage(storage), steps(steps) {}

  std::string get_hash() {
    std::stringstream ss;
    for(auto kv : storage) {
      ss << kv.first;
      ss << ":";
      ss << kv.second.data_stored;
      ss << ",";
    }
    return ss.str();
  }

  std::map<std::string, Node> storage;
  int steps;
} State;

int count_valid_pairs(const std::vector<Node> &nodes) {
  int valid_pairs = 0;
  for(const auto &node : nodes) {
    if(node.used == 0) { continue; }
    for(const auto &other : nodes) {
      if(node.x == other.x && node.y == other.y) { continue; }
      if(node.used <= other.avail) { valid_pairs++; }
    }
  }
  return valid_pairs;
}

void print(std::map<std::string, Node> storage) {
  for(auto kv : storage) {
    std::cout << kv.second.data_stored << std::endl;
  }
}

int find_min_steps_to_move_data(const std::vector<Node> &nodes) {
  int max_x = 0;
  std::map<std::string, Node> nodes_map;
  std::string initial_empty_node;
  for(Node node : nodes) {
    max_x = std::max(max_x, node.x);
    std::string key = std::to_string(node.x) + "," + std::to_string(node.y);
    if(node.used == 0) initial_empty_node = key;
    nodes_map.insert({key, node});
  }

  // Find min steps to move empty spot next to goal
  std::queue<std::pair<std::string, int>> to_visit;
  to_visit.push({initial_empty_node, 0});
  std::unordered_set<std::string> visited;
  std::vector<std::pair<int, int>> directions = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
  };

  int min_steps_to_goal = 0;
  std::string next_to_goal = std::to_string(max_x - 1) + ",0";

  while(!to_visit.empty()) {
    std::pair<std::string, int> curr_state = to_visit.front();
    to_visit.pop();

    if(curr_state.first == next_to_goal) {
      min_steps_to_goal = curr_state.second;
      break;
    }

    if(visited.find(curr_state.first) != visited.end()) continue;
    visited.insert(curr_state.first);

    for(auto direction : directions) {
      int next_x = nodes_map.find(curr_state.first)->second.x + direction.first;
      int next_y = nodes_map.find(curr_state.first)->second.y + direction.second;
      std::string next_key = std::to_string(next_x) + "," + std::to_string(next_y);

      if(nodes_map.find(next_key) == nodes_map.end()) continue;
      if(visited.find(next_key) != visited.end()) continue;
      
      int space_needed = nodes_map.find(next_key)->second.used;
      int space_available = nodes_map.find(curr_state.first)->second.size;

      if(space_needed > space_available) continue;

      to_visit.push({next_key, curr_state.second + 1});
    }
  }

  /* 
    Shifting the goal up with the empty space takes 5 steps
      . .      . .      . .      . .      . _      _ .
      _ .  ->  G .  ->  G .  ->  G _  ->  G .  ->  G .
      G .      _ .      . _      . .      . .      . .
    Do this until the empty space is at 0,0 and the goal is right next to it 
  */

  int steps_to_move_goal = (max_x - 1) * 5;

  /*
    Add one more step to move goal to 0,0
  */

  int total_steps = min_steps_to_goal + steps_to_move_goal + 1;
  return total_steps;
}

int main() {
  std::ifstream ifstrm("./data/q22.txt", std::ios::in);
  std::vector<Node> nodes;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::getline(ifstrm, line);

    std::regex match("/dev/grid/node-x([\\d]+)-y([\\d]+)[ ]*([\\d]+)T[ ]*([\\d]+)T[ ]*"
      "([\\d]+)T[ ]*([\\d]+)%\\s?");
    std::smatch sm;

    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        nodes.emplace_back(std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3]), 
          std::stoi(sm[4]), std::stoi(sm[5]), std::stoi(sm[6]));
      } else {
        std::cout << "Was unable to match line: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << count_valid_pairs(nodes) << std::endl;
  std::cout << "Answer to part 2: " << find_min_steps_to_move_data(nodes) << std::endl;
  return 0;
}