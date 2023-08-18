#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <cassert>

typedef struct Room {
  Room(std::string name1, int id1, std::string checksum1) : 
    name(name1), id(id1), checksum(checksum1) {}
  
  void print() const {
    std::cout << name << "-" << id << "[" << checksum << "]" << std::endl;
  }

  std::string name;
  int id;
  std::string checksum;
} Room;

bool CharFreqComparator(const std::pair<char, int>&a, const std::pair<char, int>& b) {
  if(a.second == b.second) return a.first < b.first;
  return a.second > b.second;
}

int sumRealSectorIDs(const std::vector<Room>& rooms) {
  int sum = 0;
  for(const Room& r : rooms) {
    std::unordered_map<char, int> charFreq;
    for(char c : r.name) {
      if(c != '-') charFreq[c]++;
    }

    std::vector<std::pair<char, int>> charFreqV(charFreq.begin(), charFreq.end());
    std::sort(charFreqV.begin(), charFreqV.end(), CharFreqComparator);

    assert(charFreqV.size() >= 5);

    std::string check = "";
    for(size_t i = 0; i < 5; i++) {
      check += charFreqV[i].first;
    }

    if(check == r.checksum) sum += r.id;
  }

  return sum;
}

int findNorthPoleObjects(const std::vector<Room>& rooms) {
  for(const Room& r : rooms) {
    std::string roomName = "";
    for(char c : r.name) {
      if (c == '-') {
        roomName += ' ';
      } else {
        roomName += (char) (((c - 'a' + (r.id % 26)) % 26) + 'a');
      }
    }
    if(roomName == "northpole object storage") return r.id;
  }
  return -1;
}

int main() {
  std::ifstream ifstrm("./data/q4.txt", std::ios::in);
  std::vector<Room> rooms;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::regex match("((?:[a-z]+)(?:-[a-z]+)*)-([\\d]+)\\[([a-z]{5})\\][\\s]?");
    std::smatch sm;
    std::string line;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        rooms.emplace_back(sm[1], stoi(sm[2]), sm[3]);
      } else {
        std::cout << "No match: " << line << std::endl;
      }
    }
  }

  std::cout << "Answer to part 1: " << sumRealSectorIDs(rooms) << std::endl;
  std::cout << "Answer to part 2: " << findNorthPoleObjects(rooms) << std::endl;
  return 0;
}