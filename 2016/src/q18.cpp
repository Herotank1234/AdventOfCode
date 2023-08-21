#include <fstream>
#include <iostream>
#include <vector>
#include <string>

char get_tile(const std::vector<char> &curr_row, size_t index) {
  if(curr_row[index - 1] == '^' && curr_row[index] == '^' && curr_row[index + 1] == '.')
    return '^';
  if(curr_row[index - 1] == '.' && curr_row[index] == '^' && curr_row[index + 1] == '^')
    return '^';
  if(curr_row[index - 1] == '^' && curr_row[index] == '.' && curr_row[index + 1] == '.')
    return '^';
  if(curr_row[index - 1] == '.' && curr_row[index] == '.' && curr_row[index + 1] == '^')
    return '^';
  return '.';
}

std::vector<char> get_next_row(const std::vector<char> &curr_row) {
  std::vector<char> next_row;
  next_row.push_back('.');
  for(size_t i = 1; i < curr_row.size() - 1; i++) {
    next_row.push_back(get_tile(curr_row, i));
  }
  next_row.push_back('.');
  return next_row;
}

int count_safe_tiles(const std::vector<std::vector<char>> &room) {
  int safe_tiles = 0;
  for(auto &row : room) {
    for(auto &col : row) {
      if(col == '.') { safe_tiles++; }
    }
  }
  return safe_tiles - (static_cast<int>(room.size()) * 2);
}

int extend_and_count_safe_tiles(std::string initial_row, int total_rows) {
  std::vector<std::vector<char>> room;

  std::vector<char> padded_initial_row;
  padded_initial_row.push_back('.');
  for(char &c : initial_row) {
    padded_initial_row.push_back(c);
  }
  padded_initial_row.push_back('.');
  room.push_back(padded_initial_row);

  for(int i = 0; i < total_rows - 1; i++) {
    room.push_back(get_next_row(room[i]));
  }

  return count_safe_tiles(room);
}

int main() {
  std::ifstream ifstrm("./data/q18.txt", std::ios::in);
  std::string initial_row;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    initial_row = line;
  }

  std::cout << "Answer to part 1: " << extend_and_count_safe_tiles(initial_row, 40) << std::endl;
  std::cout << "Answer to part 2: " << extend_and_count_safe_tiles(initial_row, 400000) << std::endl;
  return 0;
}