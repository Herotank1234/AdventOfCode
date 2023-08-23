#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>

std::string getHashString(const std::vector<int> banks) {
  std::string res = "";
  for(int i : banks) {
    res += std::to_string(i);
    res += "_";
  }
  return res;
}

int calcSteps(std::vector<int> banks) {
  int steps = 0;
  std::unordered_set<std::string> seen;
  while(seen.find(getHashString(banks)) == seen.end()) {
    std::string hash = getHashString(banks);
    seen.insert(hash);
    size_t index = 0;
    int space = 0;
    for(size_t i = 0; i < banks.size(); i++) {
      if(banks[i] > space) {
        index = i;
        space = banks[i];
      }
    }

    banks[index] = 0;
    index++;
    while(space > 0) {
      if(index == banks.size()) index = 0;
      banks[index]++;
      index++;
      space--;
    }
    steps++;
  }
  return steps;
}

int calcSize(std::vector<int> banks) {
  int steps = 0;
  std::unordered_map<std::string, int> seen;
  while(seen.find(getHashString(banks)) == seen.end()) {
    std::string hash = getHashString(banks);
    seen.insert({hash, steps});
    size_t index = 0;
    int space = 0;
    for(size_t i = 0; i < banks.size(); i++) {
      if(banks[i] > space) {
        index = i;
        space = banks[i];
      }
    }

    banks[index] = 0;
    index++;
    while(space > 0) {
      if(index == banks.size()) index = 0;
      banks[index]++;
      index++;
      space--;
    }
    steps++;
  }
  return steps - seen.find(getHashString(banks))->second;
}


int main() {
  std::ifstream ifstrm("./data/q6.txt", std::ios::in);
  std::vector<int> banks;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex match("([\\d]+)");
    std::smatch sm;
    while(std::regex_search(line, sm, match)) {
      banks.push_back(stoi(sm.str()));
      line = sm.suffix();
    }
  } 

  std::cout << "Answer to part 1: " << calcSteps(banks) << std::endl;
  std::cout << "Answer to part 2: " << calcSize(banks) << std::endl;
  return 0;
}