import re

def part1(instr):
    acc = 0
    index = 0
    visited = []

    while True:
        if index in visited:
            break
        visited.append(index)
        currOp, currVal = instr[index]
        if currOp == "nop":
            index += 1
        elif currOp == "acc":
            acc += currVal
            index += 1
        elif currOp == "jmp":
            index += currVal

    return acc

def part2(instr):
    for i in range(len(instr)):
        op, val = instr[i]
        if op == "jmp" or op == "nop":
            currInstr = instr[:]
            currInstr[i] = ("jmp", val) if op == "nop" else ("nop", val)

            acc = 0
            index = 0
            visited = []
            terminated = True

            while index < len(currInstr):
                if index in visited:
                    terminated = False
                    break
                visited.append(index)
                currOp, currVal = currInstr[index]
                if currOp == "nop":
                    index += 1
                elif currOp == "acc":
                    acc += currVal
                    index += 1
                elif currOp == "jmp":
                    index += currVal
            
            if terminated:
                return acc
    return -1

instr = []
with open("./data/q8.txt", "r") as file:
    for line in file:
        op = line[:3]
        value = re.search(r"(\+|\-)[\d]+", line).group()
        instr.append((op, int(value)))

print("Answer to part1: {}".format(part1(instr)))
print("Answer to part2: {}".format(part2(instr)))