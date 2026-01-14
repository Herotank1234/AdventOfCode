#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int get_min_dist(std::vector<int> &left_list, std::vector<int> &right_list) {
  int min_dist = 0;
  
  for(size_t i = 0; i < left_list.size(); i++) {
    min_dist += std::abs(left_list[i] - right_list[i]);
  }

  return min_dist;
}

int get_similarity_score(std::vector<int> &left_list, std::vector<int> &right_list) {
  int similarity_score = 0;

  std::map<int, int> frequency_count;
  for(int i : right_list) {
    if(frequency_count.find(i) == frequency_count.end()) {
      frequency_count[i] = 1;
    } else {
      frequency_count[i] += 1;
    }
  }

  for(int i : left_list) {
    if(frequency_count.find(i) != frequency_count.end()) {
      similarity_score += i * frequency_count[i];
    }
  }

  return similarity_score;
}

int main() {
  std::ifstream input("./data/q1.txt");

  std::vector<int> left_list;
  std::vector<int> right_list;

  std::string line;

  while(std::getline(input, line)) {
    std::stringstream ss;
    ss << line;

    std::string left;
    std::string right;
    ss >> left >> right;

    left_list.push_back(std::stoi(left));
    right_list.push_back(std::stoi(right));
  }

  std::sort(left_list.begin(), left_list.end());
  std::sort(right_list.begin(), right_list.end());

  std::cout << "Answer to part 1: " << get_min_dist(left_list, right_list) << std::endl;
  std::cout << "Answer to part 2: " << get_similarity_score(left_list, right_list) << std::endl;
  return 0;
}