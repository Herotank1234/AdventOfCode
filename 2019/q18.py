import copy
import queue

class Vault:
  def __init__(self, grid):
    self._grid = grid
    self._keys = {}
    self._doors = {}
    self._entrance = (-1, -1)
    self._setKeysDoorsAndStart()
    self._originalKeys = copy.deepcopy(self._keys)
    self._originalDoors = copy.deepcopy(self._doors)

  def _setKeysDoorsAndStart(self):
    for i in range(len(self._grid)):
      for j in range(len(self._grid[0])):
        if self._grid[i][j].islower():
          self._keys[self._grid[i][j]] = (i, j)
        if self._grid[i][j].isupper():
          self._doors[self._grid[i][j]] = (i, j)
        if self._grid[i][j] == "@":
          self._entrance = (i, j)

  def getKeyPosition(self, key):
    assert key in self._keys
    return self._keys[key]
  
  def removeKey(self, key):
    if key in self._keys:
      keyPos = self.getKeyPosition(key)
      self._grid[keyPos[0]][keyPos[1]] = "."
      self._keys.pop(key)
  
  def getDoorPos(self, door):
    assert door in self._doors
    return self._doors[door]
  
  def unlockDoor(self, key):
    door = key.upper()
    if door in self._doors:
      doorPos = self.getDoorPos(door)
      self._grid[doorPos[0]][doorPos[1]] = '.'
      self._doors.pop(door)
  
  def getEntrance(self):
    return self._entrance
  
  def getCollectableKeys(self, initialPos):
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
    toVisit = [(initialPos, 0)]
    visited = set()
    collectableKeys = []

    while toVisit:
      currPos, pathLength = toVisit.pop(0)
      
      if currPos in visited:
        continue
      visited.add(currPos)

      if self._grid[currPos[0]][currPos[1]].islower():
        collectableKeys.append((self._grid[currPos[0]][currPos[1]], pathLength))
        continue

      if self._grid[currPos[0]][currPos[1]].isupper():
        continue

      for direction in directions:
        nextPos = (currPos[0] + direction[0], currPos[1] + direction[1])
        if nextPos not in visited and self._grid[nextPos[0]][nextPos[1]] != '#':
          toVisit.append((nextPos, pathLength + 1))

    return collectableKeys

  def transformEntrance(self):
    pos = self._entrance
    self._grid[pos[0] - 1][pos[1] - 1] = "@"
    self._grid[pos[0] - 1][pos[1]] = "#"
    self._grid[pos[0] - 1][pos[1] + 1] = "@"
    self._grid[pos[0]][pos[1] - 1] = "#"
    self._grid[pos[0]][pos[1]] = "#"
    self._grid[pos[0]][pos[1] + 1] = "#"
    self._grid[pos[0] + 1][pos[1] - 1] = "@"
    self._grid[pos[0] + 1][pos[1]] = "#"
    self._grid[pos[0] + 1][pos[1] + 1] = "@"



def part1(grid):
  initialVault = Vault(grid)
  id = 0

  toVisit = queue.PriorityQueue()
  toVisit.put((0, initialVault.getEntrance(), id, initialVault))
  id += 1
  visited = set()

  while toVisit:
    currSteps, currPos, _, currVault = toVisit.get()

    collectableKeys = currVault.getCollectableKeys(currPos)
    collectableKeysKey = [k for k, _ in collectableKeys]
    collectableKeysKey.sort()
    key = str(currPos[0]) + "," + str(currPos[1]) + "," + \
      "".join(collectableKeysKey)
    
    if key in visited:
      continue
    visited.add(key)
    
    if not collectableKeys:
      return currSteps

    for collectableKey, pathLength in collectableKeys:
      nextVault = copy.deepcopy(currVault)
      nextPos = nextVault.getKeyPosition(collectableKey)
      nextVault.removeKey(collectableKey)
      nextVault.unlockDoor(collectableKey)
      toVisit.put((currSteps + pathLength, nextPos, id, nextVault))
      id += 1
  
  return -1

def part2(grid):
  initialVault = Vault(grid)
  initialVault.transformEntrance()
  id = 0
  oldE = initialVault.getEntrance()

  toVisit = queue.PriorityQueue()
  toVisit.put((0, (oldE[0] - 1, oldE[1] - 1), (oldE[0] - 1, oldE[1] + 1), 
    (oldE[0] + 1, oldE[1] - 1), (oldE[0] + 1, oldE[1] + 1), id, initialVault))
  id += 1
  visited = set()

  while toVisit:
    currSteps, currPosTL, currPosTR, currPosBL, currPosBR, _, currVault = toVisit.get()

    keysTL = currVault.getCollectableKeys(currPosTL)
    keysTR = currVault.getCollectableKeys(currPosTR)
    keysBL = currVault.getCollectableKeys(currPosBL)
    keysBR = currVault.getCollectableKeys(currPosBR)

    keysKey = [k for k, _ in keysTL] + [k for k, _ in keysTR] + [k for k, _ in keysBL] + \
      [k for k, _ in keysBR]
    keysKey.sort()

    key = str(currPosTL) + str(currPosTR) + str(currPosBL) + str(currPosBR) + \
      "".join(keysKey)
    
    if key in visited:
      continue
    visited.add(key)
    
    if not keysTL and not keysTR and not keysBL and not keysBR:
      return currSteps
    
    for collectableKey, pathLength in keysTL:
      nextVault = copy.deepcopy(currVault)
      nextPos = nextVault.getKeyPosition(collectableKey)
      nextVault.removeKey(collectableKey)
      nextVault.unlockDoor(collectableKey)
      toVisit.put((currSteps + pathLength, nextPos, currPosTR, currPosBL, currPosBR, id, nextVault))
      id += 1

    for collectableKey, pathLength in keysTR:
      nextVault = copy.deepcopy(currVault)
      nextPos = nextVault.getKeyPosition(collectableKey)
      nextVault.removeKey(collectableKey)
      nextVault.unlockDoor(collectableKey)
      toVisit.put((currSteps + pathLength, currPosTL, nextPos, currPosBL, currPosBR, id, nextVault))
      id += 1
      
    for collectableKey, pathLength in keysBL:
      nextVault = copy.deepcopy(currVault)
      nextPos = nextVault.getKeyPosition(collectableKey)
      nextVault.removeKey(collectableKey)
      nextVault.unlockDoor(collectableKey)
      toVisit.put((currSteps + pathLength, currPosTL, currPosTR, nextPos, currPosBR, id, nextVault))
      id += 1

    for collectableKey, pathLength in keysBR:
      nextVault = copy.deepcopy(currVault)
      nextPos = nextVault.getKeyPosition(collectableKey)
      nextVault.removeKey(collectableKey)
      nextVault.unlockDoor(collectableKey)
      toVisit.put((currSteps + pathLength, currPosTL, currPosTR, currPosBL, nextPos, id, nextVault))
      id += 1

  return -1

grid = []
with open("./data/q18.txt", "r") as file:
  for line in file:
    line = [*line.strip()]
    grid.append(line)

print("Answer to part1: {}".format(part1(grid[:])))
print("Answer to part2: {}".format(part2(grid[:])))