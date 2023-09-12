def part1(responses):
    total = 0
    for response in responses:
        currRep = ""
        for r in response:
            currRep += r
        total += len(set(currRep))
    return total

def part2(responses):
    total = 0
    for response in responses:
        currRep = []
        for r in response:
            currRep.append(set(r))
        res = currRep[0]
        for r in currRep:
            res = res.intersection(r)
        total += len(res)
    return total

responses = []
with open("./data/q6.txt", "r") as file:
    currRep = []
    for line in file:
        if line == "\n":
            responses.append(currRep)
            currRep = []
        else:
            currRep.append(line.strip())
    responses.append(currRep)

print("Answer to part1: {}".format(part1(responses)))
print("Answer to part2: {}".format(part2(responses)))