#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <deque>

int simulate(const int noPlayers, const int lastMarble) {
  int currPlayer = 0;
  int marbleNum = 1;
  int currMarbleIndex = 0;
  std::vector<int> board = {0};
  std::vector<int> scores(noPlayers, 0);

  while(marbleNum <= lastMarble) {
    size_t insertIndex;
    if(marbleNum % 23 != 0) {
      insertIndex = currMarbleIndex + 2;
      if(insertIndex > board.size()) insertIndex %= board.size();
      board.insert(board.begin() + insertIndex, marbleNum);
    } else {
      scores[currPlayer] += marbleNum;
      insertIndex = (currMarbleIndex - 7);
      if(insertIndex < 0) insertIndex += board.size();
      insertIndex %= board.size();
      
      scores[currPlayer] += board[insertIndex];
      board.erase(board.begin() + insertIndex);
      if(insertIndex >= board.size()) insertIndex %= board.size();
    }
    currMarbleIndex = insertIndex;

    marbleNum++;
    currPlayer++;
    if(currPlayer == noPlayers) currPlayer = 0;
  }

  int highScore = 0;
  for(int score : scores) {
    if(score > highScore) highScore = score;
  }

  return highScore;
}

long simulateFast(const int noPlayers, const int lastMarble) {
  int currPlayer = 0;
  int marbleNum = 1;
  std::vector<long> scores(noPlayers, 0);
  std::deque<int> board = {0};

  while(marbleNum <= lastMarble) {
    if(marbleNum % 23 != 0) {
      for(int i = 0; i < 2; i++) {
        int curr = board.front();
        board.pop_front();
        board.push_back(curr);
      }
      board.push_front(marbleNum);
    } else {
      for(int i = 0; i < 7; i++) {
        int curr = board.back();
        board.pop_back();
        board.push_front(curr);
      }
      scores[currPlayer] += marbleNum + board.front();
      board.pop_front();
    }
    marbleNum++;
    currPlayer++;
    if(currPlayer == noPlayers) currPlayer = 0;
  }

  long highScore = 0;
  for(long score : scores) {
    if(score > highScore) highScore = score;
  }

  return highScore;
}



int main() {
  std::ifstream ifstrm("./data/q9.txt", std::ios::in);
  int noPlayers;
  int lastMarble;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("([\\d]+) players; last marble is worth ([\\d]+) points");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        noPlayers = stoi(sm[1]);
        lastMarble = stoi(sm[2]);
      }
    }
  }

  std::cout << "Answer to part 1: " << simulate(noPlayers, lastMarble) << std::endl;
  std::cout << "Answer to part 2: " << simulateFast(noPlayers, lastMarble * 100) << std::endl;
  return 0;
}