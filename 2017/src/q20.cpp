#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>

typedef struct Particle {
  Particle(int x1, int y1, int z1, int xV1, int yV1, int zV1, int xA1, 
    int yA1, int zA1, int id1) :
    x(x1), y(y1), z(z1), xV(xV1), yV(yV1), zV(zV1), xA(xA1), yA(yA1), zA(zA1), id(id1) {}

  int getAbsAccel() const {
    return abs(xA) + abs(yA) + abs(zA);
  }

  int getAbsVel() const {
    return abs(xV) + abs(yV) + abs(zV);
  }

  int getAbsPos() const {
    return abs(x) + abs(y) + abs(z);
  }

  void update() {
    xV += xA;
    yV += yA;
    zV += zA;
    x += xV;
    y += yV;
    z += zV;
  }

  std::string getPosString() const {
    return std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
  }

  bool operator <(const Particle& other) {
    if(getAbsAccel() == other.getAbsAccel()) {
      if(getAbsVel() == other.getAbsVel()) {
        return getAbsPos() < other.getAbsPos();
      } else {
        return getAbsVel() < other.getAbsVel();
      }
    } else {
      return getAbsAccel() < other.getAbsAccel();
    }
  }

  int x, y, z, xV, yV, zV, xA, yA, zA, id;
} Particle;

int findMin(std::vector<Particle> particles) {
  std::sort(particles.begin(), particles.end());
  return particles[0].id;
}

size_t processCollisions(std::vector<Particle> particles) {
  for(int i = 0; i < 1000; i++) {
    for(Particle& p : particles) {
      p.update();
    }

    // Count number of particles for each position
    std::unordered_map<std::string, std::vector<int>> posCount;
    for(const Particle& p : particles) {
      posCount[p.getPosString()].push_back(p.id);
    }

    // If multiple particles take up a position, add them to the removable set
    std::unordered_set<int> removable;
    for(auto it : posCount) {
      if(it.second.size() > 1) {
        for(int i : it.second) {
          removable.insert(i);
        }
      }
    }

    // Remove particles with ids in removable
    auto it = particles.begin();
    while(it != particles.end()) {
      if(removable.find((*it).id) != removable.end()) {
        it = particles.erase(it);
      } else {
        it++;
      }
    }
  }

  return particles.size();
}

int main() {
  std::ifstream ifstrm("./data/q20.txt", std::ios::in);
  std::vector<Particle> particles;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("p=<(-?[\\d]+),(-?[\\d]+),(-?[\\d]+)>, "
    "v=<(-?[\\d]+),(-?[\\d]+),(-?[\\d]+)>, "
    "a=<(-?[\\d]+),(-?[\\d]+),(-?[\\d]+)>[\\s]?");
    int id = 0;
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        particles.emplace_back(stoi(sm[1]), stoi(sm[2]), stoi(sm[3]), stoi(sm[4]), 
          stoi(sm[5]), stoi(sm[6]), stoi(sm[7]), stoi(sm[8]), stoi(sm[9]), id++);
      } else {
        std::cout << "No match" << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << findMin(particles) << std::endl;
  std::cout << "Answer to part 2: " << processCollisions(particles) << std::endl;
  return 0;
}