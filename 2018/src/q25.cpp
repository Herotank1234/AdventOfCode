#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <set>

typedef struct Point {
  Point(int w, int x, int y, int z) : w(w), x(x), y(y), z(z) {}

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.w << " " << p.x << " " << p.y << " " << p.z;
    return os;
  }

  int w, x, y, z;
} Point;

inline int distance(const Point& a, const Point& b) {
  return abs(a.w - b.w) + abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

size_t count_constellations(const std::vector<Point>& points) {
  std::vector<std::vector<Point>> constellations;

  for(auto point : points) {
    std::vector<size_t> matching_const_index;
    for(size_t i = 0; i < constellations.size(); i++) {
      for(auto p : constellations[i]) {
        if(distance(point, p) <= 3) {
          matching_const_index.push_back(i);
          break;
        }
      }
    }

    if(matching_const_index.size() == 0) {
      constellations.push_back({point});
    } else {
      size_t origin = matching_const_index[0];
      constellations[origin].push_back(point);

      for(size_t index = matching_const_index.size() - 1; index >= 1; index--) {
        constellations[origin].insert(constellations[origin].end(), 
          constellations[matching_const_index[index]].begin(), 
          constellations[matching_const_index[index]].end());
        constellations.erase(constellations.begin() + matching_const_index[index]);
      }
    }
  }

  return constellations.size();
}

int main() {
  std::ifstream ifstrm("./data/q25.txt", std::ios::in);
  std::vector<Point> points;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("(-?[\\d]+),(-?[\\d]+),(-?[\\d]+),(-?[\\d]+)\\s?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        points.emplace_back(std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3]), 
          std::stoi(sm[4]));
      } else {
        std::cout << "Unable to match line: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << count_constellations(points) << std::endl;
  return 0;
}