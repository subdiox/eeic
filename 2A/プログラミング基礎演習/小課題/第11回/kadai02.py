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

class MyInteger(MyRational):
    def __init__(self, n):
        super().__init__(n, 1)

    def factorial(self):
        if self.d == 0: return 1
        return self.d * MyInteger(self.d - 1).factorial()

    def is_prime(self):
        for i in range(2, self.d - 1):
            if self.d % i == 0: return False
        return True

i1 = MyInteger(10)
print(i1.factorial())

i2 = MyInteger(101)
print(i2.is_prime())
