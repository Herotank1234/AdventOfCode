#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

unsigned long iterate_and_count(std::vector<std::string> stones, int iterations) {
  std::unordered_map<std::string, unsigned long> stones_hash;
  for(std::string stone : stones) stones_hash[stone]++;

  std::unordered_map<std::string, std::vector<std::string>> split_rules;

  for(int i = 0; i < iterations; i++) {
    std::unordered_map<std::string, unsigned long> next_stones_hash;

    for(std::pair<std::string, unsigned long> stone_kv : stones_hash) {
      if(stone_kv.first == "0") {
        next_stones_hash["1"] += stone_kv.second;

      } else if(!(stone_kv.first.size() % 2)) {
        std::string left = stone_kv.first.substr(0, stone_kv.first.size() / 2);
        std::string right = stone_kv.first.substr(stone_kv.first.size() / 2);
        while(right.size() > 1 && right[0] == '0') right.erase(right.begin());

        next_stones_hash[left] += stone_kv.second;
        next_stones_hash[right] += stone_kv.second;
      } else {
        unsigned long next_stone = std::stoul(stone_kv.first) * 2024;
        next_stones_hash[std::to_string(next_stone)] += stone_kv.second;
      }
    }
    stones_hash = next_stones_hash;
  }

  unsigned long total_stones = 0;
  for(std::pair<std::string, unsigned long> kv : stones_hash) {
    total_stones += kv.second;
  }

  return total_stones;
}

int main() {
  std::ifstream input("./data/q11.txt");
  std::vector<std::string> stones;

  std::string line;
  std::getline(input, line);
  
  std::stringstream ss;
  ss << line;

  std::string stone;
  while(std::getline(ss, stone, ' ')) stones.push_back(stone);

  std::cout << "Answer to part 1: " << iterate_and_count(stones, 25) << std::endl;
  std::cout << "Answer to part 2: " << iterate_and_count(stones, 75) << std::endl;
  return 0;
}