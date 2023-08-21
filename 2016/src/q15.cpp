#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

typedef struct Disc {
  Disc(int positions, int pos) : positions(positions), pos(pos) {}

  void tick() { 
    pos++;
    pos = pos % positions;
  }

  int positions;
  int pos;
} Disc;

int find_smallest_time_to_drop_capsule(std::vector<Disc> discs) {
  int time = 0;
  while(true) {
    bool found = true;

    for(size_t i = 0; i < discs.size(); i++) {
      if((discs[i].pos + (i + 1)) % discs[i].positions != 0) {
        found = false;
        break;
      }
    }

    if(found) { break; }

    time++;
    for(Disc& disc : discs) {
      disc.tick();
    }
  }
  return time;
}

int main() {
  std::ifstream ifstrm("./data/q15.txt", std::ios::in);
  std::vector<Disc> discs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("Disc #[\\d]+ has ([\\d]+) positions; at time=0, it is at position ([\\d]+).");
    std::smatch sm;

    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        discs.emplace_back(std::stoi(sm[1]), std::stoi(sm[2]));
      }
    }
  }

  std::cout << "Answer to part 1: " << find_smallest_time_to_drop_capsule(discs) << std::endl;
  discs.emplace_back(11, 0);
  std::cout << "Answer to part 2: " << find_smallest_time_to_drop_capsule(discs) << std::endl;
  return 0;
}