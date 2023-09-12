import copy
from operator import neg
import pprint

pp = pprint.PrettyPrinter()

def extend(currState: list, width: int) -> list:
    res = [["." for _ in range(width + 2)]]
    for row in currState:
        res.append(["."] + row + ["."])
    res.append(["." for _ in range(width + 2)])
    return res

def getExtendedEmpty(length: int, width: int) -> list:
    return [["." for _ in range(width + 2)] 
        for _ in range(length + 2)]

def count(state: list) -> int:
    total = 0
    for row in state:
        for field in row:
            total = total + 1 if field == "#" else total
    return total

def part1(initialState):

    currState = copy.deepcopy(initialState)
    length, width = len(initialState), len(initialState[0])

    currState = extend(currState, width)
    nextState = getExtendedEmpty(length, width)
    changes = -1

    while changes != 0:
        changes = 0
        for i in range(1, length + 1):
            for j in range(1, width + 1):
                if currState[i][j] == "L":
                    change = True
                    for x in range(-1, 2):
                        for y in range(-1, 2):
                            if currState[i + x][j + y] == "#":
                                change = False
                    if change:
                        nextState[i][j] = "#"
                        changes += 1
                    else:
                        nextState[i][j] = "L"
                elif currState[i][j] == "#":
                    occ = -1
                    for x in range(-1, 2):
                        for y in range(-1, 2):
                            if currState[i + x][j + y] == "#":
                                occ += 1
                    if occ >= 4:
                        nextState[i][j] = "L"
                        changes += 1
                    else:
                        nextState[i][j] = "#"
        currState = nextState
        nextState = getExtendedEmpty(length, width)

    return count(currState)

def countOccupiedSeatsInEachDirection(i, j, length, width, currState):
    directions = [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, -1], [1, 0], [1, 1]]
    occupiedSeats = 0
    for direction in directions:
        di, dj = direction[0], direction[1]
        currI = i + di
        currJ = j + dj
        while currI >= 1 and currI <= length and currJ >= 1 and currJ <= width:
            if(currState[currI][currJ] == '#'):
                occupiedSeats += 1
                break
            elif(currState[currI][currJ] == 'L'):
                break
            currI += di
            currJ += dj
    return occupiedSeats


def part2(initialState):
    currState = copy.deepcopy(initialState)
    length, width = len(initialState), len(initialState[0])

    currState = extend(currState, width)
    nextState = getExtendedEmpty(length, width)
    changes = -1

    while changes != 0:
        changes = 0
        for i in range(1, length + 1):
            for j in range(1, width + 1):
                if currState[i][j] == '#':
                    occupiedSeats = countOccupiedSeatsInEachDirection(i, j, length, width, currState)
                    if occupiedSeats >= 5:
                        changes += 1
                        nextState[i][j] = 'L'
                    else:
                        nextState[i][j] = '#'
                elif currState[i][j] == 'L':
                    occupiedSeats = countOccupiedSeatsInEachDirection(i, j, length, width, currState)
                    if occupiedSeats == 0:
                        changes += 1
                        nextState[i][j] = '#'
                    else:
                        nextState[i][j] = 'L'
        currState = nextState
        nextState = getExtendedEmpty(length, width)

                    
    return count(currState)

state = []
with open("./data/q11.txt", "r") as file:
    for line in file:
        state.append([x for x in line.strip()])

print("Answer to part1: {}".format(part1(state)))
print("Answer to part2: {}".format(part2(state)))