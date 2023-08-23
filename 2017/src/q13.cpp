#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>

class Layer{
  public:
    Layer(int id, int depth) : _layerID(id), _layerDepth(depth),
      _scannerPos(0), _isMovingDown(true) {}

    int getID() {
      return _layerID;
    }

    bool isAtTopOfLayer() {
      return _scannerPos == 0;
    }

    int getSeverity() {
      return _layerID * _layerDepth;
    }

    int getDepth() {
      return _layerDepth;
    }

    void moveScanner() {
      _isMovingDown ? _scannerPos++ : _scannerPos--;
      if(_scannerPos == _layerDepth - 1) {
        _isMovingDown = false;
      } else if(_scannerPos == 0) {
        _isMovingDown = true;
      }
    }

  private:
    int _layerID, _layerDepth, _scannerPos;
    bool _isMovingDown;
};

int calcSeverity(std::vector<Layer> layers) {
  int maxLayer = 0;
  std::unordered_map<int, Layer> layerMap;
  for(Layer layer : layers) {
    int id = layer.getID();
    layerMap.insert({id, layer});
    if(id > maxLayer) maxLayer = id;
  }
  
  int currLayer = 0, severity = 0;
  while(currLayer <= maxLayer) {
    auto search = layerMap.find(currLayer);
    if(search != layerMap.end()) {
      Layer& curr = search->second;
      if(curr.isAtTopOfLayer()) severity += curr.getSeverity();
    }

    for(auto& layerPair : layerMap) {
      Layer &layer = layerPair.second;
      layer.moveScanner();
    }
    currLayer++;
  }

  return severity;
}

bool moveThroughFirewall(std::unordered_map<int, Layer>& layerMap, const int maxLayer) {
  int currLayer = 0;
  while(currLayer <= maxLayer) {
    auto search = layerMap.find(currLayer);
    if(search != layerMap.end()) {
      Layer& curr = search->second;
      if(curr.isAtTopOfLayer()) {
        return false;
      }
    }

    for(auto& layerPair : layerMap) {
      Layer &layer = layerPair.second;
      layer.moveScanner();
    }
    currLayer++;
  }

  return true;
}

int calcDelay(std::vector<Layer> layers) {
  int delay = 0;
  bool found = false;
  while(!found) {
    bool valid = true;
    for(Layer l : layers) {
      if((l.getID() + delay) % (2 * (l.getDepth() - 1)) == 0) {
        valid = false;
        break;
      }
    }
    if(valid) {
      found = true;
    } else {
      delay++;
    }
  }

  return delay;
}

int main() {
  std::ifstream ifstrm("./data/q13.txt", std::ios::in);
  std::vector<Layer> layers;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::string line;
    while(std::getline(ifstrm, line)) {
      std::regex match("([\\d]+): ([\\d]+)[\\s]?");
      std::smatch sm;
      if(std::regex_match(line, sm, match)) {
        layers.emplace_back(stoi(sm[1]), stoi(sm[2]));
      }
    }
  }

  std::cout << "Answer to part 1: " << calcSeverity(layers) << std::endl;
  std::cout << "Answer to part 2: " << calcDelay(layers) << std::endl;
  return 0;
}