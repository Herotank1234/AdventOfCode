#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <assert.h>

class Cart {
public:
  Cart(int x, int y, int dir) : _x(x), _y(y), _dir(dir), _cross(0), _crashed(false) {}

  int getX() const {
    return _x;
  }

  int getY() const {
    return _y;
  }

  bool isCrashed() {
    return _crashed;
  }

  void setCrashed() {
    _crashed = true;
  }

  bool operator < (const Cart& other) {
    if(_y == other.getY()) return _x < other.getX();
    return _y < other.getY();
  }

  std::pair<int, int> getNextMove(char track) {
    int x = _x, y = _y;
    switch(track) {
      case '|':
        if(_dir == 0) {
          y--;
        } else if(_dir == 2) {
          y++;
        }
        break;
      case '-':
        if(_dir == 1) {
          x++;
        } else if(_dir == 3) {
          x--;
        }
        break;
      case '/':
        if(_dir == 0) {
          x++;
          _dir = 1;
        } else if(_dir == 1) {
          y--;
          _dir = 0;
        } else if(_dir == 2) {
          x--;
          _dir = 3;
        } else if(_dir == 3) {
          y++;
          _dir = 2;
        }
        break;
      case '\\':
        if(_dir == 0) {
          x--;
          _dir = 3;
        } else if (_dir == 1) {
          y++;
          _dir = 2;
        } else if(_dir == 2) {
          x++;
          _dir = 1;
        } else if(_dir == 3) {
          y--;
          _dir = 0;
        }
        break;
      case '+':
        switch(_cross) {
          case 0:
            _dir = ((_dir - 1) + 4) % 4;
            _cross++;
            break;
          case 1:
            _cross++;
            break;
          case 2:
            _dir = (_dir + 1) % 4;
            _cross = 0;
            break;
          default:
            std::cout << "NO SUCH CROSS" << std::endl;
        }
        if(_dir == 0) {
          y--;
        } else if(_dir == 1) {
          x++;
        } else if(_dir == 2) {
          y++;
        } else if(_dir == 3) {
          x--;
        }
        break;
      default:
        std::cout << "NO SUCH TRACK" << std::endl;
    }

    assert(x != _x || y != _y);
    return {x, y};
  }

  void move(std::pair<int, int> pos) {
    _x = pos.first;
    _y = pos.second;
  }

private:
  /* Direction:
  0: UP
  1: RIGHT
  2: DOWN
  3: LEFT
  */
  int _x, _y, _dir, _cross;
  bool _crashed;
};

std::string getFirstCrash(const std::vector<std::vector<char>> tracks, std::vector<Cart> carts) {
  while(true) {
    std::sort(carts.begin(), carts.end());
    for(Cart& cart : carts) {
      std::pair<int, int> pos = cart.getNextMove(tracks[cart.getY()][cart.getX()]);
      for(Cart other : carts) {
        if(other.getX() == pos.first && other.getY() == pos.second) {
          return std::to_string(pos.first) + "," + std::to_string(pos.second);
        }
      }
      cart.move(pos);
    }
  }
  return "_,_";
}

std::string getLastCartPos(const std::vector<std::vector<char>> tracks, std::vector<Cart> carts) {
  while(true) {
    std::sort(carts.begin(), carts.end());
    for(Cart& cart : carts) {
      if(!cart.isCrashed()) {
        std::pair<int, int> pos = cart.getNextMove(tracks[cart.getY()][cart.getX()]);
        for(Cart& other : carts) {
          if(!other.isCrashed() && other.getX() == pos.first && other.getY() == pos.second) {
            other.setCrashed();
            cart.setCrashed();
          }
        }
        if(!cart.isCrashed()) cart.move(pos);
      }
    }
    int remainingCarts = 0;
    for(Cart cart : carts) {
      if(!cart.isCrashed()) remainingCarts++;
    }
    if(remainingCarts == 1) {
      auto it = carts.begin();
      while(it->isCrashed()) it++;
      return std::to_string(it->getX()) + "," + std::to_string(it->getY());
    }
  }
  return "_,_";
}



int main() {
  std::ifstream ifstrm("./data/q13.txt", std::ios::in);
  std::unordered_map<char, int> cartToDirMap = {{'^', 0}, {'>', 1}, {'v', 2}, {'<', 3}};
  std::vector<std::vector<char>> tracks;
  std::vector<Cart> carts;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    int rowIndex = 0;
    while(std::getline(ifstrm, line)) {
      std::vector<char> row;
      int colIndex = 0;
      for(char c : line) {
        if(c == '^' || c == '>' || c == 'v' || c == '<') {
          carts.emplace_back(colIndex, rowIndex, cartToDirMap[c]);
          if(c == '^' || c == 'v') {
            row.push_back('|');
          } else {
            row.push_back('-');
          }
        } else {
          row.push_back(c);
        }
        colIndex++;
      }
      tracks.push_back(row);
      rowIndex++;
    }
  }

  std::cout << "Answer to part 1: " << getFirstCrash(tracks, carts) << std::endl;
  std::cout << "Answer to part 2: " << getLastCartPos(tracks, carts) << std::endl;

  return 0;
}