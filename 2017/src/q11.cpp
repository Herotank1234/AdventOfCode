#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>

enum Direction {
  NORTH,
  NORTHEAST,
  SOUTHEAST,
  SOUTH,
  SOUTHWEST,
  NORTHWEST
};

int calcDist(const std::vector<Direction>& data) {
  int x = 0, y = 0, z = 0;
  for(Direction d : data) {
    switch(d) {
      case NORTH:
        y++;
        z--;
        break;
      case NORTHEAST:
        x++;
        z--;
        break;
      case SOUTHEAST:
        x++;
        y--;
        break;
      case SOUTH:
        y--;
        z++;
        break;
      case SOUTHWEST:
        x--;
        z++;
        break;
      case NORTHWEST:
        x--;
        y++;
        break;
      default:
        std::cout << "Direction not found" << std::endl;
    }
  }

  return (abs(x) + abs(y) + abs(z)) / 2;
}

int calcMaxDist(const std::vector<Direction>& data) {
  int x = 0, y = 0, z = 0;
  int maxDist = 0;
  for(Direction d : data) {
    switch(d) {
      case NORTH:
        y++;
        z--;
        break;
      case NORTHEAST:
        x++;
        z--;
        break;
      case SOUTHEAST:
        x++;
        y--;
        break;
      case SOUTH:
        y--;
        z++;
        break;
      case SOUTHWEST:
        x--;
        z++;
        break;
      case NORTHWEST:
        x--;
        y++;
        break;
      default:
        std::cout << "Direction not found" << std::endl;
    }
    int currDist = (abs(x) + abs(y) + abs(z)) / 2;
    if(currDist > maxDist) maxDist = currDist;
  }
  return maxDist;
}


int main() {
  std::ifstream ifstrm("./data/q11.txt", std::ios::in);
  std::vector<Direction> data;
  std::unordered_map<std::string, Direction> dirMap = {
    {"n", NORTH},
    {"ne", NORTHEAST},
    {"se", SOUTHEAST},
    {"s", SOUTH},
    {"sw", SOUTHWEST},
    {"nw", NORTHWEST}
  };

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("(ne|se|sw|nw|n|s)");
      std::smatch sm;
      while(std::regex_search(line, sm, match)) {
        data.push_back(dirMap.find(sm.str())->second);
        line = sm.suffix();
      }
    }
  }

  std::cout << "Answer to part 1: " << calcDist(data) << std::endl;
  std::cout << "Answer to part 2: " << calcMaxDist(data) << std::endl;
  return 0;
}