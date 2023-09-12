import re
REQUIREDFIELDS = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
EYECOLOURS = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

def part1(passports):
    totalValid = 0
    valid = True
    for passport in passports:
        for field in REQUIREDFIELDS:
            if field not in passport:
                valid = False
                break
        totalValid += 1 if valid else 0
        valid = True
    return totalValid

def part2(passports):
    totalValid = 0
    valid = True
    for passport in passports:
        for field in REQUIREDFIELDS:
            if field not in passport:
                valid = False
                break
            else:
                value = passport[field]
                if field == "byr":
                    match = re.fullmatch(r"[\d]{4}", value)
                    if match:
                        matchedValue = int(match.group())
                        if matchedValue < 1920 or matchedValue > 2002:
                            valid = False
                            break
                    else:
                        valid = False
                        break
                elif field == "iyr":
                    match = re.fullmatch(r"[\d]{4}", value)
                    if match:
                        matchedValue = int(match.group())
                        if matchedValue < 2010 or matchedValue > 2020:
                            valid = False
                            break
                    else:
                        valid = False
                        break
                elif field == "eyr":
                    match = re.fullmatch(r"[\d]{4}", value)
                    if match:
                        matchedValue = int(match.group())
                        if matchedValue < 2020 or matchedValue > 2030:
                            valid = False
                            break
                    else:
                        valid = False
                        break
                elif field == "hgt":
                    match = re.fullmatch(r"[\d]+(cm|in)", value)
                    if match:
                        matchedValue = match.group()
                        type = matchedValue[-2:]
                        num = int(matchedValue[:-2])
                        if type == "cm":
                            if num < 150 or num > 193:
                                valid = False
                                break
                        elif type == "in":
                            if num < 59 or num > 76:
                                valid = False
                                break
                    else:
                        valid = False
                        break
                elif field == "hcl":
                    match = re.fullmatch(r"#[\d|a-f]{6}", value)
                    if not match:
                        valid = False
                        break
                elif field == "ecl":
                    if value not in EYECOLOURS:
                        valid = False
                        break
                elif field == "pid":
                    match = re.fullmatch(r"[\d]{9}", value)
                    if not match:
                        valid = False
                        break
        totalValid += 1 if valid else 0
        valid = True
    return totalValid

passports = []
with open("./data/q4.txt", "r") as file:
    currPassport = {}
    for line in file:
        if line == "\n":
            passports.append(currPassport)
            currPassport = {}
        else:
            fields = re.findall(r"[a-z]{3}:[^\s]+", line)
            for field in fields:
                kv = field.split(":")
                currPassport[kv[0]] = kv[1]
    passports.append(currPassport)


print("Answer to part1: {}".format(part1(passports)))
print("Answer to part2: {}".format(part2(passports)))