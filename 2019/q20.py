class Maze:
  def __init__(self, grid, recursive=False):
    self._grid = grid
    self._portals = {}
    self._recursivePortals = {}
    self._entrance = None
    self._exit = None
    self._recursive = recursive
    if self._recursive:
      self._setRecursivePortals()
    else:
      self._setPortals()

  def _setPortals(self):
    def _inBounds(nextI, nextJ):
      return nextI >= 0 and nextI < len(self._grid) and nextJ >= 0 and \
        nextJ < len(self._grid[0])

    def _inRangeOfEmptySpace():
      directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
      for direction in directions:
        nextI, nextJ = i + direction[0], j + direction[1]
        if _inBounds(nextI, nextJ) and self._grid[nextI][nextJ] == ".":
          return True
      return False
    
    def _getPortalName():
      directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
      name = ""
      pos = None
      for direction in directions:
        nextI, nextJ = i + direction[0], j + direction[1]
        if self._grid[nextI][nextJ].isupper():
          name = self._grid[i][j] + self._grid[nextI][nextJ]
        elif self._grid[nextI][nextJ] == ".":
          pos = (nextI, nextJ)
      return name, pos

    for i in range(len(self._grid)):
      for j in range(len(self._grid[0])):
        if self._grid[i][j].isupper() and _inRangeOfEmptySpace():
          portalName, emptySpace = _getPortalName()
          if portalName == "AA":
            self._entrance = emptySpace
          elif portalName == "ZZ":
            self._exit = emptySpace
          else:
            if portalName in self._portals:
              self._portals[portalName].append(emptySpace)
            elif portalName[::-1] in self._portals:
              self._portals[portalName[::-1]].append(emptySpace)
            else:
              self._portals[portalName] = [emptySpace]

  def _setRecursivePortals(self):
    def _inBounds(nextI, nextJ):
      return nextI >= 0 and nextI < len(self._grid) and nextJ >= 0 and \
        nextJ < len(self._grid[0])

    def _inRangeOfEmptySpace():
      directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
      for direction in directions:
        nextI, nextJ = i + direction[0], j + direction[1]
        if _inBounds(nextI, nextJ) and self._grid[nextI][nextJ] == ".":
          return True
      return False
    
    def _getPortalName():
      directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
      name = ""
      pos = None
      for direction in directions:
        nextI, nextJ = i + direction[0], j + direction[1]
        if self._grid[nextI][nextJ].isupper():
          name = self._grid[i][j] + self._grid[nextI][nextJ]
        elif self._grid[nextI][nextJ] == ".":
          pos = (nextI, nextJ)
      return name, pos
    
    def _onEdge():
      return emptySpace[0] == 2 or emptySpace[1] == 2 or \
        emptySpace[0] == len(self._grid) - 3 or emptySpace[1] == len(self._grid[0]) - 3

    for i in range(len(self._grid)):
      for j in range(len(self._grid[0])):
        if self._grid[i][j].isupper() and _inRangeOfEmptySpace():
          portalName, emptySpace = _getPortalName()
          if portalName == "AA":
            self._entrance = emptySpace
          elif portalName == "ZZ":
            self._exit = emptySpace
          else:
            if portalName in self._recursivePortals:
              self._recursivePortals[portalName]["outer" if _onEdge() else "inner"] = emptySpace
            elif portalName[::-1] in self._recursivePortals:
              self._recursivePortals[portalName[::-1]]["outer" if _onEdge() else "inner"] = emptySpace
            else:
              self._recursivePortals[portalName] = {}
              self._recursivePortals[portalName]["outer" if _onEdge() else "inner"] = emptySpace
  
  def getEntrance(self):
    return self._entrance
  
  def getExit(self):
    return self._exit
  
  def getPortals(self):
    return self._portals
  
  def getRecursivePortals(self):
    return self._recursivePortals
  
  def getPortalExit(self, nextPos):
    for _, portalPositions in self._portals.items():
      if nextPos == portalPositions[0]:
        return portalPositions[1]
      elif nextPos == portalPositions[1]:
        return portalPositions[0]
    return (-1, -1)
  
  def getRecursivePortalExit(self, nextPos, level):
    for j, portalPositions in self._recursivePortals.items():
      if nextPos == portalPositions["outer"] and level != 0:
        return portalPositions["inner"], -1
      elif nextPos == portalPositions["inner"]:
        return portalPositions["outer"], 1
    return (-1, -1), 0

def part1(grid):
  maze = Maze(grid)
  currPos = maze.getEntrance()
  exit = maze.getExit()
  toVisit = [(currPos, 0)]
  visited = set()
  shortestPath = 0
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

  while exit not in visited:
    currPos, pathLength = toVisit.pop(0)

    if currPos in visited:
      continue
    visited.add(currPos)

    if currPos == exit:
      shortestPath = pathLength
      continue

    for direction in directions:
      nextPos = (currPos[0] + direction[0], currPos[1] + direction[1])

      if nextPos in visited:
        continue

      if grid[nextPos[0]][nextPos[1]] == '#':
        continue

      if grid[nextPos[0]][nextPos[1]].isupper():
        portalExit = maze.getPortalExit(currPos)
        if portalExit == (-1, -1):
          continue
        nextPos = portalExit

      toVisit.append((nextPos, pathLength + 1))

  return shortestPath

def part2(grid):
  maze = Maze(grid, recursive=True)
  currPos = maze.getEntrance()
  exit = maze.getExit()
  toVisit = [(currPos, 0, 0)]
  visited = {0: set()}
  shortestPath = 0
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

  while exit not in visited[0]:
    currPos, level, pathLength = toVisit.pop(0)

    if level not in visited:
      visited[level] = set()

    if currPos in visited[level]:
      continue
    visited[level].add(currPos)

    if currPos == exit and level == 0:
      shortestPath = pathLength
      continue

    for direction in directions:
      nextPos = (currPos[0] + direction[0], currPos[1] + direction[1])
      levelChange = 0

      if nextPos in visited:
        continue

      if grid[nextPos[0]][nextPos[1]] == '#':
        continue

      if grid[nextPos[0]][nextPos[1]].isupper():
        portalExit, levelChange = maze.getRecursivePortalExit(currPos, level)
        if portalExit == (-1, -1):
          continue
        nextPos = portalExit

      toVisit.append((nextPos, level + levelChange, pathLength + 1))

  return shortestPath

grid = []
with open("./data/q20.txt", "r") as file:
  for line in file:
    grid.append([*line.rstrip("\n")])

print("Answer to part1: {}".format(part1(grid[:])))
print("Answer to part2: {}".format(part2(grid[:])))