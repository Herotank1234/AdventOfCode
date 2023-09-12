import re

def part1(rules):
    totalValid = 0
    for (leftBound, rightBound, letter, password) in rules:
        matches = len(re.findall(letter, password))
        totalValid += 1 if matches >= leftBound and matches <= rightBound else 0
    return totalValid

def part2(rules):
    totalValid = 0
    for (leftBound, rightBound, letter, password) in rules:
        totalValid += 1 if (password[leftBound - 1] == letter \
            or password[rightBound - 1] == letter) and not \
                (password[leftBound - 1] == letter \
            and password[rightBound - 1] == letter) else 0
    return totalValid

rules = []

with open("./data/q2.txt", "r") as file:
    for line in file:
        bounds = re.search(r"[\d]+-[\d]+", line).group().split("-")
        letter = re.search(r"[a-z]:", line).group()[0]
        password = re.search(r": [a-z]+", line).group()[2:]
        rules.append((int(bounds[0]), int(bounds[1]), letter, password))

print("Answer to part1: {}".format(part1(rules)))
print("Answer to part2: {}".format(part2(rules)))