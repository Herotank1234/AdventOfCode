#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <memory>

typedef struct Elf {
  Elf(int id) : id(id) {}

  void set_next(std::shared_ptr<Elf> next_elf) {
    next = next_elf;
  }

  int id;
  std::shared_ptr<Elf> next = nullptr;
} Elf;

int get_last_elf(int num_of_elves) {
  std::shared_ptr<Elf> root, curr_elf;
  root = std::make_shared<Elf>(1);
  curr_elf = root;

  for(int i = 2; i <= num_of_elves; i++) {
    std::shared_ptr<Elf> next_elf = std::make_shared<Elf>(i);
    curr_elf->set_next(next_elf);
    curr_elf = curr_elf->next;
  }

  curr_elf->set_next(root);
  curr_elf = curr_elf->next;

  while(curr_elf != curr_elf->next) {
    curr_elf->set_next(curr_elf->next->next);
    curr_elf = curr_elf->next;
  }

  return curr_elf->id;
}

int get_last_elf_steal_across(int num_of_elves) {
  std::vector<int> elves;
  for(int i = 1; i <= num_of_elves; i++) {
    elves.push_back(i);
  }

  size_t index = 0;

  while(elves.size() != 1) {
    size_t steal_index = (index + (elves.size() / 2)) % elves.size();
    elves.erase(elves.begin() + steal_index);
    if(steal_index > index) { index++; }
    index = index % elves.size();
  }

  return elves[0];
}

int main() {
  std::ifstream ifstrm("./data/q19.txt", std::ios::in);
  int num_of_elves = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    num_of_elves = std::stoi(line);
  }

  std::cout << "Answer to part 1: " << get_last_elf(num_of_elves) << std::endl;
  std::cout << "Answer to part 2: " << get_last_elf_steal_across(num_of_elves) << std::endl;
  return 0;
}