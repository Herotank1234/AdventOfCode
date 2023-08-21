#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

typedef struct Range {
  Range(u_int32_t lower, uint32_t upper) : lower_bound(lower), upper_bound(upper) {}

  u_int32_t lower_bound;
  u_int32_t upper_bound;

  bool operator<(const Range& other) {
    if(lower_bound == other.lower_bound) { return upper_bound < other.upper_bound; }
    return lower_bound < other.lower_bound;
  }

  friend std::ostream &operator<<(std::ostream &os, const Range& r) {
    return os << r.lower_bound << "-" << r.upper_bound;
  }
} Range;

uint32_t get_lowest_ip(std::vector<Range> ranges) {
  while(ranges.size() >= 1 && ranges[0].upper_bound >= ranges[1].lower_bound - 1) {
    ranges[0].upper_bound = std::max(ranges[0].upper_bound, ranges[1].upper_bound);
    ranges.erase(ranges.begin() + 1);
  }
  return ranges[0].upper_bound + 1;
}

uint32_t count_valid_ips(std::vector<Range> ranges) {
  size_t index = 0;
  while(index < ranges.size()) {
    while(index < ranges.size() - 1 && 
      ranges[index].upper_bound >= ranges[index + 1].lower_bound - 1)
    {
      ranges[index].upper_bound 
        = std::max(ranges[index].upper_bound, ranges[index + 1].upper_bound);
      ranges.erase(ranges.begin() + index + 1);
    }
    index++;
  }

  uint32_t valid_ips = 0;
  for(size_t i = 0; i < ranges.size() - 1; i++) {
    valid_ips += (ranges[i + 1].lower_bound - ranges[i].upper_bound - 1);
  }
  valid_ips += (UINT32_MAX - ranges[ranges.size() - 1].upper_bound);

  return valid_ips;
}

int main() {
  std::ifstream ifstrm("./data/q20.txt", std::ios::in);
  std::vector<Range> ranges;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::regex match("([\\d]+)-([\\d]+)\\s?");
    std::smatch sm;
    while(std::getline(ifstrm, line)) {
      if(std::regex_match(line, sm, match)) {
        ranges.emplace_back(std::stoul(sm[1]), std::stoul(sm[2]));
      }
    }
  }

  std::sort(ranges.begin(), ranges.end());

  std::cout << "Answer to part 1: " << get_lowest_ip(ranges) << std::endl;
  std::cout << "Answer to part 2: " << count_valid_ips(ranges) << std::endl;
  return 0;
}