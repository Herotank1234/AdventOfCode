def part1(jolts):
    currJolts = jolts[:]
    currJolts.append(0)
    currJolts.sort()

    oneDiff, threeDiff = 0, 1
    for i in range(1, len(currJolts)):
        if currJolts[i] - currJolts[i - 1] == 1:
            oneDiff += 1
        elif currJolts[i] - currJolts[i - 1] == 3:
            threeDiff += 1
    return oneDiff * threeDiff

def countDistinctWays(currJolts, jolts, memo):
    if memo.get(jolts, None) is not None:
        return memo.get(jolts)
    
    if currJolts.index(jolts) >= len(currJolts) - 2:
        return 1
    
    nextJolts = currJolts[:]
    nextJolts.remove(jolts)
    candidates = [x for x in nextJolts if x - jolts <= 3 and x - jolts > 0]
    total = 0
    for candidate in candidates:
        total += countDistinctWays(nextJolts, candidate, memo)
    
    memo[jolts] = total
    return total

def part2(jolts):
    currJolts = jolts[:]
    currJolts.append(0)
    currJolts.sort()
    currJolts = list(set(currJolts))
    memo = {}
    return countDistinctWays(currJolts, 0, memo)



jolts = []
with open("./data/q10.txt", "r") as file:
    for line in file:
        jolts.append(int(line.strip()))

print("Answer to part1: {}".format(part1(jolts)))
print("Answer to part2: {}".format(part2(jolts)))