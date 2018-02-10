from decimal import *
import matplotlib.pyplot as plt

def drange(start, stop, step):
    r = Decimal(str(start))
    while r < Decimal(str(stop)):
        yield float(r)
        r += Decimal(str(step))

def ndsolve(fs, start, fr, to):
    delta = 0.01
    traj = [[fr, *start]]
    last = start
    for t in drange(fr, to, delta):
        next = []
        for i, f in enumerate(fs):
            next.append(traj[-1][i + 1] + delta * f(*traj[-1][1:]))
        traj.append([t, *next])
    return traj

fs = [lambda x, y: x - x ** 3 - y, lambda x, y: x]
traj = ndsolve(fs, [2, 1], 0, 30)
ttr, xtr, ytr = zip(*traj)
plt.plot(xtr, ytr)
plt.show()
