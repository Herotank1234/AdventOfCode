#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <stack>
#include <queue>
#include <unordered_set>

class Room : public std::enable_shared_from_this<Room> {
  public:
    Room(int i, int j) : _i(i), _j(j) {
      rooms.insert({'N', nullptr});
      rooms.insert({'E', nullptr});
      rooms.insert({'S', nullptr});
      rooms.insert({'W', nullptr});
    }

    void set_room(std::shared_ptr<Room> next_room, char direction) {
      rooms[direction] = next_room;
    }

    std::shared_ptr<Room> build(char direction) {
      auto search = rooms.find(direction);
      if(search->second != nullptr) return search->second;

      std::pair<int, int> direction_delta = directions.find(direction)->second;
      int next_i = _i + direction_delta.first, next_j = _j + direction_delta.second;
      std::shared_ptr<Room> next_room = std::make_shared<Room>(next_i, next_j);

      set_room(next_room, direction);
      next_room->set_room(shared_from_this(), opposite_direction.find(direction)->second);

      return next_room;
    }

    std::shared_ptr<Room> get_neighbour(char direction) {
      return rooms.find(direction)->second;
    }

    std::string get_hash() {
      return std::to_string(_i) + "," + std::to_string(_j);
    }

  private:
    int _i, _j;
    std::unordered_map<char, std::shared_ptr<Room>> rooms;
    const std::unordered_map<char, char> opposite_direction = {
      {'N', 'S'},
      {'E', 'W'},
      {'S', 'N'},
      {'W', 'E'}
    };
    const std::unordered_map<char, std::pair<int, int>> directions = {
      {'N', {-1, 0}},
      {'E', {0, 1}},
      {'S', {1, 0}},
      {'W', {0, -1}}
    };
};

std::shared_ptr<Room> build_rooms(const std::string& regex_routes) {
  int i = 0, j = 0;
  std::shared_ptr<Room> root = std::make_shared<Room>(i, j);
  std::shared_ptr<Room> curr_room = root;

  std::stack<std::shared_ptr<Room>> return_points;

  for(const char& c : regex_routes) {
    switch(c) {
      case '^':
      case '$':
        break;

      case 'N':
      case 'E':
      case 'S':
      case 'W':
        curr_room = curr_room->build(c);
        break;

      case '(':
        return_points.push(curr_room);
        break;
      
      case '|':
        curr_room = return_points.top();
        break;

      case ')':
        curr_room = return_points.top();
        return_points.pop();
        break;
    
      default:
        std::cout << "Character " << c << " not recognised" << std::endl;
    }
  }

  return root;
}

int find_most_doors_opened(const std::shared_ptr<Room> root) {
  int max_doors_opened = 0;
  std::vector<char> directions = {'N', 'E', 'S', 'W'};

  std::queue<std::pair<std::shared_ptr<Room>, int>> to_visit;
  std::unordered_set<std::string> visited;

  to_visit.push({root, 0});

  while(!to_visit.empty()) {
    auto state = to_visit.front();
    std::shared_ptr<Room> curr_room = state.first;
    int steps = state.second;
    to_visit.pop();

    std::string hash = curr_room->get_hash();
    if(visited.find(hash) != visited.end()) continue;
    visited.insert(hash);

    max_doors_opened = std::max(max_doors_opened, steps);

    for(char dir : directions) {
      auto next_room = curr_room->get_neighbour(dir);
      if(next_room != nullptr) to_visit.push({next_room, steps + 1});
    }
  }

  return max_doors_opened;
}

int count_rooms_further_than_distance(const std::shared_ptr<Room> root, int distance) {
  int rooms_further_than_distance = 0;
  std::vector<char> directions = {'N', 'E', 'S', 'W'};

  std::queue<std::pair<std::shared_ptr<Room>, int>> to_visit;
  std::unordered_set<std::string> visited;

  to_visit.push({root, 0});

  while(!to_visit.empty()) {
    auto state = to_visit.front();
    std::shared_ptr<Room> curr_room = state.first;
    int steps = state.second;
    to_visit.pop();

    std::string hash = curr_room->get_hash();
    if(visited.find(hash) != visited.end()) continue;
    visited.insert(hash);

    if(steps >= distance) rooms_further_than_distance++;

    for(char dir : directions) {
      auto next_room = curr_room->get_neighbour(dir);
      if(next_room != nullptr) to_visit.push({next_room, steps + 1});
    }
  }

  return rooms_further_than_distance;
}

int main() {
  std::ifstream ifstrm("./data/q20.txt", std::ios::in);
  std::string regex_routes;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    regex_routes = line;
  }

  std::shared_ptr<Room> root = build_rooms(regex_routes);

  std::cout << "Answer to part 1: " << find_most_doors_opened(root) << std::endl;
  std::cout << "Answer to part 2: " << count_rooms_further_than_distance(root, 1000) << std::endl;
  return 0;
}