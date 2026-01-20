#include <cmath>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

typedef struct Claw_Machine {
  long double a_x;
  long double a_y;
  long double b_x;
  long double b_y;
  long double prize_x;
  long double prize_y;
} Claw_Machine;

void increase(std::vector<Claw_Machine> &cms) {
  for(Claw_Machine &cm : cms) {
    cm.prize_x += 10000000000000;
    cm.prize_y += 10000000000000;
  }
}


uint64_t sum_min_tokens(std::vector<Claw_Machine> &cms) {
  uint64_t min_tokens = 0;

  for(Claw_Machine &cm : cms) {
    long double inv_coeff = 1 / ((cm.a_x * cm.b_y) - (cm.b_x * cm.a_y));
    long double a_press = inv_coeff * ((cm.b_y * cm.prize_x) + (-cm.b_x * cm.prize_y));
    long double b_press = inv_coeff * ((-cm.a_y * cm.prize_x) + (cm.a_x * cm.prize_y));
 
    long double intpart_a;
    long double intpart_b;
    long double frac_a = std::modf(a_press, &intpart_a);
    long double frac_b = std::modf(b_press, &intpart_b);

    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.setf(std::ios::showpoint);
    if((frac_a < 0.00001 || frac_a > 0.99999) && 
      (frac_b < 0.00001 || frac_b > 0.99999)) 
    { 
      min_tokens += (a_press * 3) + b_press;
    }
  }

  return min_tokens;
}

int main() {
  std::ifstream input("./data/q13.txt");
  std::vector<Claw_Machine> cms;

  std::regex rgx_ab("Button [AB]: X\\+([\\d]+), Y\\+([\\d]+)");
  std::regex rgx_prize("Prize: X=([\\d]+), Y=([\\d]+)");
  std::smatch match;

  std::string line;
  while(std::getline(input, line)) {
    std::regex_match(line, match, rgx_ab);
    long double a_x = std::stoi(match[1].str());
    long double a_y = std::stoi(match[2].str());

    std::getline(input, line);
    std::regex_match(line, match, rgx_ab);
    long double b_x = std::stoi(match[1].str());
    long double b_y = std::stoi(match[2].str());

    std::getline(input, line);
    std::regex_match(line, match, rgx_prize);
    long double prize_x = std::stoi(match[1].str());
    long double prize_y = std::stoi(match[2].str());

    std::getline(input, line);

    cms.push_back({a_x, a_y, b_x, b_y, prize_x, prize_y});
  }

  std::vector<Claw_Machine> far_cms = cms;
  increase(far_cms);

  std::cout << "Answer to part 1: " << sum_min_tokens(cms) << std::endl;
  std::cout << "Answer to part 2: " << sum_min_tokens(far_cms) << std::endl;
  return 0;
}