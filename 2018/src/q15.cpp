#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <queue>
#include <unordered_set>

const std::vector<std::pair<int, int>> directions = {
  {-1, 0},
  {0, -1},
  {0, 1},
  {1, 0}
};

class Character {
  public:
    Character(char type, int power, int i, int j) : _type(type), _power(power), _i(i), _j(j) {}

    char get_type() const {
      return _type;
    }

    int get_hp() const {
      return _hp;
    }

    int get_power() const {
      return _power;
    }

    void set_power(int power) {
      _power = power;
    }

    void set_pos(int next_i, int next_j) {
      _i = next_i;
      _j = next_j;
    }

    int get_i() const {
      return _i;
    }

    int get_j() const {
      return _j;
    }

    void recieve_attack(int dmg) {
      _hp -= dmg;
    }

    bool is_dead() {
      return _hp <= 0;
    }

    bool operator<(const Character &other) const {
      if(_i == other._i) return  _j < other._j;
      return _i < other._i;
    }

  private:
    char _type;
    int _hp = 200;
    int _power;
    int _i;
    int _j;
};

typedef struct Move {
  Move(int i, int j, int steps) : i(i), j(j), steps(steps) {}

  inline bool operator<(const Move &other) {
    if(i == other.i) return j < other.j;
    return i < other.i;
  }

  int i;
  int j;
  int steps;
} Move;

typedef struct Enemy {
  Enemy(int i, int j, int hp) : i(i), j(j), hp(hp) {}

  inline bool operator<(const Enemy &other) {
    if(hp == other.hp) {
      if(i == other.i) {
        return j < other.j;
      }
      return i < other.i;
    }
    return hp < other.hp;
  }

  int i;
  int j;
  int hp;
} Enemy;

std::pair<int, int> count_characters(const std::vector<Character> &characters) {
  std::pair<int, int> counts = {0, 0};
  for(const Character &c : characters) {
    if(c.get_type() == 'G') {
      counts.first++;
    } else {
      counts.second++;
    }
  }
  return counts;
}

int count_enemies_in_range(char type, int i, int j, 
  const std::vector<std::vector<char>> &grid) 
{
  int enemies_in_range = 0;
  char enemy = type == 'G' ? 'E' : 'G';

  for(const auto &direction : directions) {
    int next_i = i + direction.first, next_j = j + direction.second;
    if(grid[next_i][next_j] == enemy) {
      enemies_in_range++;
    }
  }

  return enemies_in_range;
}

std::pair<bool, Move> try_get_next_move(char type, int i, int j, 
  const std::vector<std::vector<char>> &grid) 
{
  char enemy = type == 'G' ? 'E' : 'G';
  std::vector<Move> candidates;
  int min_steps = INT_MAX;

  std::queue<Move> to_visit;
  std::vector<std::vector<bool>> visited(grid.size(), 
    std::vector<bool>(grid[0].size(), false));

  to_visit.push(Move(i, j, 0));

  while(!to_visit.empty()) {
    Move curr_move = to_visit.front();
    to_visit.pop();

    if(curr_move.steps > min_steps) continue;

    if(visited[curr_move.i][curr_move.j]) continue;
    visited[curr_move.i][curr_move.j] = true;

    for(auto direction : directions) {
      int next_i = curr_move.i + direction.first, next_j = curr_move.j + direction.second;
      if(grid[next_i][next_j] == enemy) {
        candidates.push_back(curr_move);
        min_steps = curr_move.steps;
      } else if(grid[next_i][next_j] == '.') {
        to_visit.push(Move(next_i, next_j, curr_move.steps + 1));
      }
    }
  }

  if(candidates.size() == 0) return {false, Move(-1, -1, -1)};

  std::sort(candidates.begin(), candidates.end());
  return {true, candidates[0]};
}

int get_distance(int src_i, int src_j, int dst_i, int dst_j,
  const std::vector<std::vector<char>> &grid)
{
  std::queue<Move> to_visit;
  std::vector<std::vector<bool>> visited(grid.size(), 
    std::vector<bool>(grid[0].size(), false));

  to_visit.push(Move(src_i, src_j, 0));

  while(!to_visit.empty()) {
    Move curr_move = to_visit.front();
    to_visit.pop();

    if(visited[curr_move.i][curr_move.j]) continue;
    visited[curr_move.i][curr_move.j] = true;

    if(curr_move.i == dst_i && curr_move.j == dst_j) return curr_move.steps;

    for(auto direction : directions) {
      int next_i = curr_move.i + direction.first, next_j = curr_move.j + direction.second;
      if(!visited[next_i][next_j] && grid[next_i][next_j] == '.') {
        to_visit.push(Move(next_i, next_j, curr_move.steps + 1));
      }
    }
  }

  return -1;
}

