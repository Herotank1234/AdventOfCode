#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

bool is_connected_to_all(const std::vector<std::string> &network, const std::vector<std::string> &neighbours) {
  for(const std::string &node : network) {
    if(std::find(neighbours.begin(), neighbours.end(), node) == neighbours.end()) return false;
  }
  return true;
}

std::unordered_set<std::string> get_all_three_linked_comps(
  const std::unordered_map<std::string, std::vector<std::string>> &adj_list)
{
  std::unordered_set<std::string> three_linked_comps;
  std::queue<std::vector<std::string>> networks_to_be_checked;
  for(const auto &kv : adj_list) {
    networks_to_be_checked.push({kv.first});
  }

  while(!networks_to_be_checked.empty()) {
    std::vector<std::string> network = networks_to_be_checked.front();
    networks_to_be_checked.pop();

    if(network.size() == 3) {
      std::sort(network.begin(), network.end());
      std::string hash = network[0] + ',' + network[1] + ',' + network[2];
      three_linked_comps.insert(hash);
      continue;
    }

    const std::vector<std::string> &neighbours = adj_list.find(network.back())->second;
    for(const std::string &neighbour : neighbours) {
      if(std::find(network.begin(), network.end(), neighbour) == network.end() &&
        is_connected_to_all(network, adj_list.find(neighbour)->second)) 
      {
        std::vector<std::string> next_network = network;
        next_network.push_back(neighbour);
        networks_to_be_checked.push(next_network);
      }
    }
  }

  return three_linked_comps;
}

std::string get_largest_connected_network(
  const std::unordered_map<std::string, std::vector<std::string>> &adj_list)
{
  std::queue<std::vector<std::string>> networks_to_be_checked;
  for(const auto &kv : adj_list) {
    networks_to_be_checked.push({kv.first});
  }

  std::unordered_set<std::string> checked;

  while(networks_to_be_checked.size() != 1) {
    std::vector<std::string> network = networks_to_be_checked.front();
    networks_to_be_checked.pop();

    std::sort(network.begin(), network.end());
    std::stringstream hash_ss;
    for(size_t i = 0; i < network.size(); i++) {
      hash_ss << network[i];
      if(i != network.size() - 1) hash_ss << ',';
    }

    std::string hash = hash_ss.str();
    if(checked.find(hash) != checked.end()) continue;
    checked.insert(hash);

    const std::vector<std::string> &neighbours = adj_list.find(network.back())->second;
    for(const std::string &neighbour : neighbours) {
      if(std::find(network.begin(), network.end(), neighbour) == network.end() &&
        is_connected_to_all(network, adj_list.find(neighbour)->second)) 
      {
        std::vector<std::string> next_network = network;
        next_network.push_back(neighbour);
        networks_to_be_checked.push(next_network);
      }
    }
  }

  std::vector<std::string> largest_network = networks_to_be_checked.front();
  networks_to_be_checked.pop();

  std::sort(largest_network.begin(), largest_network.end());
  std::stringstream password_ss;
  for(size_t i = 0; i < largest_network.size(); i++) {
    password_ss << largest_network[i];
    if(i != largest_network.size() - 1) password_ss << ',';
  }
  std::string password = password_ss.str();

  return password; 
}


bool network_contains_t_comp(const std::string &network) {
  return network[0] == 't' || network[3] == 't' || network[6] == 't';
}

size_t count_network_with_t_comp(const std::unordered_map<std::string, std::vector<std::string>> &adj_list) {
  std::unordered_set<std::string> three_linked_comps = get_all_three_linked_comps(adj_list);

  size_t network_with_t_comp = 0;
  for(const std::string &network : three_linked_comps) {
    if(network_contains_t_comp(network)) network_with_t_comp++;
  }
  return network_with_t_comp;
}

int main() {
  std::ifstream input("./data/q23.txt");
  std::string line;
  std::unordered_map<std::string, std::vector<std::string>> adj_list;

  while(std::getline(input, line)) {
    std::string front = line.substr(0, 2);
    std::string back = line.substr(3);
    adj_list[front].push_back(back);
    adj_list[back].push_back(front);
  }
  std::vector<int> b = {1};
  std::cout << "Answer to part 1: " << count_network_with_t_comp(adj_list) << std::endl;
  std::cout << "Answer to part 2: " << get_largest_connected_network(adj_list) << std::endl;
  return 0;
}