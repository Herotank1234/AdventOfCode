#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

bool inBounds(int j, int i, int width, int length) {
  return j >= 0 && j < width && i >= 0 && i < length;
}

int calcResourceVal(std::vector<std::vector<char>> area) {
  int totalTrees = 0, totalLumberyards = 0;
  for(auto row : area) {
    for(auto col : row) {
      if(col == '|') {
        totalTrees++;
      } else if(col == '#') {
        totalLumberyards++;
      }
    }
  }
  return totalTrees * totalLumberyards;
}

int simulate(std::vector<std::vector<char>> area) {
  int length = (int) area.size(), width = (int) area[0].size();
  int iterations = 10;
  std::vector<std::vector<char>> currArea = area;

  std::vector<std::pair<int, int>> steps = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1, 0},           {1, 0},
    {-1, 1},  {0, 1},  {1, 1}
  };

  for(int iter = 0; iter < iterations; iter++) {
    std::vector<std::vector<char>> nextArea(length, 
      std::vector<char>(width, '@'));
    for(int i = 0; i < length; i++) {
      for(int j = 0; j < width; j++) {
        if(currArea[i][j] == '.') {
          int trees = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length) && currArea[nextI][nextJ] =='|') trees++;
          }
          nextArea[i][j] = (trees >= 3) ? '|' : '.';
        } else if(currArea[i][j] == '|') {
          int lumberyards = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length) && currArea[nextI][nextJ] =='#') lumberyards++;
          }
          nextArea[i][j] = (lumberyards >= 3) ? '#' : '|';
        } else if(currArea[i][j] == '#') {
          int lumberyards = 0, trees = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length)) {
              if(currArea[nextI][nextJ] == '#') {
                lumberyards++;
              } else if(currArea[nextI][nextJ] == '|') {
                trees++;
              }
            }
          }
          nextArea[i][j] = (lumberyards >= 1 && trees >= 1) ? '#' : '.';
        }
      }
    }
    currArea = nextArea;
  }

  return calcResourceVal(currArea);
}

int simulateLong(std::vector<std::vector<char>> area) {
  int length = (int) area.size(), width = (int) area[0].size();
  int iterations = 500;
  std::vector<std::vector<char>> currArea = area;

  std::map<std::vector<std::vector<char>>, int> repeats;
  repeats.insert({currArea, 0});
  std::unordered_map<int, int> scores;
  scores.insert({0, calcResourceVal(currArea)});

  std::vector<std::pair<int, int>> steps = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1, 0},           {1, 0},
    {-1, 1},  {0, 1},  {1, 1}
  };

  for(int iter = 1; iter < iterations; iter++) {
    std::vector<std::vector<char>> nextArea(length, 
      std::vector<char>(width, '@'));
    for(int i = 0; i < length; i++) {
      for(int j = 0; j < width; j++) {
        if(currArea[i][j] == '.') {
          int trees = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length) && currArea[nextI][nextJ] =='|') trees++;
          }
          nextArea[i][j] = (trees >= 3) ? '|' : '.';
        } else if(currArea[i][j] == '|') {
          int lumberyards = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length) && currArea[nextI][nextJ] =='#') lumberyards++;
          }
          nextArea[i][j] = (lumberyards >= 3) ? '#' : '|';
        } else if(currArea[i][j] == '#') {
          int lumberyards = 0, trees = 0;
          for(auto step : steps) {
            int nextJ = j + step.first, nextI = i + step.second;
            if(inBounds(nextJ, nextI, width, length)) {
              if(currArea[nextI][nextJ] == '#') {
                lumberyards++;
              } else if(currArea[nextI][nextJ] == '|') {
                trees++;
              }
            }
          }
          nextArea[i][j] = (lumberyards >= 1 && trees >= 1) ? '#' : '.';
        }
      }
    }
    currArea = nextArea;
    auto search = repeats.find(currArea);
    if(search != repeats.end()) {
      long long iters = 1000000000;
      iters = (iters - search->second) % (iter - search->second);
      return scores[search->second + iters];
      break;
    } else {
      repeats.insert({currArea, iter});
      scores.insert({iter, calcResourceVal(currArea)});
    }
  }

  return calcResourceVal(currArea);
}

int main() {
  std::ifstream ifstrm("./data/q18.txt", std::ios::in);
  std::vector<std::vector<char>> area;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::vector<char> row;
      for(size_t i = 0; i < line.length(); i++) {
        if(line[i] != 13) row.push_back(line[i]);
      }
      area.push_back(row);
    }
  }

  std::cout << "Answer to part 1: " << simulate(area) << std::endl;
  std::cout << "Answer to part 2: " << simulateLong(area) << std::endl;
  return 0;
}