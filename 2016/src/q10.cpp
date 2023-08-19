#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <climits>
#include <cassert>
#include <queue>

typedef struct Bot {
  void recieve(int value) {
    assert(value1 == INT_MAX || value2 == INT_MAX);

    if(value1 == INT_MAX) {
      value1 = value;
    } else {
      value2 = value;
    }
  }

  int get_lower_value() {
    return (value1 < value2 ? value1 : value2);
  }

  int get_upper_value() {
    return (value1 > value2 ? value1 : value2);
  }

  void reset() {
    value1 = INT_MAX;
    value2 = INT_MAX;
  }

  bool has_two_values() {
    return value1 != INT_MAX && value2 != INT_MAX;
  }

  int value1 = INT_MAX;
  int value2 = INT_MAX;
} Bot;

typedef struct Rule {
  Rule(std::string l_type, int l_val, std::string u_type, int u_val) :
    lower_type(l_type), lower_index(l_val), upper_type(u_type), upper_index(u_val) {}

  std::string lower_type;
  int lower_index;
  std::string upper_type;
  int upper_index;
} Rule;

int propagate(
  const std::unordered_map<int, Rule> &rules,
  const std::vector<std::pair<int, int>> &inputs) 
{
  int num_of_bots = 0;
  int num_of_outputs = 0;
  for(auto rule : rules) {
    if(rule.first > num_of_bots) num_of_bots = rule.first;
    if(rule.second.lower_type == "bot") {
      num_of_bots = std::max(num_of_bots, rule.second.lower_index);
    } else {
      num_of_outputs = std::max(num_of_outputs, rule.second.lower_index);
    }
    if(rule.second.upper_type == "bot") {
      num_of_bots = std::max(num_of_bots, rule.second.upper_index);
    } else {
      num_of_outputs = std::max(num_of_outputs, rule.second.upper_index);
    }
  }
  
  std::vector<Bot> bots(num_of_bots + 1, Bot());
  std::vector<int> outputs(num_of_outputs + 1, 0);
  std::queue<int> to_visit;

  for(auto input : inputs) {
    bots[input.second].recieve(input.first);
    if(bots[input.second].has_two_values()) to_visit.push(input.second);
  }

  while(!to_visit.empty()) {
    int curr_index = to_visit.front();
    to_visit.pop();

    Rule curr_rule = rules.find(curr_index)->second;

    int lower_value = bots[curr_index].get_lower_value();
    int upper_value = bots[curr_index].get_upper_value();

    if(lower_value == 17 && upper_value == 61) {
      return curr_index;
    }
    bots[curr_index].reset();

    if(curr_rule.lower_type == "bot") {
      bots[curr_rule.lower_index].recieve(lower_value);
      if(bots[curr_rule.lower_index].has_two_values()) to_visit.push(curr_rule.lower_index);
    } else {
      outputs[curr_rule.lower_index] = lower_value;
    }

    if(curr_rule.upper_type == "bot") {
      bots[curr_rule.upper_index].recieve(upper_value);
      if(bots[curr_rule.upper_index].has_two_values()) to_visit.push(curr_rule.upper_index);
    } else {
      outputs[curr_rule.upper_index] = upper_value;
    }
  }

  return -1;
}

int propagate_till_end(
  const std::unordered_map<int, Rule> &rules,
  const std::vector<std::pair<int, int>> &inputs) 
{
  int num_of_bots = 0;
  int num_of_outputs = 0;
  for(auto rule : rules) {
    if(rule.first > num_of_bots) num_of_bots = rule.first;
    if(rule.second.lower_type == "bot") {
      num_of_bots = std::max(num_of_bots, rule.second.lower_index);
    } else {
      num_of_outputs = std::max(num_of_outputs, rule.second.lower_index);
    }
    if(rule.second.upper_type == "bot") {
      num_of_bots = std::max(num_of_bots, rule.second.upper_index);
    } else {
      num_of_outputs = std::max(num_of_outputs, rule.second.upper_index);
    }
  }
  
  std::vector<Bot> bots(num_of_bots + 1, Bot());
  std::vector<int> outputs(num_of_outputs + 1, -1);
  std::queue<int> to_visit;

  for(auto input : inputs) {
    bots[input.second].recieve(input.first);
    if(bots[input.second].has_two_values()) to_visit.push(input.second);
  }

  while(!to_visit.empty()) {
    std::queue<int> next_to_visit;
    while(!to_visit.empty()) {
      int curr_index = to_visit.front();
      to_visit.pop();

      Rule curr_rule = rules.find(curr_index)->second;

      int lower_value = bots[curr_index].get_lower_value();
      int upper_value = bots[curr_index].get_upper_value();
      bots[curr_index].reset();

      if(curr_rule.lower_type == "bot") {
        bots[curr_rule.lower_index].recieve(lower_value);
        if(bots[curr_rule.lower_index].has_two_values()) {
          next_to_visit.push(curr_rule.lower_index);
        }
      } else {
        outputs[curr_rule.lower_index] = lower_value;
      }

      if(curr_rule.upper_type == "bot") {
        bots[curr_rule.upper_index].recieve(upper_value);
        if(bots[curr_rule.upper_index].has_two_values()) 
          next_to_visit.push(curr_rule.upper_index);
      } else {
        outputs[curr_rule.upper_index] = upper_value;
      }
    }
    to_visit = next_to_visit;
  }

  return outputs[0] * outputs[1] * outputs[2];
}

int main() {
  std::ifstream ifstrm("./data/q10.txt", std::ios::in);
  std::unordered_map<int, Rule> rules;
  std::vector<std::pair<int, int>> inputs;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex value_input("value ([\\d]+) goes to bot ([\\d]+)\\s?");
    std::regex rule("bot ([\\d]+) gives low to (output|bot) ([\\d]+) and high to (output|bot) ([\\d]+)\\s?");
    std::smatch sm;

    while(std::getline(ifstrm, line)) {
      if(line.substr(0, 3) == "bot") {
        if(std::regex_match(line, sm, rule)) {
          rules.insert({std::stoi(sm[1]), 
            Rule(sm[2], std::stoi(sm[3]), sm[4], std::stoi(sm[5]))});
        } else {
          std::cout << "Was unable to match line: " << line << std::endl;
        }
      } else {
        if(std::regex_match(line, sm, value_input)) {
          inputs.push_back({std::stoi(sm[1]), std::stoi(sm[2])});
        }
      }
    }
  }

  std::cout << "Answer to part 1: " << propagate(rules, inputs) << std::endl;
  std::cout << "Answer to part 2: " << propagate_till_end(rules, inputs) << std::endl;
  return 0;
}