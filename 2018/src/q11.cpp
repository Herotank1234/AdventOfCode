#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::string getLargestTotalPower(const int data) {
  int gridSize = 300;
  std::vector<std::vector<int>> grid(gridSize, std::vector<int>(gridSize, 0));
  for(int i = 0; i < gridSize; i++) {
    for(int j = 0; j < gridSize; j++) {
      int rackID = (i + 1) + 10;
      int powerLevel = rackID * (j + 1);
      powerLevel += data;
      powerLevel *= rackID;
      std::string stringRep = std::to_string(powerLevel);
      if(stringRep.length() < 3) {
        powerLevel = 0;
      } else {
        powerLevel = stringRep[stringRep.length() - 3] - '0';
      }
      powerLevel -= 5;
      grid[i][j] = powerLevel;
    }
  }

  std::string res = "";
  int largestTotalPower = -1000;
  for(int i = 0; i < gridSize - 2; i++) {
    for(int j = 0; j < gridSize - 2; j++) {
      int power = grid[i][j] + grid[i][j + 1] + grid[i][j + 2] +
        grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j + 2] +
        grid[i + 2][j] + grid[i + 2][j + 1] + grid[i + 2][j + 2];
      if(power > largestTotalPower) {
        largestTotalPower = power;
        res = std::to_string(i + 1) + "," + std::to_string(j + 1);
      }
    }
  }

  return res; 
}

std::string getLargestTotalPowerAnySize(const int data) {
  int gridSize = 300;
  std::vector<std::vector<int>> grid(gridSize, std::vector<int>(gridSize, 0));
  for(int i = 0; i < gridSize; i++) {
    for(int j = 0; j < gridSize; j++) {
      int rackID = (i + 1) + 10;
      int powerLevel = rackID * (j + 1);
      powerLevel += data;
      powerLevel *= rackID;
      std::string stringRep = std::to_string(powerLevel);
      if(stringRep.length() < 3) {
        powerLevel = 0;
      } else {
        powerLevel = stringRep[stringRep.length() - 3] - '0';
      }
      powerLevel -= 5;
      grid[i][j] = powerLevel;
    }
  }

  std::string res = "";
  int largestTotalPower = -1000;
  for(int size = 1; size <= 300; size++) {
    for(int i = 0; i < gridSize - (size - 1); i++) {
      for(int j = 0; j < gridSize - (size - 1); j++) {
        int power = 0;
        for(int y = i; y < i + size; y++) {
          for(int x = j; x < j + size; x++) {
            power += grid[y][x];
          }
        }
        if(power > largestTotalPower) {
          largestTotalPower = power;
          res = std::to_string(i + 1) + "," + std::to_string(j + 1) + "," + std::to_string(size);
        }
      }
    }
  }
  return res; 
}

int main() {
  std::ifstream ifstrm("./data/q11.txt", std::ios::in);
  int data;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      data = stoi(line);
    }
  }

  std::cout << "Answer to part 1: " << getLargestTotalPower(data) << std::endl;
  std::cout << "Answer to part 2: " << getLargestTotalPowerAnySize(data) << std::endl;
  return 0;
}