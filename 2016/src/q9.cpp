#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

std::pair<int, int> parseBrackets(const std::string& s, size_t& index) {
  std::string length = "", repeat = "";
  index++;
  while(std::isdigit(s[index])) {
    length += s[index++];
  }
  assert(s[index] == 'x');
  index++;
  while(std::isdigit(s[index])) {
    repeat += s[index++];
  }
  index++;
  return {stoi(length), stoi(repeat)};
}

int getLength(const std::string& s) {
  int length = 0;
  size_t index = 0;
  while(index < s.length()) {
    if(s[index] >= 'A' && s[index] <= 'Z') {
      length++;
      index++;
    } else if(s[index] == '(') {
      std::pair<int, int> dim = parseBrackets(s, index);
      length += dim.first * dim.second;
      index += dim.first;
    } else {
      std::cout << "Character not recognised: " << s[index] << std::endl;
    }
  }
  return length;
}

long long parseBracketRecursive(const std::string& s, size_t& index) {
  int length = 0, repeat = 0; 
  std::string lengthS = "", repeatS = "";
  index++;
  while(std::isdigit(s[index])) {
    lengthS += s[index++];
  }
  assert(s[index] == 'x');
  index++;
  while(std::isdigit(s[index])) {
    repeatS += s[index++];
  }
  index++;
  long long totalLength = 0;
  length = stoi(lengthS);
  repeat = stoi(repeatS);
  size_t end = index + length;
  while(index < end) {
    if(s[index] >= 'A' && s[index] <= 'Z') {
      totalLength++;
      index++;
    } else {
      totalLength += parseBracketRecursive(s, index);
    }
  }
  return totalLength * repeat;
}

long long getLengthRecursive(const std::string& s) {
  long long length = 0;
  size_t index = 0;
  while(index < s.length()) {
    if(s[index] >= 'A' && s[index] <= 'Z') {
      length++;
      index++;
    } else if(s[index] == '(') {
      length += parseBracketRecursive(s, index);
    } else {
      std::cout << "Character not recognised: " << s[index] << std::endl;
    }
  }
  return length;
}


int main() {
  std::ifstream ifstrm("./data/q9.txt", std::ios::in);
  std::string s;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    s = line;
  }

  std::cout << "Answer to part 1: " << getLength(s) << std::endl;
  std::cout << "Answer to part 2: " << getLengthRecursive(s) << std::endl;
  return 0;
}