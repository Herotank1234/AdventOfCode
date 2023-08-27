#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::string getScore(const int index) {
  std::vector<int> scores = {3, 7};
  int elf1Index = 0, elf2Index = 1;

  while(((int) scores.size()) < index + 10) {
    int elf1Score = scores[elf1Index];
    int elf2Score = scores[elf2Index];
    std::string nextScore = std::to_string(elf1Score + elf2Score);
    for(char c : nextScore) {
      scores.push_back(c - '0');
    }
    elf1Index = (elf1Index + elf1Score + 1) % scores.size();
    elf2Index = (elf2Index + elf2Score + 1) % scores.size();
  }

  std::string res = "";
  for(int i = index; i < index + 10; i++) {
    res += std::to_string(scores[i]);
  }

  return res;
}

int getNoRecipesToLeft(const int index) {
  std::vector<int> scores = {3, 7};
  int elf1Index = 0, elf2Index = 1;
  std::string currEnd = "____37", strIndex = std::to_string(index);

  while(currEnd != strIndex) {
    int elf1Score = scores[elf1Index];
    int elf2Score = scores[elf2Index];
    std::string nextScore = std::to_string(elf1Score + elf2Score);
    for(char c : nextScore) {
      scores.push_back(c - '0');
      currEnd = currEnd.substr(1);
      currEnd += c;
      if(currEnd == strIndex) break;
    }
    elf1Index = (elf1Index + elf1Score + 1) % scores.size();
    elf2Index = (elf2Index + elf2Score + 1) % scores.size();
  }

  return ((int) scores.size() - 6);
}

int main() {
  std::ifstream ifstrm("./data/q14.txt", std::ios::in);
  int index;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      index = stoi(line);
    }
  }

  std::cout << "Answer to part 1: " << getScore(index) << std::endl;
  std::cout << "Answer to part 2: " << getNoRecipesToLeft(index) << std::endl;
  return 0;
}