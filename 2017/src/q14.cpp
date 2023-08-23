#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <unordered_map>

void reverseList(size_t start, size_t end, std::vector<int>& data) {
  while(end > start) {
    std::swap(data[start % data.size()], data[end % data.size()]);
    start++;
    end--;
  }
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
    res << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << hash;
  }

  return res.str();
}

int calcSpaceFilled(const std::string data) {
  std::unordered_map<char, int> countPerLetter = {
    {'0', 0}, {'1', 1}, {'2', 1}, {'3', 2},
    {'4', 1}, {'5', 2}, {'6', 2}, {'7', 3},
    {'8', 1}, {'9', 2}, {'A', 2}, {'B', 3},
    {'C', 2}, {'D', 3}, {'E', 3}, {'F', 4}
  };
  int spaceFilled = 0;
  for(int i = 0; i < 128; i++) {
    std::string currStr = data + "-" + std::to_string(i);
    std::string res = fullHash(currStr);
    for(char c : res) {
      spaceFilled += countPerLetter.find(c)->second;
    }
  }

  return spaceFilled;
}

bool inBounds(int i, int j) {
  return i >= 0 && i < 128 && j >= 0 && j < 128;
}

void getRegion(std::vector<std::vector<bool>>& visited,
  const std::vector<std::vector<char>>& disk, const int i, const int j) {
  std::vector<std::pair<int, int>> steps = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  visited[i][j] = true;
  for(auto step : steps) {
    int nextI = i + step.first, nextJ = j + step.second;
    if(inBounds(nextI, nextJ) && !visited[nextI][nextJ] && disk[nextI][nextJ] == '1') {
      getRegion(visited, disk, nextI, nextJ);
    }
  }
}

int calcNoRegions(const std::string data) {
  std::unordered_map<char, std::string> hexToBin = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
    {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
    {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
    {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
  };
  std::vector<std::vector<char>> disk;
  for(int i = 0; i < 128; i++) {
    std::string currStr = data + "-" + std::to_string(i);
    std::string res = fullHash(currStr);
    std::vector<char> row;
    for(char c : res) {
      std::string bits = hexToBin.find(c)->second;
      for(char bit : bits) {
        row.push_back(bit);
      }
    }
    disk.push_back(row);
  }

  int regions = 0;
  std::vector<std::vector<bool>> visited(128, std::vector<bool>(128, false));
  for(int i = 0; i < 128; i++) {
    for(int j = 0; j < 128; j++) {
      if(!visited[i][j] && disk[i][j] == '1') {
        getRegion(visited, disk, i, j);
        regions++;
      }
    }
  }

  return regions;
}

int main() {
  std::ifstream ifstrm("./data/q14.txt", std::ios::in);
  std::string data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    data = line;
  }

  std::cout << "Answer to part 1: " << calcSpaceFilled(data) << std::endl;
  std::cout << "Answer to part 2: " << calcNoRegions(data) << std::endl;
  return 0;
}