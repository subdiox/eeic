import sympy
#sympy.init_printing()
def taylor(f, x, a, n):
    expr = 0
    for i in range(0, n):
        expr += sympy.Rational(1, sympy.factorial(i)) * f.subs([(x, a)]) * (x - a) ** i
        f = sympy.diff(f)
    return expr

x = sympy.Symbol('x')
print(taylor(sympy.sin(x), x, 0, 4))
