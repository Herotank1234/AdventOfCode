#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>

void transpose(std::string& s) {
  if(s.length() == 4) {
    std::swap(s[1], s[2]);
  } else {
    std::swap(s[1], s[3]);
    std::swap(s[2], s[6]);
    std::swap(s[5], s[7]);
  }
}

void reverseS(std::string& s) {
  std::string reversed = "";
  if(s.length() == 4) {
    reversed = s.substr(2, 2) + s.substr(0, 2);
  } else {
    reversed = s.substr(6, 3) + s.substr(3, 3) + s.substr(0, 3);
  }
  s = reversed;
}

void extendRules(std::unordered_map<std::string, std::string>& rules) {
  std::unordered_map<std::string, std::string> extendedRules;

  for(auto rule : rules) {
    std::string key = rule.first;
    std::string val = rule.second;
    extendedRules.insert({key, val});
    transpose(key);
    extendedRules.insert({key, val});
    reverseS(key);
    extendedRules.insert({key, val});
    transpose(key);
    extendedRules.insert({key, val});
    reverseS(key);
    extendedRules.insert({key, val});
    transpose(key);
    extendedRules.insert({key, val});
    reverseS(key);
    extendedRules.insert({key, val});
    transpose(key);
    extendedRules.insert({key, val});
  }

  rules = extendedRules;
}

int expandAndCount(int iter, std::vector<std::string> grid, const std::unordered_map<std::string, std::string>& rules) {
  for(int reps = 0; reps < iter; reps++) {
    std::vector<std::string> nextGrid;
    if(grid.size() % 2 == 0) {
      for(size_t i = 0; i < grid.size(); i += 2) {
        std::string row1 = "";
        std::string row2 = "";
        std::string row3 = "";
        for(size_t j = 0; j < grid[0].size(); j += 2) {
          std::string key = std::string() + 
            grid[i][j] + grid[i][j + 1] + grid[i + 1][j] + grid[i + 1][j + 1];
          std::string val = rules.find(key)->second;
          row1 += val.substr(0, 3);
          row2 += val.substr(3, 3);
          row3 += val.substr(6, 3);
        }
        nextGrid.push_back(row1);
        nextGrid.push_back(row2);
        nextGrid.push_back(row3);
      }
    } else {
      for(size_t i = 0; i < grid.size(); i += 3) {
        std::string row1 = "";
        std::string row2 = "";
        std::string row3 = "";
        std::string row4 = "";
        for(size_t j = 0; j < grid[0].size(); j += 3) {
          std::string key = std::string() + 
            grid[i][j] + grid[i][j + 1] + grid[i][j + 2] +
            grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j + 2] + 
            grid[i + 2][j] + grid[i + 2][j + 1] + grid[i + 2][j + 2];
          std::string val = rules.find(key)->second;
          row1 += val.substr(0, 4);
          row2 += val.substr(4, 4);
          row3 += val.substr(8, 4);
          row4 += val.substr(12, 4);
        }
        nextGrid.push_back(row1);
        nextGrid.push_back(row2);
        nextGrid.push_back(row3);
        nextGrid.push_back(row4);
      }
    }
    grid = nextGrid;
  }

  int on = 0;
  for(auto s : grid) {
    for(char c : s) {
      if(c == '#') on++;
    }
  }

  return on;
}

int main() {
  std::ifstream ifstrm("./data/q21.txt", std::ios::in);
  std::unordered_map<std::string, std::string> rules;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("((([.#]{2})\\/([.#]{2}) => ([.#]{3})\\/([.#]{3})\\/([.#]{3}))|"
      "(([.#]{3})\\/([.#]{3})\\/([.#]{3}) => "
      "([.#]{4})\\/([.#]{4})\\/([.#]{4})\\/([.#]{4})))[\\s]?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        if(sm[3] != "") {
          rules.insert({sm[3].str() + sm[4].str(), 
            sm[5].str() + sm[6].str() + sm[7].str()});
        } else {
          rules.insert({sm[9].str() + sm[10].str() + sm[11].str(),
            sm[12].str() + sm[13].str() + sm[14].str() + sm[15].str()});
        }
      }
    }
  }

  extendRules(rules);
  std::vector<std::string> grid = {".#.", "..#", "###"};

  std::cout << "Answer to part 1: " << expandAndCount(5, grid, rules) << std::endl;
  std::cout << "Answer to part 2: " << expandAndCount(18, grid, rules) << std::endl;
  return 0;
}