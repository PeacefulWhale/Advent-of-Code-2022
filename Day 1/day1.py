import sys

elves = []
with open(sys.argv[1], "r") as file:
    count = 0
    for line in file.readlines():
        if line == "\n":
            elves.append(count)
            count = 0
        else:
            count += int(line)
elves.sort(reverse=True)

print(f"Part 1: {elves[0]}")
print(f"Part 2: {sum(elves[0:3])}")
