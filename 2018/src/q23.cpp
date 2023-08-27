#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

typedef struct Nanobot {
  int x, y, z, r;
  Nanobot(int x1, int y1, int z1, int r1) :
    x(x1), y(y1), z(z1), r(r1) {}

  void print() {
    std::cout << "pos<" << x << "," << y << "," << z << ">, r=" 
      << r << std::endl;
  }

  bool inRange(const Nanobot other) {
    int dist = abs(x - other.x) + abs(y - other.y) + abs(z - other.z);
    return dist <= r;
  }
} Nanobot;

struct NanobotGreater {
  bool operator()(Nanobot const &a, Nanobot const &b) {
    return a.r > b.r;
  }
};

int getInRadius(std::vector<Nanobot> nanobots) {
  sort(nanobots.begin(), nanobots.end(), NanobotGreater());
  Nanobot largest = nanobots[0];
  int inRange = 0;
  for(size_t i = 0; i < nanobots.size(); i++) {
    if(largest.inRange(nanobots[i])) inRange++;
  }
  return inRange;
}

int main() {
  std::ifstream ifstrm("./data/q23.txt", std::ios::in);
  std::vector<Nanobot> nanobots;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("pos=<(\\-?[\\d]+),(\\-?[\\d]+),(\\-?[\\d]+)>, r=([\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        nanobots.emplace_back(stoi(sm[1]), stoi(sm[2]), stoi(sm[3]), stoi(sm[4]));
      } else {
        std::cout << "NO MATCH" << std::endl;
        std::cout << line << std::endl; 
      }
    }
  }

  std::cout << "Answer to part 1: " << getInRadius(nanobots) << std::endl;
  std::cout << "Answer to part 2: " << std::endl;
  return 0;
}