std::pair<int, int> get_enemy_to_attack(char type, int i, int j,
  const std::vector<std::vector<char>> &grid,
  const std::vector<Character> &characters)
{
  char enemy = type == 'G' ? 'E' : 'G';
  std::vector<Enemy> enemy_candidates;

  for(const auto &direction : directions) {
    int next_i = i + direction.first, next_j = j + direction.second;
    if(grid[next_i][next_j] == enemy) {
      for(const Character &c : characters) {
        if(c.get_i() == next_i && c.get_j() == next_j) {
          enemy_candidates.emplace_back(next_i, next_j, c.get_hp());
          break;
        }
      }
    }
  }

  std::sort(enemy_candidates.begin(), enemy_candidates.end());
  return {enemy_candidates[0].i, enemy_candidates[0].j};
}

std::pair<int, std::vector<Character>> simulate(
  std::vector<std::vector<char>> grid, 
  std::vector<Character> characters) 
{
  int rounds = 0;
  while(true) {
    std::sort(characters.begin(), characters.end());

    /* Check if there are still targets available */
    std::pair<int, int> character_counts = count_characters(characters);
    if(character_counts.first == 0 || character_counts.second == 0) break;
    rounds++;

    for(size_t index = 0; index < characters.size(); index++) {
      Character &c = characters[index];

      /* No enemies in range, move*/
      if(count_enemies_in_range(c.get_type(), c.get_i(), c.get_j(), grid) == 0) {
        std::pair<bool, Move> result = try_get_next_move(c.get_type(), c.get_i(), c.get_j(), grid);
        
        /* Can't move, continue*/
        if(!result.first) continue;
        Move next_move = result.second;

        /* Try directions in reading order to see which one first brings us closer */
        for(auto direction : directions) {
          int next_i = c.get_i() + direction.first, next_j = c.get_j() + direction.second;
          if(grid[next_i][next_j] != '.') continue;
          int next_steps = get_distance(next_i, next_j, next_move.i, next_move.j, grid);

          /* Move found, move */
          if(next_steps + 1 == next_move.steps) {
            grid[c.get_i()][c.get_j()] = '.';
            c.set_pos(next_i, next_j);
            grid[c.get_i()][c.get_j()] = c.get_type();
          }
        }
      }

      /* Attack phase */
      if(count_enemies_in_range(c.get_type(), c.get_i(), c.get_j(), grid) > 0) {
        std::pair<int, int> enemy_to_attack 
          = get_enemy_to_attack(c.get_type(), c.get_i(), c.get_j(), grid, characters);

        /* Find the correct enemy to attack and attack it */
        for(size_t i = 0; i < characters.size(); i++) {
          if(characters[i].get_i() == enemy_to_attack.first && 
            characters[i].get_j() == enemy_to_attack.second) 
          {
            characters[i].recieve_attack(c.get_power());
            if(characters[i].is_dead()) {
              grid[characters[i].get_i()][characters[i].get_j()] = '.';
              characters.erase(characters.begin() + i);
              if(i < index) index--;
            }
            break;
          }
        }
      }
    }
  }

  return {(rounds - 1), characters};
}

int get_outcome(
  std::vector<std::vector<char>> grid, 
  std::vector<Character> characters)
{
  std::pair<int, std::vector<Character>> outcome = simulate(grid, characters);
  int rem_hp = 0;
  for(const auto &c : outcome.second) {
    rem_hp += c.get_hp();
  }
  return outcome.first * rem_hp;
}

int get_min_elf_power(
  std::vector<std::vector<char>> grid, 
  std::vector<Character> characters) 
{
  int num_of_elves = 0;
  for(const auto &c : characters) {
    if(c.get_type() == 'E') num_of_elves++;
  }

  int curr_power = 3;
  while(true) {
    std::vector<Character> curr_chars = characters;
    for(auto &c : curr_chars) {
      if(c.get_type() == 'E') c.set_power(curr_power);
    }

    auto outcome = simulate(grid, curr_chars);
    int rem_elves = 0;
    for(const auto &c : outcome.second) {
      if(c.get_type() == 'E') rem_elves++;
    }

    if(rem_elves != num_of_elves) {
      curr_power++;
      continue;
    }

    int rem_hp = 0;
    for(const auto &c : outcome.second) {
      rem_hp += c.get_hp();
    }
    return outcome.first * rem_hp;
  }
}

int main() {
  std::ifstream ifstrm("./data/q15.txt", std::ios::in);
  std::vector<std::vector<char>> grid;
  std::vector<Character> characters;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    int row_index = 0;
    while(std::getline(ifstrm, line)) {
      std::vector<char> row;
      for(int col_index = 0; col_index < static_cast<int>(line.size()); col_index++) {
        if(line[col_index] != '\n') {
          row.push_back(line[col_index]);
          if(line[col_index] == 'G' || line[col_index] == 'E') {
            characters.emplace_back(line[col_index], 3, row_index, col_index);
          }
        }
      }
      grid.push_back(row);
      row_index++;
    }
  }

  std::cout << "Answer to part 1: " << get_outcome(grid, characters) << std::endl;
  std::cout << "Answer to part 2: " << get_min_elf_power(grid, characters) << std::endl;
  return 0;
}