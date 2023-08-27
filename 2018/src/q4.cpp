#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

enum LogType {
  FALL_ASLEEP,
  WAKE_UP,
  BEGIN_SHIFT
};

typedef struct Log {
  std::string date, time, desc;
  int guardId;
  LogType type;
  Log(std::string date1, std::string time1, std::string desc1) : date(date1),
    time(time1), desc(desc1), guardId(-1) {
    if(desc[0] == 'f') {
      type = FALL_ASLEEP;
    } else if(desc[0] == 'w') {
      type = WAKE_UP;
    } else {
      type = BEGIN_SHIFT;
      std::regex match("Guard #([\\d]+) begins shift");
      std::smatch sm;
      if(std::regex_match(desc, sm, match)) {
        guardId = stoi(sm[1]);
      }
    }
  }
} Log;

struct LogSorter {
  inline bool operator() (const Log* log1, Log* log2) {
    if(log1->date == log2->date) return log1->time < log2->time;
    return log1->date < log2->date;
  }
};

typedef struct SleepTracker {
  int minutesAsleep;
  std::vector<int> mins;
  SleepTracker() : minutesAsleep(0) {
    mins = std::vector<int>(60, 0);
  }
} SleepTracker;

int findGuard(std::vector<Log *> data) {
  std::unordered_map<int, SleepTracker *> guardMap;

  sort(data.begin(), data.end(), LogSorter());

  auto it = data.begin();

  while(it != data.end()) {
    int guardId = (*it)->guardId;
    auto search = guardMap.find(guardId);
    SleepTracker* currTracker;
    if(search != guardMap.end()) {
      currTracker = search->second;
    } else {
      currTracker = new SleepTracker();
      guardMap.insert({guardId, currTracker});
    }
    
    it++;
    while(it != data.end() && (*it)->type == FALL_ASLEEP) {
      int startTime = stoi((*it)->time.substr(3));
      it++;
      int endTime = stoi((*it)->time.substr(3));
      it++;
      currTracker->minutesAsleep += (endTime - startTime);
      for(int i = startTime; i < endTime; i++) {
        currTracker->mins[i]++;
      }
    }
  }

  int maxSleepGuardID = 0;
  int maxSleep = 0;
  for(auto guardIt = guardMap.begin(); guardIt != guardMap.end(); guardIt++) {
    if(guardIt->second->minutesAsleep > maxSleep) {
      maxSleepGuardID = guardIt->first;
      maxSleep = guardIt->second->minutesAsleep;
    }
  }
  
  auto search = guardMap.find(maxSleepGuardID);
  auto searchMins = search->second->mins;
  int maxMinute = 0, maxMinSlept = 0;
  for(size_t i = 0; i < searchMins.size(); i++) {
    if(searchMins[i] > maxMinSlept) {
      maxMinSlept = searchMins[i];
      maxMinute = i;
    }
  }

  return maxSleepGuardID * maxMinute;
}

int findGuard2(std::vector<Log *> data) {
  std::unordered_map<int, SleepTracker *> guardMap;

  sort(data.begin(), data.end(), LogSorter());

  auto it = data.begin();

  while(it != data.end()) {
    int guardId = (*it)->guardId;
    auto search = guardMap.find(guardId);
    SleepTracker* currTracker;
    if(search != guardMap.end()) {
      currTracker = search->second;
    } else {
      currTracker = new SleepTracker();
      guardMap.insert({guardId, currTracker});
    }
    
    it++;
    while(it != data.end() && (*it)->type == FALL_ASLEEP) {
      int startTime = stoi((*it)->time.substr(3));
      it++;
      int endTime = stoi((*it)->time.substr(3));
      it++;
      currTracker->minutesAsleep += (endTime - startTime);
      for(int i = startTime; i < endTime; i++) {
        currTracker->mins[i]++;
      }
    }
  }

  int maxGuardId = 0;
  int maxMinute = 0;
  int maxFrequency = 0;
  for(auto iter = guardMap.begin(); iter != guardMap.end(); iter++) {
    auto vec = iter->second->mins;
    for(size_t i = 0; i < vec.size(); i++) {
      if(vec[i] > maxFrequency) {
        maxFrequency = vec[i];
        maxMinute = i;
        maxGuardId = iter->first;
      }
    }
  }
  return maxGuardId * maxMinute;
}


int main() {
  std::ifstream ifstrm("./data/q4.txt", std::ios::in);
  std::vector<Log *> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("\\[([\\d]{4}\\-[\\d]{2}\\-[\\d]{2}) ([\\d]{2}\\:[\\d]{2})\\] (.*)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        Log* log = new Log(sm[1], sm[2], sm[3]);
        data.push_back(log);
      }
    }
  }

  std::cout << "Answer to part 1: " << findGuard(data) << std::endl;
  std::cout << "Answer to part 2: " << findGuard2(data) << std::endl;
  return 0;
}