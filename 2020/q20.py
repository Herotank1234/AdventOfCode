import re
import numpy as np
import copy
from itertools import chain

class Tile:
  def __init__(self, id, grid, edges):
    self._id = id
    self._grid = grid
    self._edges = edges
    self._left = None
    self._up = None
    self._right = None
    self._down = None
    self._rot = 0
    self._flipV = False
    self._flipH = False
  
  def getEdge(self, direction):
    return self._edges[direction]
  
  def rot90(self):
    self._edges = np.insert(self._edges, 0, self._edges[-1], axis=0)[:-1]
    self._rot = (self._rot + 90) % 360

  def flipH(self):
    self._edges[0] = np.flip(self._edges[0])
    self._edges[2] = np.flip(self._edges[2])
    tmp = np.copy(self._edges[1])
    self._edges[1] = np.flip(self._edges[3])
    self._edges[3] = np.flip(tmp)
    self._flipH = not self._flipH

  def flipV(self):
    self._edges[1] = np.flip(self._edges[1])
    self._edges[3] = np.flip(self._edges[3])
    tmp = np.copy(self._edges[0])
    self._edges[0] = np.flip(self._edges[2])
    self._edges[2] = np.flip(tmp)
    self._flipV = not self._flipV
  
  def setNeighbour(self, direction, neighbour):
    if direction == 0:
      self._up = neighbour
    elif direction == 1:
      self._right = neighbour
    elif direction == 2:
      self._down = neighbour
    elif direction == 3:
      self._left = neighbour
    else:
      print("No such direction: " + str(direction))
  
  def getRemainingDirections(self):
    remDir = []
    if self._up is None:
      remDir.append(0)
    if self._right is None:
      remDir.append(1)
    if self._down is None:
      remDir.append(2)
    if self._left is None:
      remDir.append(3)
    return remDir
  
  def getId(self):
    return self._id
  
  def getGrid(self):
    res = copy.deepcopy(self._grid)
    res = np.delete(res, 0, 1)
    res = np.delete(res, len(res[0]) - 1, 1)
    res = np.delete(res, 0, 0)
    res = np.delete(res, len(res) - 1, 0)
    if self._flipH:
      res = np.fliplr(res)
    if self._flipV:
      res = np.flipud(res)
    rots = self._rot // 90
    res = np.rot90(res, 4 - rots)
    return res

def checkEdges(tile, direction, otherTile, otherDirection):
  for _ in range(0, 4):
    if(np.array_equal(tile.getEdge(direction), np.flip(otherTile.getEdge(otherDirection)))):
      return True
    otherTile.rot90()
  return False

def part1(tiles):
  def _foundSetNeighbours():
    checkTile.setNeighbour(checkDir, otherTile)
    otherTile.setNeighbour(checkAgainstDir, checkTile)

    currPosition = tilePosition.get(checkTile)
    otherPosition = (currPosition[0] + directions[checkDir][0], 
                      currPosition[1] + directions[checkDir][1])
    tilePosition[otherTile] = otherPosition
    for (index, direction) in enumerate(directions):
      nextPosition = (otherPosition[0] + direction[0], otherPosition[1] + direction[1])
      neighbour = tilePosition.get(nextPosition)
      if(neighbour is not None):
        otherTile.setNeighbour(index, neighbour)
        oppositeDirection = (index + 2) % 4
        neighbour.setNeighbour(oppositeDirection, otherTile)

    remDir = otherTile.getRemainingDirections()
    for dir in remDir:
      toVisit.append((dir, otherTile))
    currTiles.pop(otherId)

  currTiles = copy.deepcopy(tiles)  
  tilePosition = {}
  rootTile = currTiles.popitem()
  tilePosition[rootTile[1]] = (0, 0)

  #0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
  toVisit = [(i, rootTile[1]) for i in range(0, 4)] #(DIRECTION, TILEOBJ)

  while currTiles and toVisit:
    currCheck = toVisit.pop(0)
    checkDir = currCheck[0]
    checkTile = currCheck[1]
    checkAgainstDir = (checkDir + 2) % 4

    found = False
    for otherId, otherTile in currTiles.items():
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
        break
      otherTile.flipH()
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
        break
      otherTile.flipH()
      otherTile.flipV()
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
      otherTile.flipV()

  minI, maxI, minJ, maxJ = 10000, -10000, 10000, -10000
  for _, v in tilePosition.items():
    if v[0] < minI:
      minI = v[0]
    if v[0] > maxI:
      maxI = v[0]
    if v[1] < minJ:
      minJ = v[1]
    if v[1] > maxJ:
      maxJ = v[1]

  upperLeftId, lowerLeftId, upperRightId, lowerRightId, = 0, 0, 0, 0 
  for k, v in tilePosition.items():
    if v == (minI, minJ):
      upperLeftId = k.getId()
    elif v == (maxI, minJ):
      lowerLeftId = k.getId()
    elif v == (minI, maxJ):
      upperRightId = k.getId()
    elif v == (maxI, maxJ):
      lowerRightId = k.getId()

  return upperLeftId * lowerLeftId * upperRightId * lowerRightId

