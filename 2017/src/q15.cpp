#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

typedef long long ll;

int findMatching(ll genA, ll genB) {
  int matching = 0;
  ll factorA = 16807, factorB = 48271, divisor = 2147483647;
  unsigned mask = (1 << 16) - 1;
  for(int i = 0; i < 40000000; i++) {
    genA *= factorA;
    genA %= divisor;

    genB *= factorB;
    genB %= divisor;

    ll matchA = genA & mask;
    ll matchB = genB & mask;
    if(matchA == matchB) matching++;
  }

  return matching;
}

int findMatchingStingy(ll genA, ll genB) {
  int matching = 0;
  ll factorA = 16807, factorB = 48271, divisor = 2147483647;
  unsigned mask = (1 << 16) - 1;
  for(int i = 0; i < 5000000; i++) {
    genA *= factorA;
    genA %= divisor;
    while(genA % 4 != 0) {
      genA *= factorA;
      genA %= divisor;
    }

    genB *= factorB;
    genB %= divisor;
    while(genB % 8 != 0) {
      genB *= factorB;
      genB %= divisor;
    }

    ll matchA = genA & mask;
    ll matchB = genB & mask;
    if(matchA == matchB) matching++;
  }

  return matching;
}

int main() {
  std::ifstream ifstrm("./data/q15.txt", std::ios::in);
  ll genAStart = 0, genBStart = 0;
  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex match("Generator [A-Z] starts with ([\\d]+)[\\s]?");
    std::smatch sm;
    if(std::regex_match(line, sm, match)) {
      genAStart = stoll(sm[1]);
    } else {
      std::cout << "Gen A starting value not found" << std::endl;
    }

    std::getline(ifstrm, line);
    if(std::regex_match(line, sm, match)) {
      genBStart = stoll(sm[1]);
    } else {
      std::cout << "Gen B starting value not found" << std::endl;
    }
  }

  std::cout << "Answer to part 1: " << findMatching(genAStart, genBStart) << std::endl;
  std::cout << "Answer to part 2: " << findMatchingStingy(genAStart, genBStart) << std::endl;
  return 0;
}