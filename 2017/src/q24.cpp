#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

typedef struct Component {
  Component(int left1, int right1) : left(left1), right(right1) {}

  void print() const {
    std::cout << left << "/" << right;
  }

  int getStrength() const {
    return left + right;
  }

  int left, right;
} Component;

std::vector<std::vector<Component>> findBridges(Component currComp, std::vector<std::vector<Component>> totalBridges,
  std::vector<Component> currBridge, std::vector<Component> remComps, bool isFlipped) {
  std::vector<std::pair<Component, int>> candidates;
  // Find potential candidates
  for(size_t i = 0; i < remComps.size(); i++) {
    if(isFlipped) {
      if(remComps[i].left == currComp.left || remComps[i].right == currComp.left) {
        candidates.push_back({remComps[i], i});
      }
    } else {
      if(remComps[i].left == currComp.right || remComps[i].right == currComp.right) {
        candidates.push_back({remComps[i], i});
      }
    }
  }

  // If there are no more, return
  if(candidates.size() == 0) {
    return totalBridges;
  }

  std::vector<std::vector<Component>> res;

  for(auto cand : candidates) {
    std::vector<Component> nextRem = remComps;
    nextRem.erase(nextRem.begin() + cand.second);

    std::vector<Component> nextBridge = currBridge;
    nextBridge.push_back(cand.first);

    std::vector<std::vector<Component>> nextTotal = totalBridges;
    nextTotal.push_back(nextBridge);

    bool nextFlipped = isFlipped ? cand.first.right == currComp.left : 
      cand.first.right == currComp.right;
    std::vector<std::vector<Component>> currRes = 
      findBridges(cand.first, nextTotal, nextBridge, nextRem, nextFlipped);
    
    res.insert(res.end(), currRes.begin(), currRes.end());
  }

  return res;
}

int findStrongestBridge(const std::vector<Component>& components) {
  std::vector<std::vector<Component>> allBridges;
  for(size_t i = 0; i < components.size(); i++) {
    if(components[i].left == 0 || components[i].right == 0) {
      std::vector<Component> currComps = components;
      currComps.erase(currComps.begin() + i);
      std::vector<std::vector<Component>> bridges = 
        findBridges(components[i], {{components[i]}}, {components[i]}, 
          currComps, components[i].right == 0);
      allBridges.insert(allBridges.end(), bridges.begin(), bridges.end());
    }
  }

  int strength = 0;
  for(auto bridge : allBridges) {
    int currStrength = 0;
    for(auto comp : bridge) {
      currStrength += comp.getStrength();
    }
    if(currStrength > strength) strength = currStrength;
  }

  return strength;
}

int findStrengthOfLongestBridge(const std::vector<Component>& components) {
  std::vector<std::vector<Component>> allBridges;
  for(size_t i = 0; i < components.size(); i++) {
    if(components[i].left == 0 || components[i].right == 0) {
      std::vector<Component> currComps = components;
      currComps.erase(currComps.begin() + i);
      std::vector<std::vector<Component>> bridges = 
        findBridges(components[i], {{components[i]}}, {components[i]}, 
          currComps, components[i].right == 0);
      allBridges.insert(allBridges.end(), bridges.begin(), bridges.end());
    }
  }

  int strength = 0;
  size_t length = 0;

  for(auto bridge : allBridges) {
    if(bridge.size() >= length) {
      if(bridge.size() > length) {
        strength = 0;
      }
      length = bridge.size();
      int currStrength = 0;
      for(auto comp : bridge) {
        currStrength += comp.getStrength();
      }
      if(currStrength > strength) strength = currStrength;
    }
  }

  return strength;
}

int main() {
  std::ifstream ifstrm("./data/q24.txt", std::ios::in);
  std::vector<Component> components;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("([\\d]+)\\/([\\d]+)[\\s]?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        components.emplace_back(stoi(sm[1]), stoi(sm[2]));
      } else {
        std::cout << "No match" << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << findStrongestBridge(components) << std::endl;
  std::cout << "Answer to part 2: " << findStrengthOfLongestBridge(components) << std::endl;
  return 0;
}