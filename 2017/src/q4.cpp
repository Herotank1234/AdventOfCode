#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <algorithm>

int validPassphrases(const std::vector<std::vector<std::string>> &passphrases) {
  int totalValid = 0;

  for(auto passphrase : passphrases) {
    std::unordered_set<std::string> seen;
    bool valid = true;
    for(auto word : passphrase) {
      if(seen.find(word) != seen.end()) {
        valid = false;
        break;
      }
      seen.insert(word);
    }
    if(valid) totalValid++; 
  }
  
  return totalValid;
}

int validPassphrasesWithAnagrams(const std::vector<std::vector<std::string>> &passphrases) {
  int totalValid = 0;

  for(auto passphrase : passphrases) {
    std::unordered_set<std::string> seen;
    bool valid = true;
    for(auto& word : passphrase) {
      std::sort(word.begin(), word.end());
      if(seen.find(word) != seen.end()) {
        valid = false;
        break;
      }
      seen.insert(word);
    }
    if(valid) totalValid++; 
  }
  
  return totalValid;
}

int main() {
  std::ifstream ifstrm("./data/q4.txt", std::ios::in);
  std::vector<std::vector<std::string>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<std::string> row;
      std::regex match("([a-z]+)");
      std::smatch sm;
      while(std::regex_search(line, sm, match)) {
        row.push_back(sm.str());
        line = sm.suffix();
      }
      data.push_back(row);
    }
  }

  std::cout << "Answer to part 1: " << validPassphrases(data) << std::endl;
  std::cout << "Answer to part 2: " << validPassphrasesWithAnagrams(data) << std::endl;
  return 0;
}