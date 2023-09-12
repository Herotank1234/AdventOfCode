UPPER = 7
LOWER = 3

def part1(passes):
    highestSeatId = 0
    for p in passes:
        lowerR = 0
        upperR = 127
        lowerC = 0
        upperC = 7
        for i in range(UPPER):
            if p[i] == "B":
                lowerR += (upperR - lowerR + 1) // 2
            else:
                upperR -= (upperR - lowerR + 1) // 2
        for j in range(LOWER):
            if p[UPPER + j] == "R":
                lowerC += (upperC - lowerC + 1) // 2
            else:
                upperC -= (upperC - lowerC + 1) // 2
        seatID = lowerR * 8 + lowerC
        highestSeatId = seatID if seatID > highestSeatId else highestSeatId
    return highestSeatId

def part2(passes):
    ids = []
    for p in passes:
        lowerR = 0
        upperR = 127
        lowerC = 0
        upperC = 7
        for i in range(UPPER):
            if p[i] == "B":
                lowerR += (upperR - lowerR + 1) // 2
            else:
                upperR -= (upperR - lowerR + 1) // 2
        for j in range(LOWER):
            if p[UPPER + j] == "R":
                lowerC += (upperC - lowerC + 1) // 2
            else:
                upperC -= (upperC - lowerC + 1) // 2
        seatID = lowerR * 8 + lowerC
        ids.append(seatID)
    ids.sort()
    index = ids[0]
    for id in ids:
        if id != index:
            break
        else:
            index += 1
    return index

passes = []

with open("./data/q5.txt", "r") as file:
    for line in file:
        passes.append(line.strip())

print("Answer to part1: {}".format(part1(passes)))
print("Answer to part2: {}".format(part2(passes)))