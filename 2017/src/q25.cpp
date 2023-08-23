#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

typedef struct Action {
  int writeVal;
  bool moveRight;
  char nextState;
} Action;

typedef struct State {
  State(char state, int zeroW, bool zeroM, char zeroN, int oneW, bool oneM, char oneN) {
    stateName = state;
    zeroAction.writeVal = zeroW;
    zeroAction.moveRight = zeroM;
    zeroAction.nextState = zeroN;
    oneAction.writeVal = oneW;
    oneAction.moveRight = oneM;
    oneAction.nextState = oneN;
  }

  void print() const {
    std::cout << stateName << " " << zeroAction.writeVal << " " << (int) zeroAction.moveRight 
      << " " << zeroAction.nextState << " " << oneAction.writeVal << " " << 
      (int) oneAction.moveRight << " " << oneAction.nextState << std::endl;
  }

  char stateName;
  Action zeroAction;
  Action oneAction;
} State;

int runAndCalcChecksum(const std::vector<State>& states, char currState, int steps) {
  std::unordered_map<int, int> tape;
  std::unordered_map<char, State> stateMap;
  for(const State& s : states) {
    stateMap.insert({s.stateName, s});
  }
  int index = 0;

  for(int i = 0; i < steps; i++) {
    int currVal = 0;
    auto search = tape.find(index);
    if(search != tape.end()) {
      currVal = search->second;
    }

    const State& curr = stateMap.find(currState)->second;
    Action act = (currVal == 0 ? curr.zeroAction : curr.oneAction);
    tape[index] = act.writeVal;
    index += (act.moveRight ? 1 : -1);
    currState = act.nextState;
  }

  int ones = 0;
  for(auto slot : tape) {
    if(slot.second == 1)  ones++;
  }
  return ones;
}

int main() {
  std::ifstream ifstrm("./data/q25.txt", std::ios::in);
  std::vector<State> states;
  char startState = 'A';
  int steps = 0;

  if(!ifstrm.is_open()) {
    std::cout << "Failed to open file" << std::endl;
  } else {
    std::regex matchStart("Begin in state ([A-Z]).[\\s]?");
    std::regex matchCheckSum("Perform a diagnostic checksum after ([\\d]+) steps.[\\s]?");
    std::regex matchState("In state ([A-Z]):[\\s]?");
    std::regex matchCurrent("  If the current value is (0|1):[\\s]?");
    std::regex matchWrite("    - Write the value (0|1).[\\s]?");
    std::regex matchMove("    - Move one slot to the (left|right).[\\s]?");
    std::regex matchContinue("    - Continue with state ([A-Z]).[\\s]?");
    std::smatch sm;

    std::string line;
    std::getline(ifstrm, line);
    if(std::regex_match(line, sm, matchStart)) {
      startState = sm[1].str()[0];
    } else {
      std::cout << "Start no match" << std::endl;
    }
    std::getline(ifstrm, line);
    if(std::regex_match(line, sm, matchCheckSum)) {
      steps = stoi(sm[1]);
    } else {
      std::cout << "Checksum no match" << std::endl;
    }
    while(std::getline(ifstrm, line)) {
      if(line == "\r") {
        continue;
      } else {
        char state = 'A';
        int zeroWrite = 0, oneWrite = 0;
        bool zeroMoveRight = false, oneMoveRight = false;
        char zeroNextState = 'A', oneNextState = 'A';
        if(std::regex_match(line, sm, matchState)) {
          state = sm[1].str()[0];
        } else {
          std::cout << "State no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(!std::regex_match(line, sm, matchCurrent)) {
          std::cout << "Current value 0 no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchWrite)) {
          zeroWrite = stoi(sm[1]);
        } else {
          std::cout << "Zero write no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchMove)) {
          zeroMoveRight = sm[1].str() == "right";
        } else {
          std::cout << "Zero move no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchContinue)) {
          zeroNextState = sm[1].str()[0];
        } else {
          std::cout << "Zero next state no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(!std::regex_match(line, sm, matchCurrent)) {
          std::cout << "Current value 1 no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchWrite)) {
          oneWrite = stoi(sm[1]);
        } else {
          std::cout << "One write no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchMove)) {
          oneMoveRight = sm[1].str() == "right";
        } else {
          std::cout << "One move no match" << std::endl;
        }
        std::getline(ifstrm, line);
        if(std::regex_match(line, sm, matchContinue)) {
          oneNextState = sm[1].str()[0];
        } else {
          std::cout << "One next state no match" << std::endl;
        }
        states.emplace_back(state, zeroWrite, zeroMoveRight, zeroNextState,
          oneWrite, oneMoveRight, oneNextState);
      }
    }
  }

  std::cout << "Answer to part 1: " << runAndCalcChecksum(states, startState, steps) << std::endl;
  return 0;
}