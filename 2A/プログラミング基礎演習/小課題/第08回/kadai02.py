product = 1
prime = []
for i in range(2, 10000):
    is_prime = True
    for j in prime:
        if i % j == 0:
            is_prime = False
    if is_prime:
        prime.append(i)
        product *= (1 - 1 / i ** 2)
pi = (6 / product) ** (1 / 2)
print(pi)
