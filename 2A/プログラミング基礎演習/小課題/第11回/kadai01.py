class MyRational():
    def __init__(self, d, m):
        gcd = self.gcd(d, m)
        self.d = int(d / gcd)   # 分子(denominator)
        self.m = int(m / gcd)   # 分母(molecule)

    def __repr__(self):
        if self.d % self.m == 0:
            return f'{self.d / self.m}'
        return f'{self.d} / {self.m}'

    def __add__(self, r):
        dd = self.m * r.d + self.d * r.m
        mm = self.m * r.m
        return MyRational(dd, mm)

    def __mul__(self, r):
        dd = self.d * r.d
        mm = self.m * r.m
        return MyRational(dd, mm)

    def gcd(self, a, b):
        while b:
            a, b = b, a % b
        return a

r1 = MyRational(2, 3)
print(r1)
r2 = MyRational(1, 3)
print(r1 * r2)
