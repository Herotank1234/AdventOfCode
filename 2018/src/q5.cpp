#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <climits>

int collapsePolymer(std::string data) {
  std::string curr = data;
  std::string next = "";
  int changes = -1;
  while(changes != 0) {
    changes = 0;
    for(size_t i = 0; i < curr.length(); i++) {
      if(i < curr.length() - 1 && (abs(curr[i] - curr[i + 1]) == 32)) {
        changes++;
        i++;
      } else {
        next += curr[i];
      }
    }
    curr = next;
    next = "";
  }

  return curr.length();
}

int removeAndCollapse(std::string data) {
  int minLength = INT_MAX;
  for(char i = 'A'; i <= 'Z'; i++) {
    std::string currData = data;
    std::string removedPolymer = "";
    for(char c : currData) {
      if(c != i && c != (i + 32)) {
        removedPolymer += c;
      }
    }  
    int length = collapsePolymer(removedPolymer);
    if(length < minLength) {
      minLength = length;
    }
  }
  
  return minLength;
}

int main() {
  std::ifstream ifstrm("./data/q5.txt", std::ios::in);
  std::string data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      data = line;
    }
  }

  std::cout << "Answer to part 1: " << collapsePolymer(data) << std::endl;
  std::cout << "Answer to part 2: " << removeAndCollapse(data) << std::endl;
  return 0;
}