#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int calcScore(const std::string& str) {
  int score = 0;
  int depth = 1;
  bool inGarbage = false;
  for(size_t i = 0; i < str.length(); i++) {
    if(str[i] == '{' && !inGarbage) {
      score += depth;
      depth++;
    } else if(str[i] == '}' && !inGarbage) {
      depth--;
    } else if(str[i] == '<' && !inGarbage) {
      inGarbage = true;
    } else if(str[i] == '>' && inGarbage) {
      inGarbage = false;
    } else if(str[i] == '!') {
      i++;
    }
  }
  return score;
}

int countGarbageChars(const std::string& str) {
  int count = 0;
  bool inGarbage = false;

  for(size_t i = 0; i < str.length(); i++) {
    if(str[i] == '<' && !inGarbage) {
      inGarbage = true;
    } else if(str[i] == '>' && inGarbage) {
      inGarbage = false;
    } else if(str[i] == '!') {
      i++;
    } else if(inGarbage) {
      count++;
    }
  }

  return count;
}

int main() {
  std::ifstream ifstrm("./data/q9.txt", std::ios::in);
  std::string data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    data = line;
  }

  std::cout << "Answer to part 1: " << calcScore(data) << std::endl;
  std::cout << "Answer to part 2: " << countGarbageChars(data) << std::endl;
  return 0;
}