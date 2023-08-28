#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <climits>

class Point {
  public:
    Point(int x, int y, int vel_x, int vel_y) : _x(x), _y(y), _vel_x(vel_x), _vel_y(vel_y) {}

    void move() {
      _x += _vel_x;
      _y += _vel_y;
    }

    int get_x() const {
      return _x;
    }

    int get_y() const {
      return _y;
    }

  private:
    int _x;
    int _y;
    int _vel_x;
    int _vel_y;
};

void show(const std::vector<Point> &points) {
  int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
  for(const Point &p : points) {
    min_x = std::min(min_x, p.get_x());
    max_x = std::max(max_x, p.get_x());
    min_y = std::min(min_y, p.get_y());
    max_y = std::max(max_y, p.get_y());
  }

  std::vector<std::vector<char>> grid(max_y - min_y + 1, 
    std::vector<char>(max_x - min_x + 1, ' '));

  for(const Point &p : points) {
    grid[p.get_y() - min_y][p.get_x() - min_x] = '#';
  }

  std::cout << std::endl;
  for(const auto &row : grid) {
    for(const auto &col : row) {
      std::cout << col;
    }
    std::cout << std::endl;
  }
}

std::string converge(std::vector<Point> points) {
  /* After testing with different times, letters could be read at time 10521 */
  for(int _ = 0; _ < 10521; _++) {
    for(Point &p : points) {
      p.move();
    }
  }

  show(points);

  return "XPFXXXKL";
}

int time_to_converge() {
  /* From the above testing, it was shown to converge at time 10521 */
  return 10521;
}

int main() {
  std::ifstream ifstrm("./data/q10.txt", std::ios::in);
  std::vector<Point> points;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("position=<([\\- ][\\d]{5}), ([\\- ][\\d]{5})> " 
      "velocity=<([\\- ][\\d]{1}), ([\\- ][\\d]{1})>\\s?");
    std::smatch sm;

    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        points.emplace_back(std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3]), 
          std::stoi(sm[4]));
      } else {
        std::cout << "Was unable to match line: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << converge(points) << std::endl;
  std::cout << "Answer to part 2: " << time_to_converge() << std::endl;
  return 0;
}