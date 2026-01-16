#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct Point {
  int i;
  int j;
} Point;

void initialise_antennas(std::vector<std::vector<char>> &grid, 
  std::unordered_map<char, std::vector<Point>> &antennas) 
{
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] != '.') {
        Point p = {(int) i, (int) j};
        if(antennas.find(grid[i][j]) != antennas.end()) {
          antennas[grid[i][j]].push_back(p);
        } else {
          antennas.insert({grid[i][j], {p}});
        }
      }
    }
  }
}

bool is_valid_candidate(Point p, int height, int width) {
  return p.i >= 0 && p.i < height && p.j >= 0 && p.j < width;
}

std::vector<Point> get_antinode_candidates(Point a1, Point a2) {  
  int di = a1.i - a2.i;
  int dj = a1.j - a2.j;

  Point anti1 = {a1.i + di, a1.j + dj};
  Point anti2 = {a2.i - di, a2.j - dj};

  return {anti1, anti2};
}

std::vector<Point> get_inline_antinode_candidates(Point a1, Point a2, int height, int width) {
  std::vector<Point> candidates;
  candidates.push_back(a1);
  candidates.push_back(a2);

  int di = a1.i - a2.i;
  int dj = a1.j - a2.j;

  Point anti1 = {a1.i + di, a1.j + dj};
  while(is_valid_candidate(anti1, height, width)) {
    candidates.push_back(anti1);
    anti1.i += di;
    anti1.j += dj;
  }

  Point anti2 = {a2.i - di, a2.j - dj};
  while(is_valid_candidate(anti2, height, width)) {
    candidates.push_back(anti2);
    anti2.i -= di;
    anti2.j -= dj;
  }

  return candidates;
}

size_t count_valid_antinodes(std::vector<std::vector<char>> &grid, 
  std::unordered_map<char, std::vector<Point>> &antennas)
{
  std::set<std::string> antinodes;
  int height = grid.size();
  int width = grid[0].size();

  for(std::pair<char, std::vector<Point>> antenna : antennas) {
    std::vector<Point> locations = antenna.second;

    for(size_t i = 0; i < locations.size() - 1; i++) {
      Point a1 = locations[i];

      for(size_t j = i + 1; j < locations.size(); j++) {
        Point a2 = locations[j];
        std::vector<Point> antinode_candidates = get_antinode_candidates(a1, a2);

        for(Point candidate : antinode_candidates) {
          if(is_valid_candidate(candidate, height, width)) {
            std::stringstream ss;
            ss << candidate.i << "-" << candidate.j;
            antinodes.insert(ss.str());
          }
        }
      }
    }
  }

  return antinodes.size();
}

size_t count_valid_inline_antinodes(std::vector<std::vector<char>> &grid, 
  std::unordered_map<char, std::vector<Point>> &antennas)
{
  std::set<std::string> antinodes;
  int height = grid.size();
  int width = grid[0].size();

  for(std::pair<char, std::vector<Point>> antenna : antennas) {
    std::vector<Point> locations = antenna.second;

    for(size_t i = 0; i < locations.size() - 1; i++) {
      Point a1 = locations[i];

      for(size_t j = i + 1; j < locations.size(); j++) {
        Point a2 = locations[j];
        std::vector<Point> antinode_candidates = get_inline_antinode_candidates(a1, a2, height, width);

        for(Point candidate : antinode_candidates) {
          std::stringstream ss;
          ss << candidate.i << "-" << candidate.j;
          antinodes.insert(ss.str());
        }
      }
    }
  }

  return antinodes.size();
}

int main() {
  std::ifstream input("./data/q8.txt");

  std::vector<std::vector<char>> grid;
  std::unordered_map<char, std::vector<Point>> antennas;

  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end() - (line[line.size() - 1] == '\r' ? 1 : 0));
    grid.push_back(row);
  }

  initialise_antennas(grid, antennas);

  std::cout << "Answer to part 1: " << count_valid_antinodes(grid, antennas) << std::endl;
  std::cout << "Answer to part 2: " << count_valid_inline_antinodes(grid, antennas) << std::endl;
  return 0;
}