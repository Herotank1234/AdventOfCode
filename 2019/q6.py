def part1(orbits):
  orbitMap = {}
  for orbit in orbits:
    if orbit[0] in orbitMap:
      orbitMap[orbit[0]].append(orbit[1])
    else:
      orbitMap[orbit[0]] = [orbit[1]]
  nodesToVisit = ["COM"]
  while nodesToVisit:
    node = nodesToVisit[0]
    nodesToVisit.remove(node)
    if node in orbitMap:
      nodesToVisit += orbitMap[node]
      for key in orbitMap:
        if key != node:
          if node in orbitMap[key]:
            orbitMap[key] = orbitMap[key] + orbitMap[node]

  total = 0
  for key in orbitMap:
    total += len(orbitMap[key])
  return total

def part2(orbits):
  orbitMap = {}
  for orbit in orbits:
    if orbit[1] in orbitMap:
      orbitMap[orbit[1]].append(orbit[0])
    else:
      orbitMap[orbit[1]] = [orbit[0]]
  youList = []
  nodesToVisit = ["YOU"]
  while nodesToVisit:
    node = nodesToVisit[0]
    nodesToVisit.remove(node)
    if node in orbitMap:
      youList.append(orbitMap[node][0])
      nodesToVisit.append(orbitMap[node][0])

  sanList = []
  nodesToVisit = ["SAN"]
  while nodesToVisit:
    node = nodesToVisit[0]
    nodesToVisit.remove(node)
    if node in orbitMap:
      sanList.append(orbitMap[node][0])
      nodesToVisit.append(orbitMap[node][0])

  intersect = set(youList).intersection(set(sanList))
  steps = 0
  for y in youList:
    if y in intersect:
      break
    steps += 1
  for s in sanList:
    if s in intersect:
      break
    steps += 1
  return steps

orbits = []
with open("./data/q6.txt", "r") as file:
  for line in file:
    orbits.append(line.strip().split(")"))

print("Answer to part1: {}".format(part1(orbits)))
print("Answer to part2: {}".format(part2(orbits)))