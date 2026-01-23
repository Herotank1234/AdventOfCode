#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <vector>

#define TURN 1000
#define STEP 1

typedef struct Point {
  int i;
  int j;
  bool operator <(const Point &other) const {
    if(i == other.i) return j < other.j;
    return i < other.i;
  }
} Point;

typedef struct State {
  Point p;
  int dir;
  int score;
} State;

typedef struct State_With_Paths {
  Point p;
  int dir;
  int score;
  std::vector<Point> path;
  bool operator <(const State_With_Paths &other) const {
    return score < other.score;
  }
} State_With_Paths;

const std::vector<Point> dirs = {
  {0, 1},
  {1, 0},
  {0, -1},
  {-1, 0}
};

void set_endpoints(std::vector<std::vector<char>> &grid, Point &start, Point &end) {
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == 'S') {
        grid[i][j] = '.';
        start.i = i;
        start.j = j;
      }
      if(grid[i][j] == 'E') {
        grid[i][j] = '.';
        end.i = i;
        end.j = j;
      }
    }
  }
}

int get_min_score(std::vector<std::vector<char>> &grid, Point &start, Point &end) {
  int height = grid.size();
  int width = grid[0].size();

  std::vector<std::vector<int>> min_scores(height, 
    std::vector<int>(width, std::numeric_limits<int>::max()));
  std::queue<State> to_visit;
  to_visit.push({start, 0, 0});
  
  while(!to_visit.empty()) {
    State curr_state = to_visit.front();
    to_visit.pop();

    if(min_scores[curr_state.p.i][curr_state.p.j] <= curr_state.score) continue;
    min_scores[curr_state.p.i][curr_state.p.j] = curr_state.score;

    if(curr_state.p.i == end.i && curr_state.p.j == end.j) continue;

    int next_i = curr_state.p.i + dirs[curr_state.dir].i;
    int next_j = curr_state.p.j + dirs[curr_state.dir].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, curr_state.dir, curr_state.score + STEP});
    }

    int c1 = (curr_state.dir + 1) % dirs.size();
    next_i = curr_state.p.i + dirs[c1].i;
    next_j = curr_state.p.j + dirs[c1].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, c1, curr_state.score + TURN + STEP});
    }

    int c2 = (curr_state.dir + 2) % dirs.size();
    next_i = curr_state.p.i + dirs[c2].i;
    next_j = curr_state.p.j + dirs[c2].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, c2, curr_state.score + TURN + TURN + STEP});
    }
    
    int ac1 = (curr_state.dir - 1 + dirs.size()) % dirs.size();
    next_i = curr_state.p.i + dirs[ac1].i;
    next_j = curr_state.p.j + dirs[ac1].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, ac1, curr_state.score + TURN + STEP});
    }
  }

  return min_scores[end.i][end.j];
}

int count_unique_tiles_on_best_paths(std::vector<std::vector<char>> &grid, 
  Point &start, Point &end) 
{
  int height = grid.size();
  int width = grid[0].size();
  int min_score = get_min_score(grid, start, end);

  std::vector<std::vector<std::vector<int>>> min_scores(height, 
    std::vector<std::vector<int>>(width, std::vector<int>(4, std::numeric_limits<int>::max())));

  std::priority_queue<State_With_Paths> to_visit;
  to_visit.push({start, 0, 0, {}});
  std::vector<State_With_Paths> paths;
  
  while(!to_visit.empty()) {
    State_With_Paths curr_state = to_visit.top();
    to_visit.pop();

    if(curr_state.score > min_score) continue;

    if(min_scores[curr_state.p.i][curr_state.p.j][curr_state.dir] < curr_state.score) continue;
    min_scores[curr_state.p.i][curr_state.p.j][curr_state.dir] = curr_state.score;
    curr_state.path.push_back(curr_state.p);

    if(curr_state.p.i == end.i && curr_state.p.j == end.j) {
      paths.push_back(curr_state);
      continue;
    }

    int next_i = curr_state.p.i + dirs[curr_state.dir].i;
    int next_j = curr_state.p.j + dirs[curr_state.dir].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, curr_state.dir, curr_state.score + STEP, curr_state.path});
    }

    int c1 = (curr_state.dir + 1) % dirs.size();
    next_i = curr_state.p.i + dirs[c1].i;
    next_j = curr_state.p.j + dirs[c1].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, c1, curr_state.score + TURN + STEP, curr_state.path});
    }

    int c2 = (curr_state.dir + 2) % dirs.size();
    next_i = curr_state.p.i + dirs[c2].i;
    next_j = curr_state.p.j + dirs[c2].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, c2, curr_state.score + TURN + TURN + STEP, curr_state.path});
    }
    
    int ac1 = (curr_state.dir - 1 + dirs.size()) % dirs.size();
    next_i = curr_state.p.i + dirs[ac1].i;
    next_j = curr_state.p.j + dirs[ac1].j;
    if(grid[next_i][next_j] == '.') {
      to_visit.push({{next_i, next_j}, ac1, curr_state.score + TURN + STEP, curr_state.path});
    }
  }

  std::set<Point> unique_tiles;
  for(State_With_Paths swp : paths) {
    if(swp.score == min_score) {
      for(Point p : swp.path) unique_tiles.insert(p);
    }
  }

  return unique_tiles.size();
}

int main() {
  std::ifstream input("./data/q16.txt");
  std::string line;

  std::vector<std::vector<char>> grid;
  while(std::getline(input, line)) {
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  Point start;
  Point end;
  set_endpoints(grid, start, end);  

  std::cout << "Answer to part 1: " << get_min_score(grid, start, end) << std::endl;
  std::cout << "Answer to part 2: " << count_unique_tiles_on_best_paths(grid, start, end) 
    << std::endl;
  return 0;
}