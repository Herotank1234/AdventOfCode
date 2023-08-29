#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>
#include <climits>

enum Tile {
  Wall,
  StillWater,
  FlowingWater
};

struct int_pair_hash {
  size_t operator()(const std::pair<int, int> &p) const {
    return p.first * 100000 + p.second;
  }
};

int fill(std::unordered_map<std::pair<int, int>, Tile, int_pair_hash>& filled, 
  const int min_y, const int max_y, int x, int y) 
{
  if(y > max_y) return 0;

  filled.insert({{x, y}, Tile::FlowingWater});

  int filled_squares = 0;
  auto search_under = filled.find({x, y + 1});

  if(search_under == filled.end()) {
    filled_squares += fill(filled, min_y, max_y, x, y + 1);
  }
  
  search_under = filled.find({x, y + 1});
  auto search_left = filled.find({x - 1, y});
  if(search_left == filled.end() && search_under != filled.end() && 
    (search_under->second == Tile::Wall || search_under->second == Tile::StillWater))
  {
    filled_squares += fill(filled, min_y, max_y, x - 1, y);
  }

  search_under = filled.find({x, y + 1});
  auto search_right = filled.find({x + 1, y});
  if(search_right == filled.end() && search_under != filled.end() && 
    (search_under->second == Tile::Wall || search_under->second == Tile::StillWater))
  {
    filled_squares += fill(filled, min_y, max_y, x + 1, y);
  }

  int curr_x = x - 1;
  auto curr_search = filled.find({curr_x, y});
  bool wall_found_left = false;
  while(curr_search != filled.end()) {
    if(curr_search->second == Tile::Wall) {
      wall_found_left = true;
      break;
    }
    curr_x--;
    curr_search = filled.find({curr_x, y});
  }

  curr_x = x + 1;
  curr_search = filled.find({curr_x, y});
  bool wall_found_right = false;
  while(curr_search != filled.end()) {
    if(curr_search->second == Tile::Wall) {
      wall_found_right = true;
      break;
    }
    curr_x++;
    curr_search = filled.find({curr_x, y});
  }

  if(wall_found_left && wall_found_right) {
    curr_x = x - 1;
    while(filled.find({curr_x, y})->second != Tile::Wall) {
      filled[{curr_x, y}] = Tile::StillWater;
      curr_x--;
    }
    curr_x = x;
    while(filled.find({curr_x, y})->second != Tile::Wall) {
      filled[{curr_x, y}] = Tile::StillWater;
      curr_x++;
    }
  }

  return (y >= min_y ? 1 : 0) + filled_squares;
}

int count_water_reachable_squares(
  std::unordered_map<std::pair<int, int>, Tile, int_pair_hash> filled) 
{
  int min_y = INT_MAX, max_y = INT_MIN;
  for(const auto &p : filled) {
    min_y = std::min(min_y, p.first.second);
    max_y = std::max(max_y, p.first.second);
  }

  return fill(filled, min_y, max_y, 500, 0);
}

int count_still_water_squares(
  std::unordered_map<std::pair<int, int>, Tile, int_pair_hash> filled)
{
  int min_y = INT_MAX, max_y = INT_MIN;
  for(const auto &p : filled) {
    min_y = std::min(min_y, p.first.second);
    max_y = std::max(max_y, p.first.second);
  }

  fill(filled, min_y, max_y, 500, 0);

  int still_water = 0;
  for(auto kv : filled) {
    if(kv.second == Tile::StillWater) still_water++;
  }

  return still_water;
}

int main() {
  std::ifstream ifstrm("./data/q17.txt", std::ios::in);
  std::unordered_map<std::pair<int, int>, Tile, int_pair_hash> filled;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(x|y)=([\\d]+), [xy]=([\\d]+)..([\\d]+)\\s?");
    std::smatch sm;

    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        int axis = std::stoi(sm[2]);
        int lower = std::stoi(sm[3]);
        int upper = std::stoi(sm[4]);

        for(int i = lower; i <= upper; i++) {
          std::pair<int, int> key;
          if(sm[1] == "x") {
            key = {axis, i};
          } else {
            key = {i, axis};
          }
          filled.insert({key, Tile::Wall});
        }
      }
    }
  }

  std::cout << "Answer to part 1: " << count_water_reachable_squares(filled) << std::endl;
  std::cout << "Answer to part 2: " << count_still_water_squares(filled) << std::endl;
  return 0;
}