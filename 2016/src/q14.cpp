#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <functional>

typedef struct Hash {
  Hash(int index, char three, std::unordered_set<char> fives) :
    index(index), three(three), fives(fives) {}

  int index;
  char three;
  std::unordered_set<char> fives;
} Hash;

inline bool same_three_letters(std::string str, size_t i) {
  return str[i] == str[i + 1] && str[i] == str[i + 2];
}

inline bool same_five_letters(std::string str, size_t i) {
  return str[i] == str[i + 1] && str[i] == str[i + 2] && str[i] == str[i + 3] && 
    str[i] == str[i + 4];
}

Hash get_hash(std::string salt, int index) {
  std::string str = salt + std::to_string(index);
  unsigned char result[MD5_DIGEST_LENGTH];
  MD5((unsigned char*) str.c_str(), str.size(), result);

  std::stringstream ss;
  for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
  }
  
  std::string md5_str = ss.str();
  char three = '@';
  std::unordered_set<char> fives;
  
  for(size_t i = 0; i < md5_str.size() - 2; i++) {
    if(i < md5_str.size() - 4) {
      if(same_five_letters(md5_str, i)) {
        fives.insert(md5_str[i]);
      }
    }
    if(three == '@' && same_three_letters(md5_str, i)) {
      three = md5_str[i];
    }
  }

  return Hash(index, three, fives);
}

Hash get_stretched_hash(std::string salt, int index) {
  std::string str = salt + std::to_string(index);

  for(int i = 0; i < 2017; i++) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*) str.c_str(), str.size(), result);

    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    str = ss.str();
  }

  std::string md5_str = str;
  char three = '@';
  std::unordered_set<char> fives;
  
  for(size_t i = 0; i < md5_str.size() - 2; i++) {
    if(i < md5_str.size() - 4) {
      if(same_five_letters(md5_str, i)) {
        fives.insert(md5_str[i]);
      }
    }
    if(three == '@' && same_three_letters(md5_str, i)) {
      three = md5_str[i];
    }
  }

  return Hash(index, three, fives);
}

int get_index(std::function<Hash(std::string, size_t)> hash_func, const std::string salt) {
  std::vector<Hash> hashes;
  for(int i = 0; i < 1001; i++) {
    hashes.push_back(hash_func(salt, i));
  }

  int index = 1001;
  int keys = 0;
  
  while(true) {
    Hash &curr_hash = hashes[0];
    bool found = false;
    if(curr_hash.three != '@') {
      for(size_t i = 1; i < hashes.size(); i++) {
        if(hashes[i].fives.find(curr_hash.three) != hashes[i].fives.end()) {
          found = true;
          keys++;
          break;
        }
        if(found) { break; }
      }
    }
    
    if(keys == 64) {
      return curr_hash.index;
    }

    hashes.erase(hashes.begin());
    hashes.push_back(hash_func(salt, index++));
  }
  
  return -1;
}

int main() {
  std::ifstream ifstrm("./data/q14.txt", std::ios::in);
  std::string salt;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    std::getline(ifstrm, line);
    salt = line;
  }

  std::cout << "Answer to part 1: " << get_index(get_hash, salt) << std::endl;
  std::cout << "Answer to part 2: " << get_index(get_stretched_hash, salt) << std::endl;
  return 0;
}