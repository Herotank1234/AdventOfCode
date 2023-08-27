#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

int getNodeSum(const std::vector<int> data, int& index) {
  int sum = 0;
  int subNodes = data[index++];
  int entries = data[index++];

  for(int i = 0; i < subNodes; i++) {
    sum += getNodeSum(data, index);
  }

  for(int i = 0; i < entries; i++) {
    sum += data[index++];
  }

  return sum;
}

int getMetadataSum(const std::vector<int> data) {
  int index = 0;
  return getNodeSum(data, index);
}

int calcNodeValue(const std::vector<int> data, int& index) {
  int subNodes = data[index++];
  int entries = data[index++];

  int val = 0;

  if(subNodes == 0) {
    for(int i = 0; i < entries; i++) {
      val += data[index++];
    }
  } else {
    std::vector<int> nodeValues;
    for(int i = 0; i < subNodes; i++) {
      nodeValues.push_back(calcNodeValue(data, index));
    }
    for(int i = 0; i < entries; i++) {
      size_t nodeIndex = data[index++] - 1;
      if(nodeIndex >= 0 && nodeIndex < nodeValues.size()) {
        val += nodeValues[nodeIndex];
      }
    }
  }

  return val;
}

int getNodeValue(const std::vector<int> data) {
  int index = 0;
  return calcNodeValue(data, index);
}

int main() {
  std::ifstream ifstrm("./data/q8.txt", std::ios::in);
  std::vector<int> data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("([\\d]+)[\\t]?");
      std::smatch sm;
      while(std::regex_search(line, sm, match)) {
        data.push_back(stoi(sm.str()));
        line = sm.suffix();
      }
    }
  }

  std::cout << "Answer to part 1: " << getMetadataSum(data) << std::endl;
  std::cout << "Answer to part 2: " << getNodeValue(data) << std::endl;
  return 0;
}