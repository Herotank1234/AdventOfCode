#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::string reverse_and_flip(const std::string &state) {
  std::stringstream ss;
  for(int i = static_cast<int>(state.size() - 1); i >= 0; i--) {
    ss << (state[i] == '1' ? '0' : '1');
  }
  return ss.str();
}

std::string fill(const std::string &state) {
  return state + "0" + reverse_and_flip(state);
}

std::string calculate_checksum(const std::string& state) {
  std::string checksum = state;

  while(checksum.size() % 2 == 0) {
    std::stringstream next_check;
    for(size_t i = 0; i < checksum.size(); i+=2) {
      next_check << (checksum[i] == checksum[i + 1] ? '1' : '0');
    }
    checksum = next_check.str();
  }

  return checksum;
}

std::string get_checksum(const std::string& state, size_t disk_size) {
  std::string curr_state = state;
  while(curr_state.size() < disk_size) {
    curr_state = fill(curr_state);
  }
  return calculate_checksum(curr_state.substr(0, disk_size));
}

int main() {
  std::ifstream ifstrm("./data/q16.txt", std::ios::in);
  std::string initial_state;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    initial_state = line;
  }

  std::cout << "Answer to part 1: " << get_checksum(initial_state, 272) << std::endl;
  std::cout << "Answer to part 2: " << get_checksum(initial_state, 35651584) << std::endl;
  return 0;
}