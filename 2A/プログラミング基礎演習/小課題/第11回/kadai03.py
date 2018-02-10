import numbers
class Matrix:
    def __init__(self, m11, m12 = 0, m21 = 0, m22 = ""):
        if isinstance(m11, numbers.Number) and isinstance(m22, str):
            self.m11 = m11
            self.m12 = 0
            self.m21 = 0
            self.m22 = m11
        elif isinstance(m11, numbers.Number) and isinstance(m12, numbers.Number) and isinstance(m21, numbers.Number) and isinstance(m22, numbers.Number):
            self.m11 = m11
            self.m12 = m12
            self.m21 = m21
            self.m22 = m22
        else:
            return TypeError('All arguments of __init__ method need to be number.')

    def __add__(self, m):
        if isinstance(m, Matrix):
            m11 = self.m11 + m.m11
            m12 = self.m12 + m.m12
            m21 = self.m21 + m.m21
            m22 = self.m22 + m.m22
            return Matrix(m11, m12, m21, m22)
        elif isinstance(m, numbers.Number):
            return self + Matrix(m)
        else:
            return TypeError('Argument of __add__ method needs be number or Matrix.')

    def __sub__(self, m):
        if isinstance(m, Matrix):
            m11 = self.m11 - m.m11
            m12 = self.m12 - m.m12
            m21 = self.m21 - m.m21
            m22 = self.m22 - m.m22
            return Matrix(m11, m12, m21, m22)
        elif isinstance(m, numbers.Number):
            return self - Matrix(m)
        else:
            return TypeError('Argument of __sub__ method needs be number or Matrix.')

    def __mul__(self, m):
        if isinstance(m, Matrix):
            m11 = self.m11 * m.m11 + self.m12 * m.m21
            m12 = self.m11 * m.m12 + self.m12 * m.m22
            m21 = self.m21 * m.m11 + self.m22 * m.m21
            m22 = self.m21 * m.m12 + self.m22 * m.m22
            return Matrix(m11, m12, m21, m22)
        elif isinstance(m, numbers.Number):
            m11 = self.m11 * m
            m12 = self.m12 * m
            m21 = self.m21 * m
            m22 = self.m22 * m
            return Matrix(m11, m12, m21, m22)
        else:
            raise TypeError('Argument of __mul__ method needs to be number or Matrix.')

    def __pow__(self, n):
        if isinstance(n, int):
            m = Matrix(1)
            for i in range(1, n):
                m *= self
            return m
        else:
            raise TypeError('Argument of __pow__ method needs to be integer.')

    def expk(self, k):
        if k == 0:
            return Matrix(1)
        else:
            return self.expk(k - 1) * (1 / k) * self

    def exp(self):
        s = Matrix(0)
        for k in range(0, 100):
            s = s + self.expk(k)
        return s

    def __repr__(self):
        s = f'[{self.m11}, {self.m12}]\n'
        s += f'[{self.m21}, {self.m22}]'
        return s

print(Matrix(1, 2, 3, 4).exp())
