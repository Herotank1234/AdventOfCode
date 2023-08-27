#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>

std::string getJobOrder(const std::vector<std::pair<char, char>> data) {
  std::unordered_map<char, std::vector<char>> childJobs;
  std::unordered_map<char, std::vector<char>> prerequisites;

  for(auto it : data) {
    childJobs[it.first].push_back(it.second);
    prerequisites[it.second].push_back(it.first);
  }

  std::vector<char> toVisit;

  for(auto it : childJobs) {
    if(prerequisites.find(it.first) == prerequisites.end()) {
      toVisit.push_back(it.first);
    }
  }

  char currNode;
  std::unordered_set<char> visited;
  std::string res = "";

  while(toVisit.size() != 0) {
    std::sort(toVisit.begin(), toVisit.end());
    for(auto it = toVisit.begin(); it != toVisit.end(); it++) {
      bool found = true;
      auto preq = prerequisites.find(*it);
      if(preq == prerequisites.end()) {
        currNode = *it;
      } else {
        for(char job : preq->second) {
          if(visited.find(job) == visited.end()) {
            found = false;
            break;
          }
        }
      }
      if(found) {
        currNode = *it;
        toVisit.erase(it);
        break;
      }
    }

    if(visited.find(currNode) != visited.end()) continue;
    visited.insert(currNode);

    res += currNode;

    auto nextJobs = childJobs.find(currNode);
    if(nextJobs != childJobs.end()) {
      for(char job : nextJobs->second) {
        if(visited.find(job) == visited.end()) {
          toVisit.push_back(job);
        }
      }
    }
  }
  return res;
}

bool workersFinished(std::vector<char> workerJobs) {
  for(char c : workerJobs) {
    if(c != '.') {
      return false;
    }
  }
  return true;
}

int getTimeTaken(const std::vector<std::pair<char, char>> data) {
  std::unordered_map<char, std::vector<char>> childJobs;
  std::unordered_map<char, std::vector<char>> prerequisites;

  for(auto it : data) {
    childJobs[it.first].push_back(it.second);
    prerequisites[it.second].push_back(it.first);
  }

  std::vector<char> toVisit;

  for(auto it : childJobs) {
    if(prerequisites.find(it.first) == prerequisites.end()) {
      toVisit.push_back(it.first);
    }
  }

  std::unordered_set<char> visited;
  int time = 0;
  int numWorkers = 5;
  std::vector<int> workerTime(numWorkers, -1);
  std::vector<char> workerJobs(numWorkers, '.');

  while(!workersFinished(workerJobs) || toVisit.size() != 0) {
    for(int i = 0; i < numWorkers; i++) {
      if(workerTime[i] == time) {
        char finishedJob = workerJobs[i];
        visited.insert(finishedJob);
        workerJobs[i] = '.';
        auto nextJobs = childJobs.find(finishedJob);
        if(nextJobs != childJobs.end()) {
          for(char job : nextJobs->second) {
            if(visited.find(job) == visited.end() &&
              std::find(toVisit.begin(), toVisit.end(), job) == toVisit.end()) {
              toVisit.push_back(job);
            }
          }
        }
      }
    }

    std::sort(toVisit.begin(), toVisit.end());
    std::vector<char> availableJobs;
    for(auto it = toVisit.begin(); it != toVisit.end(); it++) {
      bool found = true;
      auto preq = prerequisites.find(*it);
      if(preq != prerequisites.end()) {
        for(char job : preq->second) {
          if(visited.find(job) == visited.end()) {
            found = false;
            break;
          }
        }
      }
      if(found) {
        availableJobs.push_back(*it);
      }
    }
    
    for(int i = 0; i < numWorkers; i++) {
      if(workerJobs[i] == '.' && availableJobs.size() > 0) {
        char currJob = availableJobs[0];
        availableJobs.erase(availableJobs.begin());
        for(auto it = toVisit.begin(); it != toVisit.end(); it++) {
          if(*it == currJob) {
            toVisit.erase(it);
            break;
          }
        }
        workerJobs[i] = currJob;
        workerTime[i] = time + (60 + currJob - 'A' + 1);
      }
    }

    time++;
  }
  return time - 1;
}

int main() {
  std::ifstream ifstrm("./data/q7.txt", std::ios::in);
  std::vector<std::pair<char, char>> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("Step ([A-Z]) must be finished before step ([A-Z]) can begin.[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        data.push_back({sm[1].str()[0], sm[2].str()[0]});
      }
    }
  }

  std::cout << "Answer to part 1: " << getJobOrder(data) << std::endl;
  std::cout << "Answer to part 2: " << getTimeTaken(data) << std::endl;
  return 0;
}