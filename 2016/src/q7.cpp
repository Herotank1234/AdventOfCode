#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>

typedef struct IPv7 {
  IPv7(std::vector<std::string> in, std::vector<std::string> out) :
    inBracket(in), outBracket(out) {}
  std::vector<std::string> inBracket;
  std::vector<std::string> outBracket;
} IPv7;

bool findRepeatingLetters(const std::string& s) {
  for(size_t i = 0; i < s.size() - 3; i++) {
    if(s[i] == s[i + 3] && s[i + 1] == s[i + 2] && s[i] != s[i + 1]) return true;
  }
  return false;
}

int countTLSSupportingIps(const std::vector<IPv7>& ips) {
  int count = 0;
  for(const IPv7& ip : ips) {
    bool valid = false;
    for(const std::string& s : ip.outBracket) {
      if(findRepeatingLetters(s)) {
        valid = true;
        break;
      }
    }
    if(valid) {
      for(const std::string& s : ip.inBracket) {
        if(findRepeatingLetters(s)) {
          valid = false;
          break;
        }
      }
    }
    if(valid) count++;
  }
  return count;
}

void findABAs(const std::string& s, std::unordered_set<std::string>& ABAs) {
  for(size_t i = 0; i < s.length() - 2; i++) {
    if(s[i] == s[i + 2] && s[i] != s[i + 1]) {
      std::string ABA = s.substr(i, 3);
      ABAs.insert(ABA);
    }
  }
}

bool findMatchingBAB(const std::string& s, const std::unordered_set<std::string>& ABAs) {
  for(size_t i = 0; i < s.length() - 2; i++) {
    if(s[i] == s[i + 2] && s[i] != s[i + 1]) {
      std::string BABinverted = std::string() + s[i + 1] + s[i] + s[i + 1];
      auto search = ABAs.find(BABinverted);
      if(search != ABAs.end()) {
        return true;
      }
    }
  }
  return false;
}

int countSSLSupportingIps(const std::vector<IPv7>& ips) {
  int count = 0;
  for(const IPv7& ip : ips) {
    std::unordered_set<std::string> ABAs;
    for(const std::string& s : ip.outBracket) {
      findABAs(s, ABAs);
    }

    for(const std::string& s : ip.inBracket) {
      if(findMatchingBAB(s, ABAs)) {
        count++;
        break;
      }
    }
  }
  return count;
}

int main() {
  std::ifstream ifstrm("./data/q7.txt", std::ios::in);
  std::vector<IPv7> ips;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex matchInBracket("\\[([a-z]+)\\]");
    std::regex matchOutBracket("([a-z]+)");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      std::vector<std::string> inBracket;
      std::vector<std::string> outBracket;
      if(std::regex_search(line, sm, matchOutBracket)) {
        outBracket.push_back(sm[1]);
        line = sm.suffix();
      } else {
        std::cout << "No match: " << line << std::endl;
      }
      while(std::regex_search(line, sm, matchInBracket)) {
        inBracket.push_back(sm[1]);
        line = sm.suffix();
        if(std::regex_search(line, sm, matchOutBracket)) {
          outBracket.push_back(sm[1]);
          line = sm.suffix();
        } else {
          std::cout << "No match: " << line << std::endl;
        }
      }
      ips.emplace_back(inBracket, outBracket);
    }
  }

  std::cout << "Answer to part 1: " << countTLSSupportingIps(ips) << std::endl;
  std::cout << "Answer to part 2: " << countSSLSupportingIps(ips) << std::endl;
  return 0;
}