def p(x, n):
    if n == 0:
        return 1
    elif n == 1:
        return x
    else:
        return ((2 * n - 1) * x * p(x, n - 1) - (n - 1) * p(x, n - 2)) / n

print(p(0.5, 16))
print(p(0.7, 32))
