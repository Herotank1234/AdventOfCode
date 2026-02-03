#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef struct Item {
  std::vector<size_t> heights;
} Item;

Item translate_grid(const std::vector<std::vector<char>> &grid, const bool &is_lock) {
  std::vector<size_t> heights;

  switch(is_lock) {
    case true:
      for(int j = 0; j < 5; j++) {
        size_t curr_height = 0;
        while(curr_height < 7 && grid[curr_height][j] == '#') curr_height++;
        heights.push_back(curr_height - 1);
      }
      break;
    case false:
      for(int j = 0; j < 5; j++) {
        size_t curr_height = 6;
        while(curr_height >= 0 && grid[curr_height][j] == '#') curr_height--;
        heights.push_back(5 - curr_height);
      }
      break;
  }

  return {heights};
}

bool is_key_fits_lock(const Item &lock, const Item &key) {
  for(size_t i = 0; i < lock.heights.size(); i++) {
    if(lock.heights[i] + key.heights[i] > 5) return false;
  }
  return true;
}

size_t count_lock_key_combos(const std::vector<Item> &locks, const std::vector<Item> &keys) {
  size_t combos = 0;
  for(const Item &lock : locks) {
    for(const Item &key : keys) {
      combos += is_key_fits_lock(lock, key);
    }
  }
  return combos;
}

int main() {
  std::ifstream input("./data/q25.txt");
  std::string line;
  std::vector<Item> locks;
  std::vector<Item> keys;

  do {
    std::vector<std::vector<char>> grid;
    for(int _ = 0; _ < 7; _++) {
      std::getline(input, line);
      std::vector<char> row(line.begin(), line.end());
      grid.push_back(row);
    }

    if(grid[0][0] == '#') {
      locks.push_back(translate_grid(grid, true));
    } else {
      keys.push_back(translate_grid(grid, false));
    }
  } while(std::getline(input, line));

  std::cout << "Answer to part 1: " << count_lock_key_combos(locks, keys) << std::endl;
  return 0;
}