#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

typedef enum Op {
  AND,
  OR,
  XOR
} Op;

typedef struct Wire {
  bool value;
  bool is_set;
} Wire;

typedef struct Gate {
  Gate(std::string l, std::string r, std::string bit_op, std::string out) : left(l), right(r), output(out) {
    if(bit_op == "AND") {
      op = Op::AND;
    } else if(bit_op == "OR") {
      op = Op::OR;
    } else {
      op = Op::XOR;
    }
  }

  std::string left;
  std::string right;
  Op op;
  std::string output;
} Gate;

size_t simulate_and_get_z_val(std::unordered_map<std::string, Wire> wires, const std::vector<Gate> &gates) {
  std::vector<bool> completed(gates.size(), false);

  bool changed;
  do {
    changed = false;
    for(size_t i = 0; i < gates.size(); i++) {
      if(completed[i]) continue;
      
      const Gate &g = gates[i];
      Wire &l = wires[g.left];
      Wire &r = wires[g.right];
      if(!l.is_set || !r.is_set) continue;

      Wire &out = wires[g.output];
      switch(g.op) {
        case Op::AND:
          out.value = l.value & r.value;
          break;
        case Op::OR:
          out.value = l.value | r.value;
          break;
        case Op::XOR:
          out.value = l.value ^ r.value;
          break;
      }
      out.is_set = true;
      completed[i] = true;
      changed = true;
    }
  } while(changed);

  std::vector<std::string> zs;
  for(const auto& wire : wires) {
    if(wire.first[0] == 'z') zs.push_back(wire.first);
  }
  std::sort(zs.begin(), zs.end(), std::greater<>());

  std::stringstream ss;
  for(const std::string &z : zs) {
    ss << static_cast<int>(wires[z].value);
  }

  return std::stoul(ss.str(), nullptr, 2);
}

std::vector<std::string> find_half_adder_swaps(const std::vector<Gate> &gates) {
  std::vector<std::string> swaps;
  std::string l = "x00";
  std::string r = "y00";
  std::string carry = "";

  for(const Gate &g : gates) {
    if(g.left == l && g.op == Op::XOR && g.right == r) {
      if(g.output != "z00") swaps.push_back("z00");
    }
    if(g.left == l && g.op == Op::AND && g.right == r) {
      carry = g.output;
    }
  }

  bool XOR_found = false;
  bool AND_found = false;

  for(const Gate &g : gates) {
    if(g.left == carry || g.right == carry) {
      if(g.op == Op::AND) AND_found = true;
      if(g.op == Op::XOR) XOR_found = true;
    }
  }

  if(!(XOR_found && AND_found)) swaps.push_back(carry);

  return swaps;
}

std::vector<std::string> find_full_adder_swaps(int wire_num, const std::vector<Gate> &gates) {
  std::vector<std::string> swaps;

  std::string l = "x";
  if(wire_num < 10) l += "0";
  l += std::to_string(wire_num);
  std::string r = "y";
  if(wire_num < 10) r += "0";
  r += std::to_string(wire_num);

  std::string initial_XOR = "";
  std::string initial_AND = "";

  for(const Gate &g : gates) {
    if(((g.left == l && g.right == r) || (g.left == r && g.right == l)) && g.op == Op::XOR) {
      initial_XOR = g.output;
    }
    if(((g.left == l && g.right == r) || (g.left == r && g.right == l)) && g.op == Op::AND) {
      initial_AND = g.output;
    }
  }

  bool XOR_found = false;
  bool AND_found = false;
  std::string second_XOR;
  std::string second_AND;

  for(const Gate &g : gates) {
    if((g.left == initial_XOR || g.right == initial_XOR) && g.op == Op::XOR) {
      XOR_found = true;
      second_XOR = g.output;
    }
    if((g.left == initial_XOR || g.right == initial_XOR) && g.op == Op::AND) {
      AND_found = true;
      second_AND = g.output;
    }
  }
  if(!(XOR_found && AND_found)) {
    swaps.push_back(initial_XOR);
  }
  if(XOR_found) {
    if(second_XOR[0] != 'z') swaps.push_back(second_XOR);
  }

  bool OR_found = false;
  std::string initial_OR = "";
  for(const Gate &g : gates) {
    if((g.left == initial_AND || g.right == initial_AND) && g.op == Op::OR) {
      OR_found = true;
      initial_OR = g.output;
      break;
    }
  }

  if(!OR_found) swaps.push_back(initial_AND);

  if(AND_found) {
    OR_found = false;
    for(const Gate &g : gates) {
      if((g.left == second_AND || g.right == second_AND) && g.op == Op::OR) {
        OR_found = true;
      }
    }

    if(!OR_found) swaps.push_back(second_AND);
  }

  if(initial_OR != "") {
    XOR_found = false;
    AND_found = false;
    for(const Gate &g : gates) {
      if(g.left == initial_OR || g.right == initial_OR) {
        if(g.op == Op::AND) AND_found = true;
        if(g.op == Op::XOR) XOR_found = true;
      }
    }
    if(!(XOR_found && AND_found)) swaps.push_back(initial_OR);
  }

  return swaps;
}

std::string find_gate_swaps(const std::vector<Gate> &gates) {
  std::vector<std::string> swaps;

  std::vector<std::string> curr_swaps = find_half_adder_swaps(gates);
  swaps.insert(swaps.end(), curr_swaps.begin(), curr_swaps.end());

  for(int i = 1; i <= 43; i++) {
    curr_swaps = find_full_adder_swaps(i, gates);
    swaps.insert(swaps.end(), curr_swaps.begin(), curr_swaps.end());
  }

  std::sort(swaps.begin(), swaps.end());

  std::stringstream ss;
  for(size_t i = 0; i < swaps.size(); i++) {
    ss << swaps[i];
    if(i != swaps.size() - 1) ss << ',';
  }
  return ss.str();
}

int main() {
  std::ifstream input("./data/q24.txt");
  std::string line;
  std::unordered_map<std::string, Wire> wires;
  std::vector<Gate> gates;

  while(std::getline(input, line)) {
    if(line == "") break;
    std::string name = line.substr(0, 3);
    bool value = (line.back() == '0' ? false : true);
    wires[name] =  {value, true};
  }

  std::regex rgx("([0-9a-z]{3}) (AND|OR|XOR) ([0-9a-z]{3}) -> ([0-9a-z]{3})");
  std::smatch match;
  while(std::getline(input, line)) {
    if(std::regex_match(line, match, rgx)) {
      if(wires.find(match[1]) == wires.end()) wires[match[1]] = {false, false};
      if(wires.find(match[3]) == wires.end()) wires[match[3]] = {false, false};
      if(wires.find(match[4]) == wires.end()) wires[match[4]] = {false, false};
      gates.emplace_back(match[1], match[3], match[2], match[4]);
    }
  }

  std::cout << "Answer to part 1: " << simulate_and_get_z_val(wires, gates) << std::endl;
  std::cout << "Answer to part 2: " << find_gate_swaps(gates) << std::endl;
  return 0;
}