#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

int freqChecksum(std::vector<std::string> data) {
  int twos = 0, threes = 0;
  for(std::string s : data) {
    std::unordered_map<char, int> freqs;
    bool twoFound = false, threeFound = false;
    for(char c : s) freqs[c]++;

    for(auto it =  freqs.begin(); it != freqs.end(); it++) {
      if(!twoFound && it->second == 2) {
        twos++;
        twoFound = true;
      } 
      if(!threeFound && it->second == 3) {
        threes++;
        threeFound = true;
      }
    }
  }

  return twos * threes;
}

bool moreThanOneDifference(std::string s1, std::string s2) {
  int differences = 0;
  for(size_t i = 0; i < s1.length(); i++) {
    if(s1[i] != s2[i]) differences++;
    if(differences > 1) return true;
  }
  return false;
}

std::string correctBoxId(std::vector<std::string> data) {
  std::string box1;
  std::string box2;
  bool found = false;

  for(size_t i = 0; i < data.size() - 1; i++) {
    box1 = data[i];
    for(size_t j = i + 1; j < data.size(); j++) {
      box2 = data[j];
      if(!moreThanOneDifference(box1, box2)) {
        found = true;
        break;
      }
    }
    if(found) break;
  }

  std::string res = "";
  for(size_t i = 0; i < box1.length(); i++) {
    if(box1[i] == box2[i]) res += box1[i];
  }

  return res;
}

int main() {
  std::ifstream ifstrm("./data/q2.txt", std::ios::in);
  std::vector<std::string> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      data.push_back(line);
    }
  }

  std::cout << "Answer to part 1: " << freqChecksum(data) << std::endl;
  std::cout << "Answer to part 2: " << correctBoxId(data) << std::endl;
  return 0;
}