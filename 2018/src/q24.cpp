#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Group {
  public:
    Group(int id, std::string side, int num_units, int hp, std::vector<std::string> weaknesses, 
      std::vector<std::string> immunities, int dmg, std::string dmg_type, int initiative
    ) :
      _id(id), _side(side), _num_units(num_units), _hp(hp), _weaknesses(weaknesses),
      _immunities(immunities), _dmg(dmg), _dmg_type(dmg_type), _initiative(initiative) {}

  int get_id() const {
    return _id;
  }

  std::string get_side() const {
    return _side;
  }

  int get_units() const {
    return _num_units;
  }

  std::string get_dmg_type() const {
    return _dmg_type;
  }

  int get_initiative() const {
    return _initiative;
  }

  int get_effective_power() const {
    return _num_units * _dmg;
  }

  void boost_dmg(int boost) {
    _dmg += boost;
  }

  int predict_dmg(int dmg, std::string dmg_type) {
    if(std::find(_weaknesses.begin(), _weaknesses.end(), dmg_type) != _weaknesses.end()) 
      return dmg * 2;
    if(std::find(_immunities.begin(), _immunities.end(), dmg_type) != _immunities.end())
      return 0;
    return dmg;
  }

  void take_dmg(int dmg, std::string dmg_type) {
    int total_dmg = dmg;
    if(std::find(_weaknesses.begin(), _weaknesses.end(), dmg_type) != _weaknesses.end()) 
      total_dmg = dmg * 2;
    if(std::find(_immunities.begin(), _immunities.end(), dmg_type) != _immunities.end())
      total_dmg = 0;

    int units_lost = total_dmg / _hp;
    _num_units -= units_lost;
  }

  friend std::ostream& operator<<(std::ostream& os, const Group& g) {
    os << g._num_units << " units each with " << g._hp << " hit points (";
    if(!g._weaknesses.empty()) {
      os << "weak to ";
      for(size_t i = 0; i < g._weaknesses.size() - 1; i++) {
        os << g._weaknesses[i] << ", ";
      }
      os << g._weaknesses[g._weaknesses.size() - 1];
    }
    if(!g._weaknesses.empty() && !g._immunities.empty()) {
      os << "; ";
    }
    if(!g._immunities.empty()) {
      os << "immune to ";
      for(size_t i = 0; i < g._immunities.size() - 1; i++) {
        os << g._immunities[i] << ", ";
      }
      os << g._immunities[g._immunities.size() - 1];
    }
    os << ") with an attack that does " << g._dmg << " " << g._dmg_type << 
      " damage at initiative " << g._initiative;
    return os;
  }

  private:
    int _id;
    std::string _side;
    int _num_units;
    int _hp;
    std::vector<std::string> _weaknesses;
    std::vector<std::string> _immunities;
    int _dmg;
    std::string _dmg_type;
    int _initiative;
};

struct TargetSelectionTurnOrder {
  inline bool operator()(const std::shared_ptr<Group>& a, const std::shared_ptr<Group>& b) {
    if(a->get_effective_power() == b->get_effective_power())
      return a->get_initiative() > b->get_initiative();
    return a->get_effective_power() > b->get_effective_power();
  }
};

struct TargetSelection {
  inline bool operator()(const std::pair<std::shared_ptr<Group>, int>& a,
    const std::pair<std::shared_ptr<Group>, int>& b)
  {
    if(a.second != b.second) return a.second > b.second;
    if(a.first->get_effective_power() != b.first->get_effective_power())
      return a.first->get_effective_power() > b.first->get_effective_power();
    return a.first->get_initiative() > b.first->get_initiative();
  }
};

struct AttackingTurnOrder {
  inline bool operator()(const std::shared_ptr<Group>& a, const std::shared_ptr<Group>& b) {
    return a->get_initiative() > b->get_initiative();
  }
};

typedef struct Armies {
  std::vector<std::shared_ptr<Group>> immune_system, infection;
} Armies;

void parse_affinities(
  std::vector<std::string>& weaknesses, 
  std::vector<std::string>& immunities, 
  std::string affinities)
{
  std::istringstream iss(affinities);
  std::string affinity;

  bool is_weakness = true;

  while(std::getline(iss, affinity, ' ')) {
    if(affinity[affinity.size() - 1] == ',' || affinity[affinity.size() - 1] == ';') {
      affinity = affinity.substr(0, affinity.size() - 1);
    }
    if(affinity == "weak") {
      is_weakness = true;
    } else if(affinity == "immune") {
      is_weakness = false;
    } else if(affinity == "to") {
      continue;
    } else {
      if(is_weakness) {
        weaknesses.push_back(affinity);
      } else {
        immunities.push_back(affinity);
      }
    }
  }
}

