def extend(grid, length):
  extended_grid = [['.' for _ in range(length + 2)]]
  for row in grid:
    extended_grid.append(['.'] + row + ['.'])
  extended_grid.append(['.' for _ in range(length + 2)])
  return extended_grid

def get_hash(grid, length):
  hash_val = 0
  for i in range(1, length + 1):
    for j in range(1, length + 1):
      if(grid[i][j] == '#'):
        shift = ((i - 1) * length) + (j - 1)
        hash_val |= (1 << shift)
  return hash_val

def count_neighbours(i, j, grid):
  directions = [[-1, 0], [0, 1], [1, 0], [0, -1]]
  neighbours = 0
  for direction in directions:
    next_i = i + direction[0]
    next_j = j + direction[1]
    if grid[next_i][next_j] == '#':
      neighbours += 1
  return neighbours


def simulate(grid, length):
  next_grid = [['.' for _ in range(length + 2)] for _ in range(length + 2)]
  for i in range(1, length + 1):
    for j in range(1, length + 1):
      neighbours = count_neighbours(i, j, grid)
      if grid[i][j] == '#':
        if neighbours == 1:
          next_grid[i][j] = '#'
      else:
        if neighbours == 1 or neighbours == 2:
          next_grid[i][j] = '#'
  return next_grid

def part1(grid):
  length = len(grid)
  curr_grid = extend(grid, length)

  seen = set()
  hash_val = get_hash(curr_grid, length)

  while hash_val not in seen:
    seen.add(hash_val)
    curr_grid = simulate(curr_grid, length)
    hash_val = get_hash(curr_grid, length)

  return hash_val

def get_recursive_neighbours(bug):
  (i, j, level) = bug
  neighbours = []

  # Left side
  if j == 0:
    neighbours.append((2, 1, level - 1))
  elif i == 2 and j == 3:
    for y in range(5):
      neighbours.append((y, 4, level + 1))
  else:
    neighbours.append((i, j - 1, level))

  # Top side
  if i == 0:
    neighbours.append((1, 2, level - 1))
  elif i == 3 and j == 2:
    for x in range(5):
      neighbours.append((4, x, level + 1))
  else:
    neighbours.append((i - 1, j, level))

  # Right side
  if j == 4:
    neighbours.append((2, 3, level - 1))
  elif i == 2 and j == 1:
    for y in range(5):
      neighbours.append((y, 0, level + 1))
  else:
    neighbours.append((i, j + 1, level))

  # Bottom side
  if i == 4:
    neighbours.append((3, 2, level - 1))
  elif i == 1 and j == 2:
    for x in range(5):
      neighbours.append((0, x, level + 1))
  else:
    neighbours.append((i + 1, j, level))

  return neighbours

def count_recursive_neighbours(neighbour, bugs):
  neighbours = get_recursive_neighbours(neighbour)
  return sum([1 for n in neighbours if n in bugs])

def part2(grid):
  bugs = set()
  for i in range(len(grid)):
    for j in range(len(grid[0])):
      if grid[i][j] == '#':
        bugs.add((i, j, 0))

  for _ in range(200):
    next_bugs = set()

    for bug in bugs:
      neighbours = get_recursive_neighbours(bug)

      for neighbour in neighbours:
        neighbour_count = count_recursive_neighbours(neighbour, bugs)
        if neighbour in bugs:
          if neighbour_count == 1:
            next_bugs.add(neighbour)
        else:
          if neighbour_count == 1 or neighbour_count == 2:
            next_bugs.add(neighbour)
      
    bugs = next_bugs

  return len(bugs)

grid = []
with open("./data/q24.txt", "r") as file:
  for line in file:
    grid.append([c for c in line if c != '\n'])


print("Answer to part1: {}".format(part1(grid[:])))
print("Answer to part2: {}".format(part2(grid[:])))