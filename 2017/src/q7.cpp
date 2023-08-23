#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

typedef struct Program {
  std::string name;
  int id;
  std::vector<std::string> children;
  Program(std::string name1, int id1, std::vector<std::string> children1) :
    name(name1), id(id1), children(children1) {}

  void print() const {
    std::cout << name << " (" << id << ")";
    if(children.size() != 0) {
      std::cout << " -> ";
      for(size_t i = 0; i < children.size(); i++) {
        if(i != children.size() - 1) {
          std::cout << children[i] << ", ";
        } else {
          std::cout << children[i];
        }
      }
    }
    std::cout << std::endl;
  }
} Program;

std::string findRoot(std::vector<Program> programs) {
  std::unordered_set<std::string> programNames;
  for(auto program : programs) {
    programNames.insert({program.name});
  }

  for(auto program : programs) {
    for(auto child : program.children) {
      auto search = programNames.find(child);
      if(search != programNames.end()) programNames.erase(search);
    }
  }

  assert(programNames.size() == 1);

  return *(programNames.begin());
}

std::pair<int, int> getCorrectWeight(std::string currProgram, 
  std::unordered_map<std::string, Program> programs) {
  Program curr = programs.find(currProgram)->second;

  if(curr.children.size() == 0) {
    return {0, curr.id};
  }

  std::unordered_map<std::string, int> weights;
  for(std::string child : curr.children) {
    std::pair<int, int> weight = getCorrectWeight(child, programs);
    if(weight.first == -1) return weight;
    weights.insert({child, weight.second});
  }

  std::unordered_map<int, int> weightCounts;
  for(auto w : weights) {
    weightCounts[w.second]++;
  }

  if(weightCounts.size() == 1) {
    return {0, curr.id + (weights.size() * weightCounts.begin()->first)};
  } else {
    int oddOneOut = 0;
    int matched = 0;
    for(auto count : weightCounts) {
      if(count.second == 1) {
        oddOneOut = count.first;
      } else {
        matched = count.first;
      }
    }

    std::string oddProgram = "";
    for(auto w : weights) {
      if(w.second == oddOneOut) {
        oddProgram = w.first;
        break;
      }
    }
    Program odd = programs.find(oddProgram)->second;
    return {-1, odd.id - (oddOneOut - matched)};
  }
  return {-1, -1};
}

int findCorrectWeight(std::vector<Program> programs) {
  std::string root = findRoot(programs);
  std::unordered_map<std::string, Program> programMap;
  for(auto program : programs) {
    programMap.insert({program.name, program});
  }
  std::pair<int, int> res = getCorrectWeight(root, programMap);
  assert(res.first == -1 && res.second != -1);
  return res.second;
}

int main() {
  std::ifstream ifstrm("./data/q7.txt", std::ios::in);
  std::vector<Program> programs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::string name;
      int id;
      std::vector<std::string> children;
      std::regex match("([a-z]+) \\(([\\d]+)\\)");
      std::smatch sm;
      if(std::regex_search(line, sm, match)) {
        name = sm[1];
        id = stoi(sm[2]);
        line = sm.suffix();
      }
      std::regex matchChild("([a-z]+)");
      std::smatch smChild;
      while(std::regex_search(line, smChild, matchChild)) {
        children.push_back(smChild.str());
        line = smChild.suffix();
      }
      programs.emplace_back(name, id, children);
    }
  }

  std::cout << "Answer to part 1: " << findRoot(programs) << std::endl;
  std::cout << "Answer to part 2: " << findCorrectWeight(programs) << std::endl;
  return 0;
}