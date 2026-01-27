#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef struct Trie_Node {
  Trie_Node(char c, bool is_end_of_word) :
    c(c), is_end_of_word(is_end_of_word) {}
  char c;
  std::unordered_map<char, std::unique_ptr<Trie_Node>> children;
  bool is_end_of_word;
} Trie_Node;

std::vector<std::string> parse_prefixes(std::string &s) {
  std::vector<std::string> prefixes;
  std::string curr_prefix;
  for(char c : s) {
    if(c == ' ') continue;
    if(c == ',') {
      prefixes.push_back(curr_prefix);
      curr_prefix = "";
    } else {
      curr_prefix += c;
    }
  }
  prefixes.push_back(curr_prefix);
  return prefixes;
}

std::unique_ptr<Trie_Node> build_trie(std::vector<std::string> &prefixes) {
  std::unique_ptr<Trie_Node> head = std::make_unique<Trie_Node>(' ',  false);
  Trie_Node *curr_node;

  for(std::string prefix : prefixes) {
    curr_node = head.get();
    for(size_t i = 0; i < prefix.size(); i++) {
      if(curr_node->children.find(prefix[i]) == curr_node->children.end()) {
        curr_node->children.emplace(std::make_pair(prefix[i], 
          std::make_unique<Trie_Node>(prefix[i], false)));
      }
      curr_node = curr_node->children[prefix[i]].get();
      if(i == prefix.size() - 1) curr_node->is_end_of_word = true;
    }
  }

  return head;
}

std::vector<std::string> walk_trie(Trie_Node *node) {
  if(node->children.empty()) {
    if(node->c == ' ') return {};
    std::string s;
    s += node->c;
    return {s};
  }

  std::vector<std::string> words;
  for(std::pair<const char, std::unique_ptr<Trie_Node>> &child : node->children) {
    std::vector<std::string> child_words = walk_trie(child.second.get());
    for(std::string &word : child_words) {
      if(node->c != ' ') word.insert(word.begin(), node->c);
      words.push_back(word);
    }
  }

  if(node->is_end_of_word) {
    std::string new_word;
    new_word += node->c;
    words.push_back(new_word);
  }

  return words;
}

bool is_possible_design(Trie_Node *head, std::string design, std::unordered_set<std::string> &memo) {
  if(design.empty()) return true;

  if(memo.find(design) != memo.end()) return false;

  Trie_Node *curr_node = head;
  for(size_t i = 0; i < design.size(); i++) {
    if(curr_node->children.find(design[i]) == curr_node->children.end()) break;
    curr_node = curr_node->children[design[i]].get();
    if(curr_node->is_end_of_word) {
      if(is_possible_design(head, design.substr(i + 1), memo)) return true;
    }
  }

  memo.insert(design);
  return false;
}

int count_possible_designs(Trie_Node *head, std::vector<std::string> &designs) { 
  int possible_designs = 0;
  for(std::string design : designs) {
    std::unordered_set<std::string> memo;
    if(is_possible_design(head, design, memo)) possible_designs++;
  }
  return possible_designs;
}

uint64_t number_of_ways_for_design(Trie_Node *head, std::string design, std::unordered_map<std::string, uint64_t> &memo) {
  if(design.empty()) return 1;

  if(memo.find(design) != memo.end()) return memo[design];

  Trie_Node *curr_node = head;
  uint64_t number_of_ways = 0;
  for(size_t i = 0; i < design.size(); i++) {
    if(curr_node->children.find(design[i]) == curr_node->children.end()) break;
    curr_node = curr_node->children[design[i]].get();
    if(curr_node->is_end_of_word) {
      number_of_ways += number_of_ways_for_design(head, design.substr(i + 1), memo);
    }
  }

  memo.insert({design, number_of_ways});
  return number_of_ways;
}

uint64_t total_number_of_ways_for_each_design(Trie_Node *head, std::vector<std::string> &designs) { 
  uint64_t possible_designs = 0;
  for(std::string design : designs) {
    std::unordered_map<std::string, uint64_t> memo;
    possible_designs += number_of_ways_for_design(head, design, memo);
  }
  return possible_designs;
}


int main() {
  std::ifstream input("./data/q19.txt");
  std::string line;
  std::getline(input, line);
  std::vector<std::string> prefixes = parse_prefixes(line);

  std::unique_ptr<Trie_Node> head = build_trie(prefixes);
  Trie_Node *head_ref = head.get();
  std::vector<std::string> results = walk_trie(head_ref);

  std::getline(input, line);

  std::vector<std::string> designs;
  while(std::getline(input, line)) {
    designs.push_back(line);
  }

  std::cout << "Answer to part 1: " << count_possible_designs(head_ref, designs) << std::endl;
  std::cout << "Answer to part 2: " << total_number_of_ways_for_each_design(head_ref, designs) << std::endl;
  return 0;
}
