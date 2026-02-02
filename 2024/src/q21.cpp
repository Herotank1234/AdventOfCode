#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct Point {
  int i;
  int j;
} Point;

typedef struct State {
  Point pos;
  std::string path;
  std::vector<std::vector<bool>> visited;
} State;

const std::vector<std::vector<char>> numpad = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {' ', '0', 'A'},
};

const std::vector<std::vector<char>> keypad = {
  {' ', '^', 'A'},
  {'<', 'v', '>'}
};

std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>> numpad_sp;
std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>> keypad_sp;

bool is_in_bounds(int i, int j, int h, int w)
  {
    return i >= 0 && i < h && j >= 0 && j < w;
  }

std::unordered_map<char, std::vector<std::string>> get_shortest_paths_from_char(
  const std::vector<std::vector<char>> &pad, int i, int j)
{
  std::unordered_map<char, std::vector<std::string>> shortest_paths;
  int h = pad.size();
  int w = pad[0].size();
  std::vector<std::vector<size_t>> min_dists(h, std::vector<size_t>(w, 
    std::numeric_limits<size_t>::max()));

  std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
  std::queue<State> to_be_visited;
  to_be_visited.push({{i, j}, "", visited});

  while(!to_be_visited.empty()) {
    State curr_state = to_be_visited.front();
    to_be_visited.pop();
    int curr_i = curr_state.pos.i;
    int curr_j = curr_state.pos.j;

    if(pad[curr_i][curr_j] == ' ') continue;

    if(curr_state.path.size() > min_dists[curr_i][curr_j]) continue;
    min_dists[curr_i][curr_j] = curr_state.path.size();
    shortest_paths[pad[curr_i][curr_j]].push_back(curr_state.path);

    std::vector<std::vector<bool>> next_visited = curr_state.visited;
    next_visited[curr_i][curr_j] = true;

    if(is_in_bounds(curr_i - 1, curr_j, h, w) && !next_visited[curr_i - 1][curr_j]) {
      to_be_visited.push({{curr_i - 1, curr_j}, curr_state.path + '^', next_visited});
    }
    if(is_in_bounds(curr_i, curr_j + 1, h, w) && !next_visited[curr_i][curr_j + 1]) {
      to_be_visited.push({{curr_i, curr_j + 1}, curr_state.path + '>', next_visited});
    }
    if(is_in_bounds(curr_i + 1, curr_j, h, w) && !next_visited[curr_i + 1][curr_j]) {
      to_be_visited.push({{curr_i + 1, curr_j}, curr_state.path + 'v', next_visited});
    }
    if(is_in_bounds(curr_i, curr_j - 1, h, w) && !next_visited[curr_i][curr_j - 1]) {
      to_be_visited.push({{curr_i, curr_j - 1}, curr_state.path + '<', next_visited});
    }
  }
  
  return shortest_paths;
}

std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>>
  get_all_shortest_paths(const std::vector<std::vector<char>> &pad)
{
  std::unordered_map<char, std::unordered_map<char, std::vector<std::string>>> all_shortest_paths;
  for(size_t i = 0; i < pad.size(); i++) {
    for(size_t j = 0; j < pad[0].size(); j++) {
      if(pad[i][j] != ' ') {
        std::unordered_map<char, std::vector<std::string>> char_shortest_paths
          = get_shortest_paths_from_char(pad, i, j);
        all_shortest_paths.insert({pad[i][j], char_shortest_paths});
      }
    }
  }
  return all_shortest_paths;
}

std::vector<std::string> translate_paths(const std::string &code) {
  std::string new_code = 'A' + code;
  std::vector<std::string> curr_paths = {""};
  for(size_t i = 0; i < new_code.size() - 1; i++) {
    std::vector<std::string> new_paths;
    for(const std::string &mapping : numpad_sp[new_code[i]][new_code[i + 1]]) {
      for(const std::string &curr_path : curr_paths) {
        new_paths.push_back(curr_path + mapping + 'A');
      }
    }
    curr_paths = new_paths;
  }
  return curr_paths;
}

size_t calculate_shortest_human_path(std::string path, size_t depth, 
  std::unordered_map<std::string, size_t> &memo) 
{
  if(depth == 0) return path.size();

  std::string hash = path + std::to_string(depth);
  auto it = memo.find(hash);
  if(it != memo.end()) return it->second;

  std::string curr_path = 'A' + path;
  size_t min_length = 0; 

  for(size_t i = 0; i < curr_path.size() - 1; i++) {
    size_t curr_min_length = std::numeric_limits<size_t>::max();
    for(const auto &mapping : keypad_sp[curr_path[i]][curr_path[i + 1]]) {
      curr_min_length = std::min(curr_min_length, calculate_shortest_human_path(mapping + 'A', depth - 1, memo));
    }
    min_length += curr_min_length;
  }

  memo[hash] = min_length;
  return min_length;
}

size_t sum_complexities(const std::vector<std::string> &codes, size_t depth) {
  std::vector<size_t> min_lengths;
  std::unordered_map<std::string, size_t> memo;
  for(const std::string &code : codes) {
    std::vector<std::string> possible_starting_paths = translate_paths(code);
    size_t min_length = std::numeric_limits<size_t>::max();
    for(const std::string &starting_path : possible_starting_paths) {
      min_length = std::min(min_length, calculate_shortest_human_path(starting_path, depth, memo));
    }
    min_lengths.push_back(min_length);
  }

  size_t complexity = 0;
  for(size_t i = 0; i < codes.size(); i++) {
    complexity += std::stoul(codes[i].substr(0, 3)) * min_lengths[i];
  }

  return complexity;
}

int main() {
  std::ifstream input("./data/q21.txt");
  std::string line;
  std::vector<std::string> codes;

  while(std::getline(input, line)) {
    codes.push_back(line);
  }

  numpad_sp = get_all_shortest_paths(numpad);
  keypad_sp = get_all_shortest_paths(keypad);

  std::cout << "Answer to part 1: " << sum_complexities(codes, 2) << std::endl;
  std::cout << "Answer to part 2: " << sum_complexities(codes, 25) << std::endl;
  return 0;
}