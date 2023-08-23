#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int checksum(std::string data) {
  int res = 0;
  for(size_t i = 0; i < data.length() - 1; i++) {
    if(data[i] == data[i + 1]) res += (data[i] - '0');
  }

  if(data[data.length() - 1] == data[0]) res += (data[data.length() - 1] - '0');

  return res;
}

int checksumHalfway(std::string data) {
  int res = 0;
  size_t mid = data.length() / 2;
  for(size_t i = 0; i < data.length(); i++, mid++) {
    if(mid == data.length()) mid = 0;
    if(data[i] == data[mid]) res += data[i] - '0';
  }

  return res;
}

int main() {
  std::ifstream ifstrm("./data/q1.txt", std::ios::in);
  std::string data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    data = line;
  }

  std::cout << "Answer to part 1: " << checksum(data) << std::endl;
  std::cout << "Answer to part 2: " << checksumHalfway(data) << std::endl;
  return 0;
}