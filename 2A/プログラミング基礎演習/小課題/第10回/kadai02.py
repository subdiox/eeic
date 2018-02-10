class Matrix:
    def __init__(self, m11, m12, m21, m22):
        self.m11 = m11
        self.m12 = m12
        self.m21 = m21
        self.m22 = m22

    def add(self, matrix):
        m11 = self.m11 + matrix.m11
        m12 = self.m12 + matrix.m12
        m21 = self.m21 + matrix.m21
        m22 = self.m22 + matrix.m22
        return Matrix(m11, m12, m21, m22)

    def mul(self, matrix):
        m11 = self.m11 * matrix.m11 + self.m12 * matrix.m21
        m12 = self.m11 * matrix.m12 + self.m12 * matrix.m22
        m21 = self.m21 * matrix.m11 + self.m22 * matrix.m21
        m22 = self.m21 * matrix.m12 + self.m22 * matrix.m22
        return Matrix(m11, m12, m21, m22)

    def conmul(self, c):
        m11 = self.m11 * c
        m12 = self.m12 * c
        m21 = self.m21 * c
        m22 = self.m22 * c
        return Matrix(m11, m12, m21, m22)

    def expk(self, k):
        if k == 0:
            return Matrix(1, 0, 0, 1)
        else:
            return self.expk(k - 1).conmul(1 / k).mul(self)

    def exp(self):
        s = Matrix(0, 0, 0, 0)
        for k in range(0, 100):
            s = s.add(self.expk(k))
        return s

    def __repr__(self):
        s = f'[{self.m11}, {self.m12}]\n'
        s += f'[{self.m21}, {self.m22}]'
        return s

print(Matrix(1, 2, 3, 4).exp())
