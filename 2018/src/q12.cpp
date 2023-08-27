#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>

int grow(std::string initialState, std::unordered_map<std::string, std::string> rules) {
  std::string currGen = initialState;
  std::string nextGen = "";
  int gens = 20, zeroIndex = 0;
  for(int i = 0; i < gens; i++) {
    currGen = "...." + currGen + "....";
    zeroIndex += 2;
    for(size_t j = 0; j < currGen.length() - 4; j++) {
      std::string currWindow = currGen.substr(j, 5);
      auto search = rules.find(currWindow);
      nextGen += (search == rules.end()) ? "." : search->second;
    }
    currGen = nextGen;
    nextGen = "";
  }

  int plantNums = 0;
  for(size_t i = 0; i < currGen.length(); i++) {
    
    if(currGen[i] == '#') {
      plantNums += (i - zeroIndex);
    }
  }
  return plantNums;
}

long long manyYearsGrow() {
  /* After looking at the number of plants every 20 generations for
  2000 generations, after the first 100 gens, the difference between
  the number of plants per 20 gens was 1240
  Number of plants after 100 gens: 5691
  Number of cycles left (per 20 gens): (50000000000 - 100) / 20
  Difference in number of plants per 20 gens: 1240*/
  long long res = 5691 + (((50000000000 - 100) / 20) * 1240);
  return res;
}



int main() {
  std::ifstream ifstrm("./data/q12.txt", std::ios::in);
  std::vector<int> data;
  std::string initialState;
  std::unordered_map<std::string, std::string> rules;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;

    std::getline(ifstrm, line);
    std::regex match("initial state: ([#|\\.]+)[\\s]?");
    std::smatch sm;
    if(std::regex_match(line, sm, match)) {
      initialState = sm[1];
    }

    std::getline(ifstrm, line);

    while(std::getline(ifstrm, line)) {
      std::regex match1("([#|\\.]{5}) => ([#|\\.])[\\s]?");
      std::smatch sm1;
      if(std::regex_match(line, sm1, match1)) {
        rules.insert({sm1[1], sm1[2]});
      }
    }

  }

  std::cout << "Answer to part 1: " << grow(initialState, rules) << std::endl;
  std::cout << "Answer to part 2: " << manyYearsGrow() << std::endl;
  return 0;
}