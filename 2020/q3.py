def part1(map):
    i = 0
    j = 0
    length = len(map)
    width = len(map[0])
    total = 0

    while i < length:
        j += 3
        if j >= width:
            j = j % width
        i += 1
        if i < length:
            total += 1 if map[i][j] == "#" else 0
    return total

def part2(map):
    slopes = [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2)]
    length = len(map)
    width = len(map[0])
    totals = []

    for (x, y) in slopes:
        i = 0
        j = 0
        total = 0
        while i < length:
            j += x
            if j >= width:
                j = j % width
            i += y
            if i < length:
                total += 1 if map[i][j] == "#" else 0
        totals.append(total)
    res = 1
    for t in totals:
        res *= t
    return res

map = []

with open("./data/q3.txt", "r") as file:
    for line in file:
        map.append([x for x in line.strip()])

print("Answer to part1: {}".format(part1(map)))
print("Answer to part2: {}".format(part2(map)))