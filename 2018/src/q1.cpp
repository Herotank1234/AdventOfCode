#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

int addNums(std::vector<int> data) {
  int res = 0;
  for(int num : data) res += num;
  return res;
}

int duplicateFrequency(std::vector<int> data) {
  std::unordered_set<int> freqs = {0};
  int currFreq = 0;
  size_t index = 0;
  bool found = false;

  while(!found) {
    currFreq += data[index++];
    if(index == data.size()) index = 0;

    auto search = freqs.find(currFreq);
    if(search != freqs.end()) {
      found = true;
    } else {
      freqs.insert(currFreq);
    }
  }

  return currFreq;
}

int main() {
  std::ifstream ifstrm("./data/q1.txt", std::ios::in);
  std::vector<int> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      line[0] == '+' ? data.push_back(stoi(line.substr(1))) : 
        data.push_back(stoi(line.substr(1)) * -1);
    }
  }

  std::cout << "Answer to part 1: " << addNums(data) << std::endl;
  std::cout << "Answer to part 2: " << duplicateFrequency(data) << std::endl;
  return 0;
}