#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

typedef struct State {
  State(std::vector<std::vector<int>> floors) : floors(floors) {}

  std::string get_hash() {
    std::stringstream ss;
    ss << elevator_floor << " [";
    for(size_t i = 0; i < floors.size(); i++) {
      std::vector<int> curr_floor = floors[i];
      std::sort(curr_floor.begin(), curr_floor.end());
      ss << "[";
      for(size_t c = 0; c < curr_floor.size(); c++) {
        if(c != curr_floor.size() - 1) {
          ss << curr_floor[c] << ",";
        } else {
          ss << curr_floor[c];
        }
      }
      if(i != floors.size() - 1) {
        ss << "],";
      } else {
        ss << "]";
      }
    }
    ss << "]";
    return ss.str();
  }

  bool is_finished() {
    for(size_t i = 0; i < floors.size() - 1; i++) {
      if(!floors[i].empty()) {
        return false;
      }
    }
    return true;
  }

  void move_component(size_t comp_index, size_t delta_floor) {
    floors[elevator_floor + delta_floor].push_back(floors[elevator_floor][comp_index]);
    floors[elevator_floor].erase(floors[elevator_floor].begin() + comp_index);
    elevator_floor += delta_floor;
    steps++;
  }

  void move_two_components(size_t comp_index1, size_t comp_index2, size_t delta_floor) {
    int comp1 = floors[elevator_floor][comp_index1], comp2 = floors[elevator_floor][comp_index2];
    floors[elevator_floor + delta_floor].push_back(comp1);
    floors[elevator_floor + delta_floor].push_back(comp2);
    floors[elevator_floor].erase(std::find(floors[elevator_floor].begin(), floors[elevator_floor].end(), comp1));
    floors[elevator_floor].erase(std::find(floors[elevator_floor].begin(), floors[elevator_floor].end(), comp2));
    elevator_floor += delta_floor;
    steps++;
  }

  bool is_valid_move(size_t delta_floor) {
    std::vector<int> curr_floor = floors[elevator_floor];
    std::vector<int> prev_floor = floors[elevator_floor + delta_floor];

    std::vector<int> curr_unpaired;
    bool curr_contains_gen = false;
    for(int i : curr_floor) {
      if(std::find(curr_floor.begin(), curr_floor.end(), i * -1) == curr_floor.end()) {
        curr_unpaired.push_back(i);
      }
      if(i > 0) { curr_contains_gen = true; }
    }
    if(!curr_unpaired.empty()) {
      for(int c : curr_unpaired) {
        if(curr_contains_gen && c < 0) return false;
      }
    }

    std::vector<int> prev_unpaired;
    bool prev_contains_gen = false;
    for(int i : prev_floor) {
      if(std::find(prev_floor.begin(), prev_floor.end(), i * -1) == prev_floor.end()) {
        prev_unpaired.push_back(i);
      }
      if(i > 0) { prev_contains_gen = true; }
    }
    if(!prev_unpaired.empty()) {
      for(int c : prev_unpaired) {
        if(prev_contains_gen && c < 0) return false;
      }
    }

    return true;
  }

  bool try_move(size_t comp_index, size_t delta_floor) {
    move_component(comp_index, delta_floor);
    return is_valid_move(delta_floor * -1);
  }

  bool try_move_two(size_t comp_index1, size_t comp_index2, size_t delta_floor) {
    int comp1 = floors[elevator_floor][comp_index1], comp2 = floors[elevator_floor][comp_index2];
    if(comp1 + comp2 != 0) {
      if((comp1 < 0 && comp2 > 0) || (comp1 > 0 && comp2 < 0)) { return false; }
    }
    move_two_components(comp_index1, comp_index2, delta_floor);
    return is_valid_move(delta_floor * -1);
  }

  std::vector<std::vector<int>> floors;
  size_t elevator_floor = 0;
  int steps = 0;
} State;

std::vector<State> get_next_valid_states(State state) {
  std::vector<State> next_states;

  if(state.elevator_floor != state.floors.size() - 1) {
    for(size_t comp1 = 0; comp1 < state.floors[state.elevator_floor].size(); comp1++) {
      auto next_state = state;
      if(next_state.try_move(comp1, 1)) {
        next_states.push_back(next_state); 
      }
      for(size_t comp2 = comp1 + 1; comp2 < state.floors[state.elevator_floor].size(); comp2++) {
        auto next_state = state;
        if(next_state.try_move_two(comp1, comp2, 1)) { 
          next_states.push_back(next_state); 
        }
      }
    }
  }

  if(state.elevator_floor != 0) {
    for(size_t comp1 = 0; comp1 < state.floors[state.elevator_floor].size(); comp1++) {
      auto next_state = state;
      if(next_state.try_move(comp1, -1)) { 
        next_states.push_back(next_state); 
      }
      for(size_t comp2 = comp1 + 1; comp2 < state.floors[state.elevator_floor].size(); comp2++) {
        auto next_state = state;
        if(next_state.try_move_two(comp1, comp2, -1)) { 
          next_states.push_back(next_state); 
        }
      }
    }
  }

  return next_states;
}

int min_steps_to_move_to_top(const State &initial_state) {
  State initial = initial_state;
  std::queue<State> to_visit;
  std::unordered_set<std::string> visited;
  to_visit.push(initial);

  while(true) {
    State curr_state = to_visit.front();
    to_visit.pop();
    std::string hash = curr_state.get_hash();
    if(visited.find(hash) != visited.end()) { continue; }
    // std::cout << curr_state.steps << " " << curr_state.get_hash() << std::endl;

    visited.insert(hash);

    if(curr_state.is_finished()) { return curr_state.steps; }

    std::vector<State> next_states = get_next_valid_states(curr_state);
    for(auto state : next_states) {
      to_visit.push(state);
    }
  }

  return -1;
}

int main() {
  std::ifstream ifstrm("./data/q11.txt", std::ios::in);
  std::vector<std::vector<int>> floors;
  int element_index = 1;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::unordered_map<std::string, int> element_map;

    while(std::getline(ifstrm, line)) {
      std::vector<int> curr_floor;
      std::string delim = "contains ";
      std::string components = line.substr(line.find(delim) + delim.size(), 
        line.size() - line.find(delim) - delim.size());
      
      if(components != "nothing relevant.") {
        std::regex match("([a-z]+)( generator|-compatible microchip)");
        std::smatch sm;
        while(std::regex_search(components, sm, match)) {
          auto search = element_map.find(sm[1].str());
          if(search == element_map.end()) {
            element_map.insert({sm[1].str(), element_index++});
          }
          curr_floor.push_back(element_map.find(sm[1].str())->second * (sm[2] == " generator" ? 1 : -1));
          
          components = sm.suffix();
        }
      }

      floors.push_back(curr_floor);
    }
  }

  State initial_state = State(floors);
  std::cout << "Answer to part 1: " << min_steps_to_move_to_top(initial_state) << std::endl;

  floors[0].push_back(element_index);
  floors[0].push_back(element_index++ * -1);
  floors[0].push_back(element_index);
  floors[0].push_back(element_index++ * -1);
  State new_inital_state = State(floors);
  std::cout << "Answer to part 2: " << min_steps_to_move_to_top(new_inital_state) << std::endl;
  return 0;
}