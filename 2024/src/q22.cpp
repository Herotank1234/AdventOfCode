#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Secret_Number {
  public:
    Secret_Number(size_t s_num) : _s_num(s_num) {
      _digit = _s_num % 10;
    }

    void next() {
      mix(_s_num << 6);
      prune();
      mix(_s_num >> 5);
      prune();
      mix(_s_num << 11);
      prune();

      int new_digit = _s_num % 10;
      _diffs.push_back(new_digit - _digit);
      _digit = new_digit;

      if(_diffs.size() >= 4) {
        if(_diffs.size() > 4) _diffs.erase(_diffs.begin());

        std::string hash;
        std::stringstream ss;
        ss << _diffs[0] << ',' << _diffs[1] << ',' << _diffs[2] << ',' << _diffs[3];
        hash = ss.str();

        if(_seq_to_price.find(hash) == _seq_to_price.end()) {
          _seq_to_price[hash] = _digit;
        }
      }
    }

    size_t get_s_num() {
      return _s_num;
    }

    std::unordered_map<std::string, int> get_seq_to_price() {
      return _seq_to_price;
    }

  private:
    void mix(size_t num) {
      _s_num ^= num;
    }

    void prune() {
      _s_num %= 16777216; 
    }

    size_t _s_num;
    std::vector<int> _diffs;
    int _digit = 0;
    std::unordered_map<std::string, int> _seq_to_price;
};

size_t sum_2000th_secret_number(std::vector<Secret_Number> &s_nums) {
  size_t sum = 0;
  for(Secret_Number &s : s_nums) {
    for(size_t _ = 0; _ < 2000; _++) {
      s.next();
    }
    sum += s.get_s_num();
  }
  return sum;
}

size_t find_optimal_price(std::vector<Secret_Number> &s_nums) {
  std::unordered_map<std::string, int> _seq_to_total_price;
  for(Secret_Number &s : s_nums) {
    const std::unordered_map<std::string, int> &curr_seq = s.get_seq_to_price();
    for(const auto &seq_price : curr_seq) {
      _seq_to_total_price[seq_price.first] += seq_price.second;
    }
  }

  int optimal_price = 0;
  for(const auto &seq_total_price : _seq_to_total_price) {
    optimal_price = std::max(optimal_price, seq_total_price.second);
  }
  return optimal_price;
}

int main() {
  std::ifstream input("./data/q22.txt");
  std::string line;
  std::vector<Secret_Number> s_nums;

  while(std::getline(input, line)) {
    s_nums.emplace_back(std::stoul(line));
  }

  std::cout << "Answer to part 1: " << sum_2000th_secret_number(s_nums) << std::endl;
  std::cout << "Answer to part 2: " << find_optimal_price(s_nums) << std::endl;
  return 0;
}