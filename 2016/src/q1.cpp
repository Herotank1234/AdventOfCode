#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <complex>
#include <unordered_set>

typedef struct Move {
  Move(bool turnRight1, int steps1) : turnRight(turnRight1), steps(steps1) {}
  
  void print() const {
    std::cout << (turnRight ? "R" : "L") << steps << std::endl;
  }

  bool turnRight;
  int steps;
} Move;

int getDistAfterMoves(const std::vector<Move>& moves) {
  std::complex<int> pos(0, 0), dir(1, 0);

  for(const Move& m : moves) {
    dir *= std::complex<int>(0, m.turnRight ? -1 : 1);
    pos += m.steps * dir;
  }

  return abs(pos.real()) + abs(pos.imag());
}

int getFirstRepeatingLocation(const std::vector<Move>& moves) {
  std::complex<int> pos(0, 0), dir(1, 0);
  std::unordered_set<std::string> seen;

  for(const Move& m : moves) {
    dir *= std::complex<int>(0, m.turnRight ? -1 : 1);
    for(int i = 0; i < m.steps; i++) {
      pos += dir;
      auto search = seen.find(std::to_string(pos.real()) + "_" + std::to_string(pos.imag()));
      if(search != seen.end()) {
        return abs(pos.real()) + abs(pos.imag());
      }
      seen.insert(std::to_string(pos.real()) + "_" + std::to_string(pos.imag()));
    }
  }

  return -1; 
}

int main() {
  std::ifstream ifstrm("./data/q1.txt", std::ios::in);
  std::vector<Move> moves;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(L|R)([\\d]+)");
    std::smatch sm;
    std::getline(ifstrm, line);
    while(std::regex_search(line, sm, match)) {
      bool turnRight = (sm[1].str() == "R");
      int steps = stoi(sm[2]);
      moves.emplace_back(turnRight, steps);
      line = sm.suffix();
    }
  }

  std::cout << "Answer to part 1: " << getDistAfterMoves(moves) << std::endl;
  std::cout << "Answer to part 2: " << getFirstRepeatingLocation(moves) << std::endl;
  return 0;
}