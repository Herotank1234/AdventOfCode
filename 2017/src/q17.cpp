#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>

class Spinlock {
public:
  Spinlock(int step) : _currVal(1), _step(step) {
    _values.push_back(0);
  }

  void insert() {
    for(int i = 0; i <= _step; i++) {
      int temp = _values.front();
      _values.pop_front();
      _values.push_back(temp);
    }
    _values.push_front(_currVal++);
  }

  void print() {
    for(int i : _values) {
      std::cout << i <<  " ";
    }
    std::cout << std::endl;
  }

  int getSecondValue() {
    return _values[1];
  }

  int getValueAfterZero() {
    for(size_t i = 0; i < _values.size(); i++) {
      if(_values[i] == 0) {
        if(i + 1 < _values.size()) {
          return _values[i + 1];
        } else {
          return _values[0];
        }
      }
    }
    return -1;
  }

private:
  std::deque<int> _values;
  int _currVal, _step;
};

int runSpin(Spinlock s) {
  for(int i = 0; i < 2017; i++) {
    s.insert();
  }
  return s.getSecondValue();
}

int runSpinLarge(Spinlock s) {
  for(int i = 0; i < 50000000; i++) {
    s.insert();
  }
  return s.getValueAfterZero();
}

int main() {
  std::ifstream ifstrm("./data/q17.txt", std::ios::in);
  int step;
  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    step = stoi(line);
  }

  Spinlock s(step);

  std::cout << "Answer to part 1: " << runSpin(s) << std::endl;
  std::cout << "Answer to part 2: " << runSpinLarge(s) << std::endl;
  return 0;
}