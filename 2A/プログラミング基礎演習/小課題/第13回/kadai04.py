import numpy as np
from sympy import *
import sys
sys.setrecursionlimit(10000)
#init_printing()

def volume(f, g, x, y, z): # f:球の方程式, g:平面の方程式
    n = 5000
    xc = Poly(f, x).all_coeffs()
    yc = Poly(f, y).all_coeffs()
    zc = Poly(f, z).all_coeffs()
    xm = xc[1] / xc[0] / 2
    ym = yc[1] / yc[0] / 2
    zm = zc[1] / zc[0] / 2
    x0, x1 = solve(f.subs([(y, -ym), (z, -zm)]))
    y0, y1 = solve(f.subs([(z, -zm), (x, -xm)]))
    z0, z1 = solve(f.subs([(x, -xm), (y, -ym)]))
    rec_vol = Abs(x1 - x0) * Abs(y1 - y0) * Abs(z1 - z0)
    return (count(f, g, x, y, z, x0, y0, z0, x1, y1, z1, n) / n * rec_vol).evalf()

def count(f, g, x, y, z, x0, y0, z0, x1, y1, z1, n):
    xval = np.random.rand() * (x1 - x0) + x0
    yval = np.random.rand() * (y1 - y0) + y0
    zval = np.random.rand() * (z1 - z0) + z0
    fval = f.subs([(x, xval), (y, yval), (z, zval)])
    gval = g.subs([(x, xval), (y, yval), (z, zval)])
    if n == 0:
        if fval < 0 and gval > 0:
            return 1
        else:
            return 0
    elif fval < 0 and gval > 0:
        return count(f, g, x, y, z, x0, y0, z0, x1, y1, z1, n - 1) + 1
    else:
        return count(f, g, x, y, z, x0, y0, z0, x1, y1, z1, n - 1)

x, y, z = symbols('x y z')
print(volume(2*x**2 + 3*y**2 + z**2 - 4, x + y + 2*z - 1, x, y, z))
