#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>

bool sortByVal(const std::pair<char, int>& a, const std::pair<char, int>& b) {
  return a.second > b.second;
}

std::string mostFrequentChars(const std::vector<std::string>& strs) {
  std::vector<std::unordered_map<char, int>> charFreqs(strs[0].length(),
    std::unordered_map<char, int>());

  for(const std::string& s : strs) {
    for(size_t i = 0; i < s.length(); i++) {
      charFreqs[i][s[i]]++;
    }
  }

  std::vector<std::vector<std::pair<char, int>>> charFreqsV;
  for(auto map : charFreqs) {
    std::vector<std::pair<char, int>> vecForm(map.begin(), map.end());
    charFreqsV.push_back(vecForm);
  }

  for(auto& vec : charFreqsV) {
    std::sort(vec.begin(), vec.end(), sortByVal);
  }

  std::string res = "";
  for(const auto& vec : charFreqsV) {
    res += vec[0].first;
  }
  return res;
}

bool sortByValAscending(const std::pair<char, int>& a, const std::pair<char, int>& b) {
  return a.second < b.second;
}

std::string leastFrequentChars(const std::vector<std::string>& strs) {
  std::vector<std::unordered_map<char, int>> charFreqs(strs[0].length(),
    std::unordered_map<char, int>());

  for(const std::string& s : strs) {
    for(size_t i = 0; i < s.length(); i++) {
      charFreqs[i][s[i]]++;
    }
  }

  std::vector<std::vector<std::pair<char, int>>> charFreqsV;
  for(auto map : charFreqs) {
    std::vector<std::pair<char, int>> vecForm(map.begin(), map.end());
    charFreqsV.push_back(vecForm);
  }

  for(auto& vec : charFreqsV) {
    std::sort(vec.begin(), vec.end(), sortByValAscending);
  }

  std::string res = "";
  for(const auto& vec : charFreqsV) {
    res += vec[0].first;
  }
  return res;
}


int main() {
  std::ifstream ifstrm("./data/q6.txt", std::ios::in);
  std::vector<std::string> strs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("([a-z]+)[\\s]*");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        strs.push_back(sm[1]);
      } else {
        std::cout << "No match: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << mostFrequentChars(strs) << std::endl;
  std::cout << "Answer to part 2: " << leastFrequentChars(strs) << std::endl;
  return 0;
}