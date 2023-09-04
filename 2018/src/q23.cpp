#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <cmath>
#include <queue>

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

typedef struct SearchSpace {
  SearchSpace(int x, int y, int z, int length) : 
    x(x), y(y), z(z), length(length)
  {
    dist = abs(x) + abs(y) + abs(z);
  }

  void setInRange(int num) {
    inRange = num;
  }

  inline bool operator<(const SearchSpace& other) const {
    if(inRange != other.inRange) return inRange < other.inRange;
    if(dist != other.dist) return dist > other.dist;
    return length > other.length;
  }

  int x, y, z, dist, length, inRange = 0;
} SearchSpace;

int getInRadius(std::vector<Nanobot> nanobots) {
  sort(nanobots.begin(), nanobots.end(), NanobotGreater());
  Nanobot largest = nanobots[0];
  int inRange = 0;
  for(size_t i = 0; i < nanobots.size(); i++) {
    if(largest.inRange(nanobots[i])) inRange++;
  }
  return inRange;
}

int distanceFromSpace(int val, int lowerBound, int upperBound) {
  if(val < lowerBound) return lowerBound - val;
  if(val > upperBound) return val - upperBound;
  return 0;
}

bool isIntersecting(const Nanobot& n, const SearchSpace& space) {
  int distance = 0;
  distance += distanceFromSpace(n.x, space.x, space.x + space.length - 1);
  distance += distanceFromSpace(n.y, space.y, space.y + space.length - 1);
  distance += distanceFromSpace(n.z, space.z, space.z + space.length - 1);
  return distance <= n.r;
}

int countInRange(const std::vector<Nanobot>& nanobots, const SearchSpace& space) {
  int inRange = 0;
  for(const Nanobot &n : nanobots) {
    if(isIntersecting(n, space)) {
      inRange++;
      continue;
    }
  }

  return inRange;
}

int getMostInRange(std::vector<Nanobot> nanobots) {
  int furthestDistance = 0;
  for(auto nanobot : nanobots) {
    furthestDistance = std::max(furthestDistance, abs(nanobot.x));
    furthestDistance = std::max(furthestDistance, abs(nanobot.y));
    furthestDistance = std::max(furthestDistance, abs(nanobot.z));
  }

  int startingPoint = std::pow(2, std::ceil(std::log2(furthestDistance)));
  
  std::priority_queue<SearchSpace> toVisit;
  SearchSpace initialSpace = SearchSpace(-startingPoint, -startingPoint, -startingPoint, startingPoint * 2);
  initialSpace.setInRange(static_cast<int>(nanobots.size()));
  toVisit.push(initialSpace);

  while(!toVisit.empty()) {
    auto searchSpace = toVisit.top();
    toVisit.pop();

    if(searchSpace.length == 1) return searchSpace.dist;

    int next_len = searchSpace.length / 2;

    for(int next_x = searchSpace.x; next_x <= searchSpace.x + next_len; next_x += next_len) {
      for(int next_y = searchSpace.y; next_y <= searchSpace.y + next_len; next_y += next_len) {
        for(int next_z = searchSpace.z; next_z <= searchSpace.z + next_len; next_z += next_len) {
          SearchSpace nextSpace = SearchSpace(next_x, next_y, next_z, next_len);
          int inRange = countInRange(nanobots, nextSpace);
          nextSpace.setInRange(inRange);
          toVisit.push(nextSpace);
        }
      }
    }
  }

  return 0;
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
  std::cout << "Answer to part 2: " << getMostInRange(nanobots) << std::endl;
  return 0;
}