def checkAndSetMonster(i, j, grid, monsterPosition):
  monsterCoords = [(i, j), (i + 1, j + 1), (i + 1, j + 4), (i, j + 5), 
                   (i, j + 6), (i + 1, j + 7), (i + 1, j + 10), (i, j + 11),
                   (i, j + 12), (i + 1, j + 13), (i + 1, j + 16), (i, j + 17),
                   (i - 1, j + 18), (i, j + 18), (i, j + 19)]

  for monsterCoord in monsterCoords:
    if grid[monsterCoord[0]][monsterCoord[1]] != '#':
      return
  
  for monsterCoord in monsterCoords:
    monsterPosition[monsterCoord[0]][monsterCoord[1]] = 1

def part2():
  def _foundSetNeighbours():
    checkTile.setNeighbour(checkDir, otherTile)
    otherTile.setNeighbour(checkAgainstDir, checkTile)

    currPosition = tilePosition.get(checkTile)
    otherPosition = (currPosition[0] + directions[checkDir][0], 
                      currPosition[1] + directions[checkDir][1])
    tilePosition[otherTile] = otherPosition
    for (index, direction) in enumerate(directions):
      nextPosition = (otherPosition[0] + direction[0], otherPosition[1] + direction[1])
      neighbour = tilePosition.get(nextPosition)
      if(neighbour is not None):
        otherTile.setNeighbour(index, neighbour)
        oppositeDirection = (index + 2) % 4
        neighbour.setNeighbour(oppositeDirection, otherTile)

    remDir = otherTile.getRemainingDirections()
    for dir in remDir:
      toVisit.append((dir, otherTile))
    currTiles.pop(otherId)
  
  def _checkForMonster():
    for i in range(1, len(grid) - 1):
      for j in range(0, len(grid) - monsterWidth):
        checkAndSetMonster(i, j, grid, monsterPositions)
    return

  def _countWaterRoughness():
    roughness = 0
    for i in range(0, len(grid)):
      for j in range(0, len(grid)):
        if grid[i][j] == '#' and monsterPositions[i][j] == 0:
          roughness += 1
    return roughness

  currTiles = copy.deepcopy(tiles)  
  tilePosition = {}
  rootTile = currTiles.popitem()
  tilePosition[rootTile[1]] = (0, 0)

  #0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
  toVisit = [(i, rootTile[1]) for i in range(0, 4)] #(DIRECTION, TILEOBJ)

  while currTiles and toVisit:
    currCheck = toVisit.pop(0)
    checkDir = currCheck[0]
    checkTile = currCheck[1]
    checkAgainstDir = (checkDir + 2) % 4

    found = False
    for otherId, otherTile in currTiles.items():
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
        break
      otherTile.flipH()
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
        break
      otherTile.flipH()
      otherTile.flipV()
      found = checkEdges(checkTile, checkDir, otherTile, checkAgainstDir)
      if found:
        _foundSetNeighbours()
      otherTile.flipV()

  minI, maxI, minJ, maxJ = 10000, -10000, 10000, -10000
  for _, v in tilePosition.items():
    if v[0] < minI:
      minI = v[0]
    if v[0] > maxI:
      maxI = v[0]
    if v[1] < minJ:
      minJ = v[1]
    if v[1] > maxJ:
      maxJ = v[1]

  reverseTilePos = {}
  for k, v in tilePosition.items():
    reverseTilePos[v] = k

  grid = []
  for i in range(minI, maxI + 1):
    rows = []
    for j in range(minJ, maxJ + 1):
      currTile = reverseTilePos.get((i, j))
      rows.append(currTile.getGrid().tolist())
    rows = zip(*rows)
    for r in rows:
      grid.append(list(chain(*list(r))))

  grid = np.array(grid)
  monsterPositions = np.zeros((len(grid), len(grid)), dtype=int)

  monsterWidth = 20

  for _ in range(0, 4):
    _checkForMonster()
    grid = np.rot90(grid)
    monsterPositions = np.rot90(monsterPositions)

  grid = np.fliplr(grid)
  monsterPositions = np.fliplr(monsterPositions)
  for _ in range(0, 4):
    _checkForMonster()
    grid = np.rot90(grid)
    monsterPositions = np.rot90(monsterPositions)
  grid = np.fliplr(grid)
  monsterPositions = np.fliplr(monsterPositions)

  grid = np.flipud(grid)
  monsterPositions = np.flipud(monsterPositions)
  for _ in range(0, 4):
    _checkForMonster()
    grid = np.rot90(grid)
    monsterPositions = np.rot90(monsterPositions)

  return _countWaterRoughness()

tiles = {}

with open("./data/q20.txt", "r") as file:
  tileId = 0
  grid = []
  line = file.readline()

  while line:
    tileId = int(re.search(r"Tile ([\d]+):", line).group(1))
    line = file.readline()
    while line and line != "\n":
      grid.append([char for char in line.strip()])
      line = file.readline()
    
    grid = np.array(grid)
    edges = np.array([grid[0], grid[:,len(grid[0]) - 1], np.flip(grid[len(grid) - 1]), 
      np.flip(grid[:,0])])

    tile = Tile(tileId, grid, edges)
    tiles[tileId] = tile
    grid = []
    line = file.readline() 

print("Answer to part1: {}".format(part1(tiles)))
print("Answer to part2: {}".format(part2()))