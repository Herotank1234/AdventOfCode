#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <cassert>
#include <algorithm>

typedef struct Triangle {
  Triangle(int x1, int y1, int z1) : x(x1), y(y1), z(z1) {}
  int x, y, z;
} Triangle;

int countValidTriangles(const std::vector<Triangle>& triangles) {
  int valid = 0;
  for(const Triangle& t : triangles) {
    if(t.x + t.y > t.z && t.x + t.z > t.y && t.y + t.z > t.x) valid++;
  }
  return valid;
}

int countValidTrianglesByCol(const std::vector<Triangle>& triangles) {
  int valid = 0;
  for(size_t i = 0; i < triangles.size(); i += 3) {
    std::vector<int> xs = {triangles[i].x, triangles[i + 1].x, triangles[i + 2].x};
    std::vector<int> ys = {triangles[i].y, triangles[i + 1].y, triangles[i + 2].y};
    std::vector<int> zs = {triangles[i].z, triangles[i + 1].z, triangles[i + 2].z};   
    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    std::sort(zs.begin(), zs.end());

    if(xs[0] + xs[1] > xs[2]) valid++;
    if(ys[0] + ys[1] > ys[2]) valid++;
    if(zs[0] + zs[1] > zs[2]) valid++;
  }
  return valid;
}

int main() {
  std::ifstream ifstrm("./data/q3.txt", std::ios::in);
  std::vector<Triangle> triangles;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("([\\d]+)");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      std::vector<int> dims;
      while(std::regex_search(line, sm, match)) {
        dims.push_back(stoi(sm[1].str()));
        line = sm.suffix();
      }
      assert(dims.size() == 3);
      triangles.emplace_back(dims[0], dims[1], dims[2]);
    } 
  }

  std::cout << "Answer to part 1: " << countValidTriangles(triangles) << std::endl;
  std::cout << "Answer to part 2: " << countValidTrianglesByCol(triangles) << std::endl;
  return 0;
}