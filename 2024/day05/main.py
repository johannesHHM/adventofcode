rules = {}

middlesum = 0
fixedsum = 0

file = open("input")

def numbcorrect(numb, page):
    return numb in rules and any(check in rules.get(numb) for check in page)

def testpage(page):
    while page:
        current = page.pop()
        if numbcorrect(current, page):
            return False
    return True

def fixpage(page):
    newpage = []
    while page:
        for numb in page:
            if not numbcorrect(numb, page):
                newpage.append(numb)
                page.remove(numb)
                break
    return newpage

for line in file:
    if line == "\n":
        break
    numbs = line.split('|')
    before = int(numbs[0])
    after = int(numbs[1])

    if before not in rules:
        rules[before] = []
    rules[before].append(after)

for line in file:
    page = list(map(int, line[:-1].split(',')))
    middle = page[len(page)//2]
    if testpage(page.copy()):
        middlesum += middle
    else:
        page = fixpage(page)
        fixedsum += page[len(page)//2]

print(f"Result part 1: {middlesum}")
print(f"Result part 2: {fixedsum}")
