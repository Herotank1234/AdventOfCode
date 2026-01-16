#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

void fill_disk(int *disk, std::string &line) {
  bool fill = true;
  int index = 0;
  int data = 0;

  for(char c : line) {
    int spaces = c - '0';
    if(fill) {
      for(int i = 0; i < spaces; i++) { disk[index++] = data; };
      data++;
    } else {
      index += spaces;
    }

    fill = !fill;
  }
}

unsigned long get_checksum(int *disk, int size) {
  int l_ptr = 0;
  int r_ptr = size - 1;

  while(l_ptr < r_ptr) {
    while(disk[l_ptr] != -1) l_ptr++;
    while(disk[r_ptr] == -1) r_ptr--;
    if(l_ptr > r_ptr) break;

    disk[l_ptr] = disk[r_ptr];
    disk[r_ptr] = -1;
  }

  unsigned long checksum = 0;
  int ptr = 0;
  unsigned long coeff = 0;
  while(disk[ptr] != -1) {
    checksum += (unsigned long) disk[ptr] * coeff;
    ptr++;
    coeff++;
  }

  return checksum;
}

unsigned long get_checksum_without_frag(int *disk, int size) {
  int r_ptr = size - 1;

  while(r_ptr >= 0) {
    while(disk[r_ptr] == -1) r_ptr--;

    int curr_file_size = 0;
    int curr_file_id = disk[r_ptr];
    while(disk[r_ptr] == curr_file_id) {
      curr_file_size++;
      r_ptr--;
    }

    int l_ptr = 0;
    while(l_ptr < r_ptr) {
      while(disk[l_ptr] != -1) l_ptr++;
      if(l_ptr > r_ptr) break;

      int free_space_size = 0;
      while(disk[l_ptr] == -1) {
        free_space_size++;
        l_ptr++;
      }


      if(free_space_size >= curr_file_size) {
        int free_space_ptr = l_ptr - free_space_size;
        int curr_file_ptr = r_ptr + 1;

        for(int i = 0; i < curr_file_size; i++) {
          disk[free_space_ptr++] = curr_file_id;
          disk[curr_file_ptr++] = -1;
        }
        break;
      }
    }
  }

  unsigned long checksum = 0;
  unsigned long coeff = 0;
  for(int i = 0; i < size; i++) {
    if(disk[i] != -1) checksum += coeff * disk[i];
    coeff++;
  }

  return checksum;
}

int main() {
  std::ifstream input("./data/q9.txt");
  std::string line;
  std::getline(input, line);

  int size = 0;
  for(char c : line) { size += c - '0'; }

  int *disk = new int[size];
  std::fill(disk, disk + size, -1);

  fill_disk(disk, line);
  int *disk2 =  new int[size];
  std::copy(disk, disk + size, disk2);

  std::cout << "Answer to part 1: " << get_checksum(disk, size) << std::endl;
  std::cout << "Answer to part 2: " << get_checksum_without_frag(disk2, size) << std::endl;
  return 0;
}