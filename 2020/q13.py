import re
import numpy as np

def part1(earliestTime, busIDs):
    waitTimes = []
    for busID in busIDs:
        waitTime = (((earliestTime // busID) + 1) * busID) - earliestTime
        waitTimes.append(waitTime)

    minIndex = np.argmin(waitTimes)

    return waitTimes[minIndex] * busIDs[minIndex]


def part2(allIDs):
    bi = []
    ni = []
    for i in range(0, len(allIDs)):
        if(allIDs[i] != 'x'):
            busNumber = int(allIDs[i])
            bi.append(((busNumber - i) % busNumber))
            ni.append(busNumber)

    N = np.prod(ni)
    Ni = [N // n for n in ni]
    
    xi = []
    coefficient = 0
    for i in range(0, len(Ni)):
        coefficient = Ni[i] % ni[i]
        if coefficient == 1:
            xi.append(1)
        else:
            x = 1
            while ((coefficient * x) % ni[i] != 1):
                x += 1
            xi.append(x)

    total = 0
    for i in range(0, len(bi)):
        total += bi[i] * Ni[i] * xi[i]

    return total % N

earliestTime: int
busIDs: list
allIDs : list
with open("./data/q13.txt", "r") as file:
    earliestTime = int(file.readline().strip())
    next = file.readline().strip()
    matches = re.findall(r"[\d]+", next)
    busIDs = [int(x) for x in matches]
    allIDs = next.split(",")

print("Answer to part1: {}".format(part1(earliestTime, busIDs)))
print("Answer to part2: {}".format(part2(allIDs)))