#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <cassert>
#include <queue>

int findInGroupZero(const std::unordered_map<int, std::unordered_set<int>>& pipesMap) {
  std::queue<int> toVisit;
  toVisit.push(0); 
  std::unordered_set<int> visited;

  while(!toVisit.empty()) {
    int curr = toVisit.front();
    toVisit.pop();

    if(visited.find(curr) != visited.end()) continue;
    visited.insert(curr);

    const std::unordered_set<int>& connected = pipesMap.find(curr)->second;
    for(int c : connected) {
      if(visited.find(c) == visited.end()) toVisit.push(c);
    }
  }
  
  return visited.size();
}

int findNoGroups(const std::unordered_map<int, std::unordered_set<int>>& pipesMap) {
  int index = 0, numberOfGroups = 0;
  std::unordered_set<int> visited;
  while(index < 2000) {
    std::queue<int> toVisit;
    toVisit.push(index); 

    while(!toVisit.empty()) {
      int curr = toVisit.front();
      toVisit.pop();

      if(visited.find(curr) != visited.end()) continue;
      visited.insert(curr);

      const std::unordered_set<int>& connected = pipesMap.find(curr)->second;
      for(int c : connected) {
        if(visited.find(c) == visited.end()) toVisit.push(c);
      }
    }
    numberOfGroups++;
    while(visited.find(index) != visited.end()) index++;
  }
  
  return numberOfGroups;
}

int main() {
  std::ifstream ifstrm("./data/q12.txt", std::ios::in);
  std::unordered_map<int, std::unordered_set<int>> pipesMap;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match(("[\\d]+"));
      std::smatch sm;
      int sender = -1;
      std::vector<int> recipients;
      if(std::regex_search(line, sm, match)) {
        sender = stoi(sm.str());
        line = sm.suffix();
      } else {
        std::cout << "No sender" << std::endl;
      }

      while(std::regex_search(line, sm, match)) {
        recipients.push_back(stoi(sm.str()));
        line = sm.suffix();
      }
      
      assert(sender != -1);
      assert(recipients.size() != 0);

      for(int recipient : recipients) {
        pipesMap[sender].insert(recipient);
        pipesMap[recipient].insert(sender);
      }
    }
  }

  std::cout << "Answer to part 1: " << findInGroupZero(pipesMap) << std::endl;
  std::cout << "Answer to part 2: " << findNoGroups(pipesMap) << std::endl;
  return 0;
}