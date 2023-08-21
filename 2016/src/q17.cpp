#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <openssl/md5.h>
#include <queue>
#include <iomanip>
#include <sstream>
#include <unordered_map>

typedef struct State {
  State(int x, int y, std::string moves) : x(x), y(y), moves(moves) {}

  int x;
  int y;
  std::string moves;
} State;

const std::vector<std::pair<int, int>> directions = {
  {0, -1},
  {0, 1},
  {-1, 0},
  {1, 0}
};

const std::unordered_map<int, std::string> directions_to_letter = {
  {0, "U"},
  {1, "D"},
  {2, "L"},
  {3, "R"}
};

inline bool in_bounds(int x, int y) {
  return x >= 0 && x <= 3 && y >= 0 && y <= 3;
}

inline bool is_open_door(char c) {
  return c >= 'b' && c <= 'f';
}


std::string find_shortest_path_to_vault(const std::string &password) {
  std::queue<State> to_visit;

  to_visit.push(State(0, 0, ""));

  while(!to_visit.empty()) {
    State curr_state = to_visit.front();
    to_visit.pop();

    if(curr_state.x == 3 && curr_state.y == 3) { return curr_state.moves; }

    std::string input = password + curr_state.moves;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char *) input.c_str(), input.size(), result);

    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    std::string doors = ss.str().substr(0, 4);

    for(size_t i = 0; i < directions.size(); i++) {
      int next_x = curr_state.x + directions[i].first;
      int next_y = curr_state.y + directions[i].second;
      if(!in_bounds(next_x, next_y)) { continue; }

      if(!is_open_door(doors[i])) { continue; }

      std::string next_move = curr_state.moves + directions_to_letter.find(i)->second;
      to_visit.push(State(next_x, next_y, next_move));
    }
  }

  return "";
}

int find_max_steps_to_vault(const std::string &password) {
  std::queue<State> to_visit;
  int max_steps = 0;

  to_visit.push(State(0, 0, ""));

  while(!to_visit.empty()) {
    State curr_state = to_visit.front();
    to_visit.pop();

    if(curr_state.x == 3 && curr_state.y == 3) { 
      max_steps = std::max(max_steps, static_cast<int>(curr_state.moves.size()));
      continue;
    }

    std::string input = password + curr_state.moves;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char *) input.c_str(), input.size(), result);

    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    std::string doors = ss.str().substr(0, 4);

    for(size_t i = 0; i < directions.size(); i++) {
      int next_x = curr_state.x + directions[i].first;
      int next_y = curr_state.y + directions[i].second;
      if(!in_bounds(next_x, next_y)) { continue; }

      if(!is_open_door(doors[i])) { continue; }

      std::string next_move = curr_state.moves + directions_to_letter.find(i)->second;
      to_visit.push(State(next_x, next_y, next_move));
    }
  }

  return max_steps;
}

int main() {
  std::ifstream ifstrm("./data/q17.txt", std::ios::in);
  std::string password;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    password = line;
  }

  std::cout << "Answer to part 1: " << find_shortest_path_to_vault(password) << std::endl;
  std::cout << "Answer to part 2: " << find_max_steps_to_vault(password) << std::endl;
  return 0;
}