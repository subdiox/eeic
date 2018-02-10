list = [4, 8, 2, 6]

for i in range(1, len(list)):
    for j in range(0, len(list) - i):
        if list[j] > list[j + 1]:
            tmp = list[j]
            list[j] = list[j + 1]
            list[j + 1] = tmp
print(list)
