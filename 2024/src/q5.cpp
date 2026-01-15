#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <set>
#include <string>
#include <vector>

bool is_valid_update(std::unordered_map<int, std::set<int>> &rules, std::vector<int> update) {
  for(size_t i = 0; i < update.size(); i++) {
    if(rules.find(update[i]) == rules.end()) continue;

    std::set<int> ruleset = rules[update[i]];

    for(size_t j = 0; j < i; j++) {
      if(ruleset.find(update[j]) != ruleset.end()) return false;
    }
  }

  return true;
}

int sum_safe_updates_middle_page(std::unordered_map<int, std::set<int>> &rules, 
  std::vector<std::vector<int>> &updates) 
{
  int middle_pages = 0;

  for(std::vector<int> update : updates) {
    if(is_valid_update(rules, update)) {
      middle_pages += update[update.size() / 2];
    }
  }

  return middle_pages;
}

std::vector<int> fix_update(std::unordered_map<int, std::set<int>> &rules, std::vector<int> update) {

  for(size_t i = 0; i < update.size(); i++) {
    if(rules.find(update[i]) == rules.end()) continue;

    std::set<int> ruleset = rules[update[i]];

    for(size_t j = 0; j < i; j++) {
      if(ruleset.find(update[j]) != ruleset.end()) {
        int move_value = update[i];

        update.erase(update.begin() + i);
        update.insert(update.begin() + j, move_value);
        break;
      }
    }
  }

  return update;
}

int sum_unsafe_updates_middle_page(std::unordered_map<int, std::set<int>> &rules, 
  std::vector<std::vector<int>> &updates) 
{
  int middle_pages = 0;

  for(std::vector<int> update : updates) {
    if(!is_valid_update(rules, update)) {
      std::vector<int> new_update = fix_update(rules, update);
      middle_pages += new_update[new_update.size() / 2];
    }
  }

  return middle_pages;
}

int main() {
  std::ifstream input("./data/q5.txt");
  std::string line;

  std::unordered_map<int, std::set<int>> rules;
  std::vector<std::vector<int>> updates;

  std::regex rgx("([\\d]+)\\|([\\d]+)\r");
  std::smatch match;

  while(std::getline(input, line)) {
    if(line == "\r") break;

    if(std::regex_match(line, match, rgx)) {
      int key = std::stoi(match[1].str());
      int value = std::stoi(match[2].str());
      
      if(rules.find(key) == rules.end()) {
        rules[key] = {value};
      } else {
        rules[key].insert(value);
      }
    } else {
      std::cout << line << " did not match the regex" << std::endl;
    }
  }

  while(std::getline(input, line)) {
    std::stringstream ss;
    ss << line;

    std::vector<int> update;
    std::string page;
    while(std::getline(ss, page, ',')) {
      update.push_back(std::stoi(page));
    }

    updates.push_back(update);
  }
  
  std::cout << "Answer to part 1: " << sum_safe_updates_middle_page(rules, updates) << std::endl;
  std::cout << "Answer to part 2: " << sum_unsafe_updates_middle_page(rules, updates) << std::endl;
  return 0;
}