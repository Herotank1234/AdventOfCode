import itertools
import copy
import numpy as np

def part1(state):
  currState = copy.deepcopy(state)
  iterations = 6
  for _ in range(0, iterations):
    nextState = set()
    checked = set()
    for (x, y, z) in currState:
      for direction in itertools.product([-1, 0, 1], repeat=3):
        currX, currY, currZ = x + direction[0], y + direction[1], z + direction[2]
        if (currX, currY, currZ) in checked: continue
        neighbours = len([(a, b, c) for (a, b, c) in currState 
          if abs(currX - a) <= 1 and abs(currY - b) <= 1 and abs(currZ - c) <= 1])
        if (currX, currY, currZ) in currState:
          neighbours -= 1
          if neighbours == 2 or neighbours == 3:
            nextState.add((currX, currY, currZ))
        else:
          if neighbours == 3:
            nextState.add((currX, currY, currZ))
        checked.add((currX, currY, currZ))
    currState = nextState
        
  return len(currState)

def part2(state4D):
  currState = copy.deepcopy(state4D)
  iterations = 6
  for _ in range(0, iterations):
    nextState = set()
    checked = set()
    for (x, y, z, w) in currState:
      for direction in itertools.product([-1, 0, 1], repeat=4):
        currX, currY, currZ, currW = x + direction[0], y + direction[1], z + direction[2], w + direction[3]
        if(currX, currY, currZ, currW) in checked: continue
        neighbours = len([(a, b, c, d) for (a, b, c, d) in currState if abs(currX - a) <= 1 and abs(currY - b) <= 1 and 
          abs(currZ - c) <= 1 and abs(currW - d) <= 1 ])
        if (currX, currY, currZ, currW) in currState:
          neighbours -= 1
          if neighbours == 2 or neighbours == 3:
            nextState.add((currX, currY, currZ, currW))
        else:
          if neighbours == 3:
            nextState.add((currX, currY, currZ, currW))
        checked.add((currX, currY, currZ, currW))
    currState = nextState
        
  return len(currState)

state = set()
state4D = set()
with open("./data/q17.txt", "r") as file:
  for i, line in enumerate(file):
    for j, col in enumerate(line):
      if line[j] == '#':
        state.add((i, j, 0))
        state4D.add((i, j, 0, 0))

print("Answer to part1: {}".format(part1(state)))
print("Answer to part2: {}".format(part2(state4D)))