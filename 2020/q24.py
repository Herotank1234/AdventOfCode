import re

def flipTiles(directionsList):
  directionMap = {
    "e": (1, 1, 0),
    "se": (1, 0, -1),
    "sw": (0, -1, -1),
    "w": (-1, -1, 0),
    "ne": (0, 1, 1),
    "nw": (-1, 0 ,1)
  }
  tiles = {}

  for directions in directionsList:
    currPos = (0, 0, 0)
    for direction in directions:
      step = directionMap[direction]
      currPos = tuple(map(sum, zip(currPos, step)))
    tile = tiles.get(currPos, False)
    tiles[currPos] = not tile
  
  return tiles

def simulate(tiles):
  def _countBlackNeighbours():
    directions = [(1, 1, 0), (1, 0, -1), (0, -1, -1), (-1, -1, 0), (0, 1, 1), (-1, 0 ,1)]
    isBlack = 0
    for direction in directions:
      check = tuple(map(sum, zip(neighbour, direction)))
      if tiles.get(check, False):
        isBlack += 1
    
    return isBlack

  blackTiles = dict([(k, v) for k, v in tiles.items() if v])
  neighbourDirs = [(1, 1, 0), (1, 0, -1), (0, -1, -1), (-1, -1, 0), (0, 1, 1), (-1, 0 ,1), (0, 0, 0)]
  nextTiles = {}
  for blackTilePos, _ in blackTiles.items():
    for neighbourDir in neighbourDirs:
      neighbour = tuple(map(sum, zip(blackTilePos, neighbourDir)))
      noBlackNeighbours = _countBlackNeighbours()
      if tiles.get(neighbour, False):
        if not (noBlackNeighbours == 0 or noBlackNeighbours > 2):
          nextTiles[neighbour] = True
      else:
        if noBlackNeighbours == 2:
          nextTiles[neighbour] = True

  return nextTiles

def part1(directionsList):
  tiles = flipTiles(directionsList)
  return sum([1 if v else 0 for _, v in tiles.items()])

def part2(directionsList):
  tiles = flipTiles(directionsList)
  for _ in range(0, 100):
    tiles = simulate(tiles)
  return sum([1 if v else 0 for _, v in tiles.items()])

directionsList = []

with open("./data/q24.txt", "r") as file:
  for line in file:
    match = re.findall(r"(e|se|sw|w|nw|ne)", line)
    directionsList.append(match)


print("Answer to part1: {}".format(part1(directionsList)))
print("Answer to part2: {}".format(part2(directionsList)))