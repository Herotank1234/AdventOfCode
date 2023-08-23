#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <deque>
#include <algorithm>
#include <unordered_map>

class DanceMove {
public:
  DanceMove(char dance, int size) : _dance(dance), _size(size) {}

  DanceMove(char dance, int swapStart, int swapEnd) :
    _dance(dance), _swapStart(swapStart), _swapEnd(swapEnd) {}

  DanceMove(char dance, char swapProg1, char swapProg2) :
    _dance(dance), _swapProg1(swapProg1), _swapProg2(swapProg2) {}

  void doMove(std::deque<char>& progs) const {
    switch(_dance) {
      case 's':
        for(int i = 0; i < _size; i++) {
          char temp = progs.back();
          progs.pop_back();
          progs.push_front(temp);
        }
        break;
      case 'x':
        std::swap(progs[_swapStart], progs[_swapEnd]);
        break;
      case 'p':
        {
          size_t index1 = -1, index2 = -1;
          for(size_t i = 0; i < progs.size(); i++) {
            if(progs[i] == _swapProg1) index1 = i;
            if(progs[i] == _swapProg2) index2 = i;
          }
          std::swap(progs[index1], progs[index2]);
          break;
        }
      default:
        std::cout << "No such move: " << _dance << std::endl;
    }
  }

  void print() const {
    switch(_dance) {
      case 's':
        std::cout << _dance << _size << std::endl;
        break;
      case 'x':
        std::cout << _dance << _swapStart << "/" << _swapEnd << std::endl;
        break;
      case 'p':
        std::cout << _dance << _swapProg1 << "/" << _swapProg2 << std::endl;
        break;
      default:
        std::cout << "No such move: " << _dance << std::endl;
    }
  }

private:
  char _dance;
  int _size, _swapStart, _swapEnd;
  char _swapProg1, _swapProg2;
};

std::string toString(const std::deque<char>& progs) {
  std::string res = "";
  for(char c : progs) {
    res += c;
  }
  return res;
}

std::string doDanceMoves(const std::vector<DanceMove>& danceMoves) {
  std::deque<char> progs;
  for(char c = 'a'; c <= 'p'; c++) {
    progs.push_back(c);
  }

  for(const DanceMove& dm : danceMoves) {
    dm.doMove(progs);
  }

  return toString(progs);
}

std::string doDanceMovesBillion(const std::vector<DanceMove>& danceMoves) {
  std::deque<char> progs;
  for(char c = 'a'; c <= 'p'; c++) {
    progs.push_back(c);
  }

  std::unordered_map<std::string, int> seen;
  seen.insert({toString(progs), 0});
  int repeatAt = -1;

  // Check for repeats
  for(int i = 1; i <= 1000000000; i++) {
    for(const DanceMove& dm : danceMoves) {
      dm.doMove(progs);
    }

    // If we have seen this configuration, break;
    auto search = seen.find(toString(progs));
    if(search != seen.end()) {
      repeatAt = i;
      break;
    }
    seen.insert({toString(progs), i});
  }

  int rem = 1000000000 % (repeatAt - seen.find(toString(progs))->second);
  for(int i = 0; i < rem; i++) {
    for(const DanceMove& dm : danceMoves) {
      dm.doMove(progs);
    }
  }
  return toString(progs);
}

int main() {
  std::ifstream ifstrm("./data/q16.txt", std::ios::in);
  std::vector<DanceMove> danceMoves;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    std::regex match("((s)([\\d]+))|((x)([\\d]+)\\/([\\d]+))|((p)([a-p])\\/([a-p]))");
    std::smatch sm;
    while(std::regex_search(line, sm, match)) {
      if(sm[2] != "") {
        danceMoves.emplace_back(sm[2].str()[0], stoi(sm[3]));
      } else if(sm[5] != "") {
        danceMoves.emplace_back(sm[5].str()[0], stoi(sm[6]), stoi(sm[7]));
      } else {
        danceMoves.emplace_back(sm[9].str()[0], sm[10].str()[0], sm[11].str()[0]);
      }
      line = sm.suffix();
    }
  }

  std::cout << "Answer to part 1: " << doDanceMoves(danceMoves) << std::endl;
  std::cout << "Answer to part 2: " << doDanceMovesBillion(danceMoves) << std::endl;
  return 0;
}