#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <queue>
#include <climits>
#include <algorithm>

inline bool in_bounds(int i, int j, int length, int width) {
  return i >= 0 && i < length && j >= 0 && j < width;
}

std::vector<int> get_edges_from_source (
  const std::vector<std::vector<char>> &grid,
  const std::pair<int, int> &source,
  const size_t num_vertices
)
{
  std::vector<int> edges(num_vertices, 0);
  std::queue<std::pair<int, std::pair<int, int>>> to_visit;
  std::unordered_set<std::string> visited;

  std::vector<std::pair<int, int>> directions = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
  };

  to_visit.push({0, source});

  int length = static_cast<int>(grid.size()), width = static_cast<int>(grid[0].size());

  while(!to_visit.empty()) {
    auto state = to_visit.front();
    to_visit.pop();

    int steps = state.first;
    std::pair<int, int> pos = state.second;

    std::string key = std::to_string(pos.first) + "," + std::to_string(pos.second);
    if(visited.find(key) != visited.end()) continue;
    visited.insert(key);

    if(grid[pos.first][pos.second] >= '0' && grid[pos.first][pos.second] <= '9') {
      int index = grid[pos.first][pos.second] - '0';
      edges[index] = steps;
    }

    for(auto direction : directions) {
      int next_i = pos.first + direction.first;
      int next_j = pos.second + direction.second;
      std::string next_key = std::to_string(next_i) + "," + std::to_string(next_j);
      if(in_bounds(next_i, next_j, length, width) && visited.find(next_key) == visited.end()
        && grid[next_i][next_j] != '#') 
      {
        to_visit.push({steps + 1, {next_i, next_j}});
      }
    }
  }

  return edges;
}

int find_shortest_path_length(
  const std::vector<std::vector<char>> &grid,
  const std::map<int, std::pair<int, int>> &num_positions,
  bool with_return
)
{
  std::vector<std::vector<int>> edges(num_positions.size(), 
    std::vector<int>(num_positions.size(), 0));

  for(size_t i = 0; i < num_positions.size(); i++) {
    std::vector<int> edges_from_source = 
      get_edges_from_source(grid, num_positions.find(i)->second, num_positions.size());

    for(size_t j = 0; j < edges_from_source.size(); j++) {
      edges[i][j] = edges_from_source[j];
    }
  }

  std::vector<int> nodes_to_visit;
  for(int i = 1; i < static_cast<int>(num_positions.size()); i++) {
    nodes_to_visit.push_back(i);
  }

  int shortest_path_length = INT_MAX;
  while(std::next_permutation(nodes_to_visit.begin(), nodes_to_visit.end())) {
    int curr_length = edges[0][nodes_to_visit[0]];
    for(size_t i = 0; i < nodes_to_visit.size() - 1; i++) {
      curr_length += edges[nodes_to_visit[i]][nodes_to_visit[i + 1]];
    }
    if(with_return) curr_length += edges[nodes_to_visit[nodes_to_visit.size()- 1]][0];
    shortest_path_length = std::min(shortest_path_length, curr_length);
  }

  return shortest_path_length;
}

int main() {
  std::ifstream ifstrm("./data/q24.txt", std::ios::in);
  std::vector<std::vector<char>> grid;
  std::map<int, std::pair<int, int>> num_positions;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    size_t row = 0;
    while(std::getline(ifstrm, line)) {
      std::vector<char> curr_row;

      for(size_t col = 0; col < line.size(); col++) {
        if(line[col] >= '0' && line[col] <= '9') {
          int value = line[col] - '0';
          num_positions.insert({value, {row, col}});
        }
        if(line[col] != '\n') curr_row.push_back(line[col]);
      }

      grid.push_back(curr_row);
      row++;
    }
  }

  std::cout << "Answer to part 1: " << find_shortest_path_length(grid, num_positions, false) << std::endl;
  std::cout << "Answer to part 2: " << find_shortest_path_length(grid, num_positions, true) << std::endl;
  return 0;
}