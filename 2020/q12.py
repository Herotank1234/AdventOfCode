DIR = ["N", "E", "S", "W"]

def part1(instr):
    dir = "E"
    dirIndex = 1
    x, y = 0, 0
    for (op, val) in instr:
        if op == "F":
            if dir == "E":
                x += val
            elif dir == "S":
                y -= val
            elif dir == "W":
                x -= val
            elif dir == "N":
                y += val
        elif op == "E":
            x += val
        elif op == "S":
            y -= val
        elif op == "W":
            x -= val
        elif op == "N":
            y += val
        elif op == "L":
            dirIndex = (dirIndex - (val // 90)) % 4
            dir = DIR[dirIndex]
        elif op == "R":
            dirIndex = (dirIndex + (val // 90)) % 4
            dir = DIR[dirIndex]
    return abs(x) + abs(y)

def part2(instr):
    wayX, wayY = 10, 1
    x, y = 0, 0
    for (op, val) in instr:
        if op == "F":
            x += (val * wayX)
            y += (val * wayY)
        elif op == "E":
            wayX += val
        elif op == "S":
            wayY -= val
        elif op == "W":
            wayX -= val
        elif op == "N":
            wayY += val
        elif op == "L":
            reps = val // 90
            for _ in range(reps):
                tmp = wayX
                wayX = wayY * -1
                wayY = tmp
        elif op == "R":
            reps = val // 90
            for _ in range(reps):
                tmp = wayX * -1
                wayX = wayY
                wayY = tmp
    return abs(x) + abs(y)

instr = []
with open("./data/q12.txt", "r") as file:
    for line in file:
        instr.append((line[0], int(line.strip()[1:])))

print("Answer to part1: {}".format(part1(instr)))
print("Answer to part2: {}".format(part2(instr)))