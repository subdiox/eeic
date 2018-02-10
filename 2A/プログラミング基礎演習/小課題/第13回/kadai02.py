import sympy
#sympy.init_printing()
def legendre_poly(n, x):
    return p(n, x)
def p(n, x):
    if n == 0:
        return 1
    elif n == 1:
        return x
    else:
        return (sympy.Rational((2 * n - 1), n) * x * p(n - 1, x) - sympy.Rational((n - 1), n) * p(n - 2, x)).simplify()

x = sympy.Symbol('x')
print(legendre_poly(4, x))
print(legendre_poly(10, x))
