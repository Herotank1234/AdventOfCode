#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iomanip>

void reverseList(size_t start, size_t end, std::vector<int>& data) {
  while(end > start) {
    std::swap(data[start % data.size()], data[end % data.size()]);
    start++;
    end--;
  }
}

int halfTwistHash(const std::vector<int>& lengths) {
  std::vector<int> data(256);
  std::iota(data.begin(), data.end(), 0);

  size_t index = 0, skipSize = 0;
  for(int length : lengths) {
    size_t end = index + length - 1;
    reverseList(index, end, data);
    index += length + skipSize++;
    index %= data.size();
  }

  return data[0] * data[1];
}

void oneHash(size_t& index, size_t& skipSize, std::vector<int>& data,
  const std::vector<int>& lengths) {
  for(int length : lengths) {
    size_t end = index + length - 1;
    reverseList(index, end, data);
    index += length + skipSize++;
    index %= data.size();
  }
}

std::string fullHash(const std::string& bytes) {
  std::vector<int> lengths;
  for(char c : bytes) {
    lengths.push_back(c);
  }
  std::vector<int> suffix = {17, 31, 73, 47, 23};
  lengths.insert(lengths.end(), suffix.begin(), suffix.end());

  size_t index = 0, skipSize = 0;
  std::vector<int> data(256);
  std::iota(data.begin(), data.end(), 0);
  for(int i = 0; i < 64; i++) {
    oneHash(index, skipSize, data, lengths);
  }

  std::vector<int> denseHash(16);
  for(size_t i = 0; i < denseHash.size(); i++) {
    int val = data[16 * i];
    for(size_t j = (16 * i) + 1; j < 16 * (i + 1); j++) {
      val ^= data[j];
    }
    denseHash[i] = val;
  }
  std::stringstream res;
  for(int hash : denseHash) {
    res << std::setfill('0') << std::setw(2) << std::hex << hash;
  }

  return res.str();
}

int main() {
  std::ifstream ifstrm("./data/q10.txt", std::ios::in);
  std::vector<int> data;
  std::string bytes;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      bytes = line;
      std::regex match("([\\d]+)");
      std::smatch sm;
      while(std::regex_search(line, sm, match)) {
        data.push_back(stoi(sm.str()));
        line = sm.suffix();
      }
    }
  }

  std::cout << "Answer to part 1: " << halfTwistHash(data) << std::endl;
  std::cout << "Answer to part 2: " << fullHash(bytes) << std::endl;
  return 0;
}