Armies get_armies() {
  std::ifstream ifstrm("./data/q24.txt", std::ios::in);
  std::vector<std::shared_ptr<Group>> immune_system;
  std::vector<std::shared_ptr<Group>> infection;

  std::regex match("([\\d]+) units each with ([\\d]+) hit points "
    "\\(?([a-z,; ]*)?\\)? ?"
    "with an attack that does ([\\d]+) ([a-z]+) damage at initiative ([\\d]+)\\s?");
  std::smatch sm;

  bool is_immune_system = true;
  int id = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    while(std::getline(ifstrm, line)) {
      if(line == "\r") {
        std::getline(ifstrm, line);
        is_immune_system = false;
        continue;
      }

      if(std::regex_match(line, sm, match)) {
        int num_units = std::stoi(sm[1]);
        int hp = std::stoi(sm[2]);

        std::vector<std::string> weaknesses;
        std::vector<std::string> immunities;
        parse_affinities(weaknesses, immunities, sm[3].str());

        int dmg = std::stoi(sm[4]);
        std::string dmg_type = sm[5].str();
        int initiative = std::stoi(sm[6]);

        if(is_immune_system) {
          immune_system.push_back(std::move(std::make_shared<Group>(
            id++, "immune", num_units, hp, weaknesses, immunities, dmg, dmg_type, initiative)));
        } else {
          infection.push_back(std::move(std::make_shared<Group>(
            id++, "infection", num_units, hp, weaknesses, immunities, dmg, dmg_type, initiative)));
        }
      } else {
        std::cout << "Unable to match line: " << line << std::endl;
      }
    }
  }

  Armies armies = Armies();
  armies.immune_system = immune_system;
  armies.infection = infection;

  return armies;
}

void remove_group(std::string side, int id, Armies& armies) {
  if(side == "immune") {
    for(size_t d = 0; d < armies.immune_system.size(); d++) {
      if(armies.immune_system[d]->get_id() == id) {
        armies.immune_system.erase(armies.immune_system.begin() + d);
        break;
      }
    }
  } else {
    for(size_t d = 0; d < armies.infection.size(); d++) {
      if(armies.infection[d]->get_id() == id) {
        armies.infection.erase(armies.infection.begin() + d);
        break;
      }
    } 
  }
}

void simulate(Armies& armies) {
  while(!(armies.immune_system.empty() || armies.infection.empty())) {
    std::vector<std::shared_ptr<Group>> turn_order;
    turn_order.insert(turn_order.end(), armies.immune_system.begin(), armies.immune_system.end());
    turn_order.insert(turn_order.end(), armies.infection.begin(), armies.infection.end());

    /* Target Selection Phase */
    std::sort(turn_order.begin(), turn_order.end(), TargetSelectionTurnOrder());
    std::unordered_set<int> selected;
    std::unordered_map<int, int> target_selected;

    for(auto group : turn_order) { 
      std::vector<std::shared_ptr<Group>> enemies
        = group->get_side() == "immune" ? armies.infection : armies.immune_system;
      std::vector<std::pair<std::shared_ptr<Group>, int>> predicted_dmgs;

      for(auto enemy : enemies) {
        if(selected.find(enemy->get_id()) != selected.end()) continue;

        int predicted_dmg = enemy->predict_dmg(group->get_effective_power(), 
          group->get_dmg_type());

        if(predicted_dmg == 0) continue;

        predicted_dmgs.push_back({enemy, predicted_dmg});
      }

      if(predicted_dmgs.empty()) continue;

      std::sort(predicted_dmgs.begin(), predicted_dmgs.end(), TargetSelection());
      selected.insert(predicted_dmgs[0].first->get_id());
      target_selected.insert({group->get_id(), predicted_dmgs[0].first->get_id()});
    }

    if(target_selected.empty()) break;

    // for(auto kv : target_selected) {
    //   std::cout << kv.first << " " << kv.second << std::endl;
    // }

    /* Attacking Phase */
    std::sort(turn_order.begin(), turn_order.end(), AttackingTurnOrder());
    for(auto group : turn_order) {
      if(group->get_units() <= 0) continue;

      auto target = target_selected.find(group->get_id());

      if(target == target_selected.end()) continue;

      int target_id = target->second;
      std::vector<std::shared_ptr<Group>> enemies
        = group->get_side() == "immune" ? armies.infection : armies.immune_system;
      for(auto enemy : enemies) {
        if(enemy->get_id() == target_id) {
          enemy->take_dmg(group->get_effective_power(), group->get_dmg_type());
          break;
        }
      }
    }

    /* Remove dead groups */
    for(size_t i = 0; i < turn_order.size(); i++) {
      if(turn_order[i]->get_units() <= 0) {
        remove_group(turn_order[i]->get_side(), turn_order[i]->get_id(), armies);
      }
    }
    // std::cout << std::endl;
    // for(auto group : armies.immune_system) {
    //   std::cout << group->get_units() << std::endl;
    // }
    // for(auto group : armies.infection) {
    //   std::cout << group->get_units() << std::endl;
    // }
  }
}

int get_total_units_remaining() {
  Armies armies = get_armies();
  simulate(armies);
  int rem_units = 0;
  for(auto group : armies.immune_system) {
    rem_units += group->get_units();
  }
  for(auto group : armies.infection) {
    rem_units += group->get_units();
  }
  return rem_units;
}

int find_min_boost() {
  int boost = 0;
  Armies armies;
  do {
    boost++;
    armies = get_armies();
    for(auto group : armies.immune_system) {
      group->boost_dmg(boost);
    }
    simulate(armies);
  } while(armies.immune_system.empty() || !armies.infection.empty());

  int rem_units = 0;
  for(auto group : armies.immune_system) {
    rem_units += group->get_units();
  }
  return rem_units;
}

int main() {
  std::cout << "Answer to part 1: " << get_total_units_remaining() << std::endl;
  std::cout << "Answer to part 2: " << find_min_boost() << std::endl;
  return 0;
}