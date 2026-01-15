#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef struct Safe_Indicator {
  bool is_safe;
  size_t fault_index;
} Safe_Indicator;

Safe_Indicator is_safe(bool is_increasing, std::vector<int> &report) {
  bool valid = true;
  size_t fault_index = 0;

  for(size_t i = 0; i < report.size() - 1; i++) {
    if(is_increasing) {
      if(report[i + 1] <= report[i]) {
        valid = false;
        fault_index = i;
        break;
      }
      int difference = report[i + 1] - report[i];
      if(difference < 1 || difference > 3) {
        valid = false;
        fault_index = i;
        break;
      }
    } else {
      if(report[i + 1] >= report[i]) {
        valid = false;
        fault_index = i;
        break;
      }
      int difference = report[i] - report[i + 1];
      if(difference < 1 || difference > 3) {
        valid = false;
        fault_index = i;
        break;
      }
    }
  }

  return {valid, fault_index};
}

int count_safe_reports(std::vector<std::vector<int>> &reports) {
  int safe_reports = 0;

  for(std::vector<int> report : reports) {
    bool is_increasing = false;
    if(report[0] < report[report.size() - 1]) {
      is_increasing = true;
    }

    Safe_Indicator si = is_safe(is_increasing, report);
    if(si.is_safe) {
      safe_reports++;
    }
  }

  return safe_reports;
}

int count_safe_reports_with_dampener(std::vector<std::vector<int>> &reports) {
  int safe_reports = 0;

  for(std::vector<int> report : reports) {
    bool is_increasing = false;
    if(report[0] < report[report.size() - 1]) {
      is_increasing = true;
    }

    Safe_Indicator si = is_safe(is_increasing, report);
    if(si.is_safe) {
      safe_reports++;
    } else {
      // If not safe, check removing level at fault_index
      std::vector<int> altered_report = report;
      altered_report.erase(altered_report.begin() + si.fault_index);

      Safe_Indicator new_si = is_safe(is_increasing, altered_report);
      if(new_si.is_safe) {
        safe_reports++;
      } else {
        // If still not safe, check removing level at fault_index + 1 (if possible)
        altered_report = report;
        if(si.fault_index < report.size() - 1) {
          altered_report.erase(altered_report.begin() + si.fault_index + 1);

          new_si = is_safe(is_increasing, altered_report);
          if(new_si.is_safe) {
            safe_reports++;
          }
        }
      }
    }
  }

  return safe_reports;
}

int main() {
  std::ifstream input("./data/q2.txt");

  std::vector<std::vector<int>> reports;

  std::string line;
  while(std::getline(input, line)) {
    std::stringstream ss;
    ss << line;

    std::vector<int> report;
    std::string curr_level;
    while(ss >> curr_level) {
      report.push_back(std::stoi(curr_level));
    }

    reports.push_back(report);
  }

  std::cout << "Answer to part 1: " << count_safe_reports(reports) << std::endl;
  std::cout << "Answer to part 2: " << count_safe_reports_with_dampener(reports) << std::endl;
  return 0;
}