#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

std::string findPassword(const std::string& doorID) {
  std::string password = "";
  int counter = 0;
  while(password.length() < 8) {
    std::string currStr = doorID + std::to_string(counter);
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5((unsigned char*) currStr.c_str(), currStr.size(), hash);

    std::ostringstream sout;
    sout << std::hex << std::setfill('0');
    for(long long c : hash)
    {
        sout << std::setw(2) << (long long) c;
    }
    std::string hashStr = sout.str();
    if(hashStr[0] == '0' && hashStr[1] == '0' && hashStr[2] == '0' &&
      hashStr[3] == '0' && hashStr[4] == '0') {
      password += hashStr[5];
    }
    counter++;
  }

  return password;
}

bool containsZ(const std::string& password) {
  for(char c : password) {
    if(c == 'z') return true;
  }
  return false;
}

std::string findPasswordWithPos(const std::string& doorID) {
  std::string password = "zzzzzzzz";
  int counter = 0;
  while(containsZ(password)) {
    std::string currStr = doorID + std::to_string(counter);
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5((unsigned char*) currStr.c_str(), currStr.size(), hash);

    std::ostringstream sout;
    sout << std::hex << std::setfill('0');
    for(long long c : hash)
    {
        sout << std::setw(2) << (long long) c;
    }
    std::string hashStr = sout.str();
    if(hashStr[0] == '0' && hashStr[1] == '0' && hashStr[2] == '0' &&
      hashStr[3] == '0' && hashStr[4] == '0') {
      if(std::isdigit(hashStr[5]) && (int) (hashStr[5] - '0') < 8 &&
        password[(int) (hashStr[5] - '0')] == 'z') {
        password[(int) (hashStr[5] - '0')] = hashStr[6];
      }
    }
    counter++;
  }

  return password;
}

int main() {
  std::ifstream ifstrm("./data/q5.txt", std::ios::in);
  std::string doorID;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    doorID = line;
  }

  std::cout << "Answer to part 1: " << findPassword(doorID) << std::endl;
  std::cout << "Answer to part 2: " << findPasswordWithPos(doorID) << std::endl;
  return 0;
}