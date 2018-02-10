import numpy as np

def nintegrate(f, fr, to, n):
    y = 0
    step = (to - fr) / n
    for x in np.arange(fr, to, step):
        y += f(x) * step
    return y

print(nintegrate(lambda x: x + x ** 2, 0, 10, 100))
