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

    def __repr__(self):
        s = f'[{self.m11}, {self.m12}]\n'
        s += f'[{self.m21}, {self.m22}]'
        return s

m1 = Matrix(1, 2, 5, 1)
m2 = Matrix(2, 3, 4, 2)
print(m1.add(m2))
print(m1.mul(m2))
