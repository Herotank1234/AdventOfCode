#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

#define WIDTH 101
#define HEIGHT 103

class Robot {
  public:
    Robot(int j, int i, int dj, int di) : _i(i), _j(j), _di(di), _dj(dj) {}

    void move() {
      _i += _di;
      if(_i < 0) _i += HEIGHT;
      if(_i >= HEIGHT) _i -= HEIGHT;

      _j += _dj;
      if(_j < 0) _j += WIDTH;
      if(_j >= WIDTH) _j -= WIDTH;
    }

    int get_i() {
      return _i;
    }

    int get_j() {
      return _j;
    }
  private:
    int _i;
    int _j;
    int _di;
    int _dj;
};

int simulate_and_count_quadrants(std::vector<Robot> robots) {
  for(Robot &r : robots) {
    for(int i = 0; i < 100; i++) r.move();
  }
  
  int top_left = 0;
  int top_right = 0;
  int bot_left = 0;
  int bot_right = 0;

  for(Robot &r : robots) {
    if(r.get_i() < HEIGHT / 2) {
      if(r.get_j() < WIDTH / 2) {
        top_left++;
      } else if(r.get_j() > WIDTH / 2) {
        top_right++;
      }
    } else if(r.get_i() > HEIGHT / 2) {
      if(r.get_j() < WIDTH / 2) {
        bot_left++;
      } else if(r.get_j() > WIDTH / 2) {
        bot_right++;
      }
    }
  }

  return top_left * top_right * bot_left * bot_right;
}

int find_christmas_tree(std::vector<Robot> robots) {
  int iteration = 0;

  while(true) {
    std::vector<std::vector<bool>> grid(HEIGHT, std::vector<bool>(WIDTH, false));
    for(Robot &r : robots) grid[r.get_i()][r.get_j()] = true;

    int max_consecutive = 0;
    for(std::vector<bool> &row : grid) {
      int max_row_consecutive = 0;
      int curr_consecutive = 0;
      int ptr = 0;

      while(ptr < WIDTH) {
        while(ptr < WIDTH && !row[ptr]) ptr++;

        while(ptr < WIDTH && row[ptr]) {
          curr_consecutive++;
          ptr++;
        }

        max_row_consecutive = std::max(max_row_consecutive, curr_consecutive);
        curr_consecutive = 0;
      }

      max_consecutive = std::max(max_consecutive, max_row_consecutive);
    }

    if(max_consecutive > 20) return iteration;

    for(Robot &r : robots) r.move();
    iteration++;
  }

  return -1;
}

int main() {
  std::ifstream input ("./data/q14.txt");
  std::vector<Robot> robots;
  std::string line;

  std::regex rgx("p=([\\d]+),([\\d]+) v=(-?[\\d]+),(-?[\\d]+)");
  std::smatch match;

  while(std::getline(input, line)) {
    if(std::regex_match(line, match, rgx)) {
      int j = std::stoi(match[1].str());
      int i = std::stoi(match[2].str());
      int dj = std::stoi(match[3].str());
      int di = std::stoi(match[4].str());
      robots.emplace_back(j, i, dj, di);
    } else {
      std::cout << line << " could not be matched" << std::endl;
    }
  }

  std::cout << "Answer to part 1: " << simulate_and_count_quadrants(robots) << std::endl;
  std::cout << "Answer to part 2: " << find_christmas_tree(robots) << std::endl;
  return 0;
}